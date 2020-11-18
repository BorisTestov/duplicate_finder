#pragma once

#include "hash.h"
#include "hashed_file.h"

#include <QStringList>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>

class DuplicateFinder
{
public:
    DuplicateFinder() = delete;
    DuplicateFinder(bool searchByHash,
                    bool searchByMeta,
                    std::string hashType,
                    size_t depth,
                    unsigned int minSize,
                    QStringList includeDirectories,
                    QStringList excludeDirectories,
                    QStringList includeMasks,
                    QStringList excludeMasks);

    /**
     * @brief Find duplicate files
     * @return - map of duplicates: path to file and vector of duplicates
     */
    std::unordered_map<std::string, std::unordered_set<std::string>> Find();

    /**
     * @brief Set directories from vector of strings
     * @param dirs - vector of paths to directories
     * @throw std::runtime_error if one of directories doesn't exists
     * @return vector of boost paths
     */
    std::vector<boost::filesystem::path> TrySetDirs(const std::vector<std::string>& dirs);

private:
    bool _searchByHash;
    bool _searchByMeta;
    std::string _hashType;

    const std::vector<boost::filesystem::path> _include_dirs;
    const std::vector<boost::filesystem::path> _exclude_dirs;
    const std::vector<boost::regex> _filemasks;
    const size_t _block_size = 512;
    const size_t _scan_depth;
    const unsigned long long int _min_file_size;
    std::shared_ptr<IHash> _hasher;

    std::vector<HashedFile> _files;
    std::unordered_set<std::string> _scanned_file_paths;

    /**
     * @brief Set hasher type for scanning
     * @param hasher - string of hasher name. Possible names: crc32/md5/sha1
     * @throw std::runtime_error if hasher name not in possible names list
     * @return shared_ptr to hasher
     */
    std::shared_ptr<IHash> TrySetHasher(const std::string& hasher);

    /**
     * @brief Set filemasks for scanning
     * @param filemasks - vector of filemasks as std::string
     * @return vector of filemasks as boost::regex
     */
    std::vector<boost::regex> SetFileMasks(const std::vector<std::string>& filemasks);

    void ScanPath(const boost::filesystem::path& path, size_t depth);

    void AddFile(const boost::filesystem::path& path);

    bool InExcludeDirs(const boost::filesystem::path& path);

    bool MasksSatisfied(const boost::filesystem::path& path);

    bool AlreadyInDuplicates(const boost::filesystem::path& path,
                             std::unordered_map<std::string, std::unordered_set<std::string>>& duplicates);
};