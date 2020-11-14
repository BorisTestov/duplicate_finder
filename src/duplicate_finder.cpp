#include "duplicate_finder.h"

#include <iostream>

DuplicateFinder::DuplicateFinder(bool searchByHash, bool searchByMeta, std::string hashType) :
    _searchByHash(searchByHash),
    _searchByMeta(searchByMeta),
    _hashType(hashType),
    _block_size(512),
    _scan_depth(5),
    _min_file_size(1)
{
    std::cout << "Duplicate finder created: " << std::endl;
    std::cout << std::boolalpha << "Search by hash: " << _searchByHash << std::endl;
    if (_searchByHash)
    {
        std::cout << "Chosen hash type: " << _hashType << std::endl;
    }
    std::cout << std::boolalpha << "Search by metadata: " << _searchByMeta << std::endl;
}

std::unordered_map<std::string, std::unordered_set<std::string>> DuplicateFinder::Find()
{
    for (const auto& dir : _include_dirs)
    {
        for (const boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(dir))
        {
            ScanPath(entry.path(), _scan_depth);
        }
    }
    std::unordered_map<std::string, std::unordered_set<std::string>> duplicates;
    if (_files.size() < 2)
    {
        return duplicates;
    }
    for (auto first_file = std::begin(_files); first_file != end(_files); first_file++)
    {
        if (AlreadyInDuplicates(first_file->GetFilePath(), duplicates))
        {
            continue;
        }
        for (auto second_file = std::begin(_files); second_file != end(_files); second_file++)
        {
            if (AlreadyInDuplicates(second_file->GetFilePath(), duplicates))
            {
                continue;
            }
            if (first_file->GetFilePath() == second_file->GetFilePath())
            {
                continue;
            }
            if (first_file->Equal(*second_file))
            {
                auto first_file_path = first_file->GetFilePath().string();
                auto second_file_path = second_file->GetFilePath().string();
                duplicates[first_file_path].insert(second_file_path);
            }
        }
    }
    return duplicates;
}

std::vector<boost::filesystem::path> DuplicateFinder::TrySetDirs(const std::vector<std::string>& dirs)
{
    std::vector<boost::filesystem::path> directories;
    for (const std::string& dir : dirs)
    {
        auto path = boost::filesystem::weakly_canonical(boost::filesystem::absolute(dir));
        if (not boost::filesystem::exists(path))
        {
            throw std::runtime_error("Directory doesn't exists: " + dir);
        }
        if (not boost::filesystem::is_directory(path))
        {
            throw std::runtime_error("Path isn't a directory: " + dir);
        }
        directories.push_back(path);
    }
    return directories;
}

std::shared_ptr<IHash> DuplicateFinder::TrySetHasher(const std::string& hasher)
{
    if (hasher == "md5")
    {
        return std::make_shared<MD5Hasher>();
    }
    if (hasher == "sha1")
    {
        return std::make_shared<SHA1Hasher>();
    }
    if (hasher == "crc32")
    {
        return std::make_shared<CRC32Hasher>();
    }
    throw std::runtime_error("Hasher type doesn't recognized: " + hasher);
}

std::vector<boost::regex> DuplicateFinder::SetFileMasks(const std::vector<std::string>& filemasks)
{
    std::vector<boost::regex> resulted_masks;
    for (const std::string& mask : filemasks)
    {
        resulted_masks.emplace_back(boost::regex(mask));
    }
    return resulted_masks;
}

void DuplicateFinder::ScanPath(const boost::filesystem::path& path, size_t depth)
{
    if (boost::filesystem::exists(path) and not InExcludeDirs(path))
    {
        if (boost::filesystem::is_regular_file(path))
        {
            AddFile(path);
        }
        else if (boost::filesystem::is_directory(path) and depth > 0)
        {
            for (const boost::filesystem::directory_entry& dir : boost::filesystem::directory_iterator(path))
            {
                ScanPath(dir.path(), depth - 1);
            }
        }
    }
}

void DuplicateFinder::AddFile(const boost::filesystem::path& path)
{
    using namespace boost::filesystem;
    if (not exists(path) or not is_regular_file(path))
    {
        return;
    }
    if (file_size(path) < _min_file_size)
    {
        return;
    }
    if (not MasksSatisfied(path))
    {
        return;
    }
    std::string abs_path = weakly_canonical(absolute(path)).string();
    if (_scanned_file_paths.find(abs_path) != _scanned_file_paths.end())
    {
        return;
    }
    _scanned_file_paths.insert(abs_path);
    _files.emplace_back(HashedFile(path.string(), _block_size, _hasher));
}

bool DuplicateFinder::InExcludeDirs(const boost::filesystem::path& path)
{
    for (const auto& excluded_dir : _exclude_dirs)
    {
        if (boost::filesystem::equivalent(path, excluded_dir))
        {
            return true;
        }
    }
    return false;
}

bool DuplicateFinder::MasksSatisfied(const boost::filesystem::path& path)
{
    if (_filemasks.size() == 0)
    {
        return true;
    }
    std::string filename = path.filename().string();
    for (const auto& mask : _filemasks)
    {
        if (boost::regex_match(filename, mask))
        {
            return true;
        }
    };
    return false;
}

bool DuplicateFinder::AlreadyInDuplicates(const boost::filesystem::path& path,
                                          std::unordered_map<std::string, std::unordered_set<std::string>>& duplicates)
{
    bool already_in_duplicates = false;
    std::string filepath = path.string();
    for (const auto& duplicate : duplicates)
    {
        if (duplicate.second.find(path.string()) != duplicate.second.end())
        {
            already_in_duplicates = true;
            break;
        }
    }
    return already_in_duplicates;
}