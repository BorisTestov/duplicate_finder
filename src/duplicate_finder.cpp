#include "duplicate_finder.h"

#include <QtConcurrent/QtConcurrent>
#include <common.h>
#include <iostream>

DuplicateFinder::DuplicateFinder(bool searchByHash,
                                 bool searchByMeta,
                                 QCryptographicHash::Algorithm hash_method,
                                 size_t depth,
                                 unsigned int minSize,
                                 const QStringList& includeDirectories,
                                 const QStringList& excludeDirectories,
                                 const QStringList& includeMasks,
                                 const QStringList& excludeMasks) :
    _searchByHash(searchByHash),
    _searchByMeta(searchByMeta),
    _scan_depth(depth),
    _min_file_size(minSize),
    _hash_method(hash_method)
{
    std::vector<std::string> includePathsVector = common::toVectorOfStrings(includeDirectories);
    std::vector<std::string> excludePathsVector = common::toVectorOfStrings(excludeDirectories);
    _include_dirs = TrySetDirs(includePathsVector);
    _exclude_dirs = TrySetDirs(excludePathsVector);
    _includeMasks = common::toVectorOfRegex(includeMasks);
    _excludeMasks = common::toVectorOfRegex(excludeMasks);
}

std::unordered_map<std::string, std::unordered_set<std::string>> DuplicateFinder::Find()
{
    /**
     * scan directories and add files to multimap
     */
    for (const auto& dir : _include_dirs)
    {
        for (const boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(dir))
        {
            ScanDirectory(entry.path(), _scan_depth);
        }
    }

    /**
     * get keys of multimap - all different sizes of files
     */
    std::vector<uintmax_t> keys;
    for (const auto& pair : _filesToScan)
    {
        if (std::find(keys.begin(), keys.end(), pair.first) == keys.end())
        {
            keys.emplace_back(pair.first);
        }
    }

    /**
     * create separate vector for each filesize
     * then run scan process in parallel
     */
    std::vector<QFuture<void>> processes;
    for (const auto& key : keys)
    {
        std::vector<QPointer<HashedFile>> files = _filesToScan[key];
        processes.emplace_back(QtConcurrent::run(this, &DuplicateFinder::FindDuplicates, files));
    }

    /**
     * wait for all processes
     */
    for (auto& process : processes)
    {
        process.waitForFinished();
    }
    for (const auto& duplicates : _totalDuplicates)
    {
        std::cout << duplicates.first << std::endl;
        for (const auto& path : duplicates.second)
        {
            std::cout << path << std::endl;
        }
        std::cout << std::endl;
    }
    return _totalDuplicates;
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

void DuplicateFinder::FindDuplicates(std::vector<QPointer<HashedFile>>& files)
{
    if (files.size() < 2)
    {
        return;
    }
    std::unordered_map<std::string, std::unordered_set<std::string>> localDuplicates;
    for (const QPointer<HashedFile>& first_file : files)
    {
        if (AlreadyInDuplicates(first_file->GetFilePath(), localDuplicates))
        {
            continue;
        }
        for (const QPointer<HashedFile>& second_file : files)
        {
            if (AlreadyInDuplicates(second_file->GetFilePath(), localDuplicates))
            {
                continue;
            }
            if (first_file->GetFilePath() == second_file->GetFilePath())
            {
                continue;
            }
            bool files_equal = true;
            if (_searchByHash)
            {
                files_equal = files_equal and first_file->Equal(second_file);
            }
            if (_searchByMeta)
            {
                auto first_file_name = first_file->GetFilePath().filename().string();
                auto second_file_name = second_file->GetFilePath().filename().string();
                files_equal = files_equal and (first_file_name == second_file_name);
            }
            if (files_equal)
            {
                localDuplicates[first_file->GetFilePath().string()].insert(second_file->GetFilePath().string());
                _totalDuplicates[first_file->GetFilePath().string()].insert(second_file->GetFilePath().string());
            }
        }
    }
}

void DuplicateFinder::ScanDirectory(const boost::filesystem::path& path, size_t depth)
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
                ScanDirectory(dir.path(), depth - 1);
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
    if (not MasksSatisfied(path, _includeMasks) and not _includeMasks.empty())
    {
        return;
    }
    if (MasksSatisfied(path, _excludeMasks) and not _excludeMasks.empty())
    {
        return;
    }
    std::string abs_path = weakly_canonical(absolute(path)).string();
    if (_scanned_file_paths.find(abs_path) == _scanned_file_paths.end())
    {
        _scanned_file_paths.insert(abs_path);
        _filesToScan[file_size(path)].emplace_back(QPointer<HashedFile>(new HashedFile(abs_path, _hash_method)));
        return;
    }
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

bool DuplicateFinder::MasksSatisfied(const boost::filesystem::path& path, const std::vector<boost::regex>& masksToCheck)
{
    std::string filename = path.filename().string();
    for (const auto& mask : masksToCheck)
    {
        if (boost::regex_match(filename, mask))
        {
            return true;
        }
    }
    return false;
}

bool DuplicateFinder::AlreadyInDuplicates(const boost::filesystem::path& path,
                                          std::unordered_map<std::string, std::unordered_set<std::string>>& duplicates)
{
    bool already_in_duplicates = false;
    const std::string& filepath = path.string();
    for (const auto& duplicate : duplicates)
    {
        if (duplicate.second.find(filepath) != duplicate.second.end())
        {
            already_in_duplicates = true;
            break;
        }
    }
    return already_in_duplicates;
}
