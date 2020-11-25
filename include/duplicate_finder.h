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
                    const QStringList& includeDirectories,
                    const QStringList& excludeDirectories,
                    const QStringList& includeMasks,
                    const QStringList& excludeMasks);

    /**
     * @brief Find duplicate files
     */
    void Find();

private:
    bool _searchByHash;
    bool _searchByMeta;
    std::string _hashType;

    std::vector<boost::filesystem::path> _include_dirs;
    std::vector<boost::filesystem::path> _exclude_dirs;
    std::vector<boost::regex> _includeMasks;
    std::vector<boost::regex> _excludeMasks;
    const size_t _block_size = 512;
    const size_t _scan_depth;
    const unsigned long long int _min_file_size;
    std::shared_ptr<IHash> _hasher;

    std::unordered_set<std::string> _scanned_file_paths;

    std::unordered_map<uintmax_t, std::vector<HashedFile>> _filesToScan;
    uintmax_t _totalFiles;
    uintmax_t _completedFiles;
    std::unordered_map<std::string, std::unordered_set<std::string>> _totalDuplicates;

    /**
    * @brief Set directories from vector of strings
    * @param dirs - vector of paths to directories
    * @throw std::runtime_error if one of directories doesn't exists
    * @return vector of boost paths
    */
    std::vector<boost::filesystem::path> TrySetDirs(const std::vector<std::string>& dirs);

    /**
     * @brief Scan vector of (pointer to) files of same size to find duplicates
     * @param files - files for scanning
     */
    void Scan(std::vector<HashedFile>& files);

    /**
     * @brief Set hasher type for scanning
     * @param hasher - string of hasher name. Possible names: crc32/md5/sha1
     * @throw std::runtime_error if hasher name not in possible names list
     * @return shared_ptr to hasher
     */
    std::shared_ptr<IHash> TrySetHasher(const std::string& hasher);

    /**
     * @brief Scan directory
     * @param path - path to scanning
     * @param depth - depth of scanning, 0 - unlimited
     */
    void ScanPath(const boost::filesystem::path& path, size_t depth);

    /**
     * @brief Add file to analyzing
     * @param path - path to file
     */
    void AddFile(const boost::filesystem::path& path);

    /**
     * @brief Check if scanned path in excluded directories
     * @param path - path to check
     * @return true if path in excluded dirs, otherwise false
     */
    bool InExcludeDirs(const boost::filesystem::path& path);

    /**
     * @brief Check if path satisfied at least one mask from vector
     * @param path - path to file
     * @param masksToCheck  - vector of masks to check
     * @return true if satisfied, otherwise false
     */
    bool MasksSatisfied(const boost::filesystem::path& path, const std::vector<boost::regex>& masksToCheck);

    /**
     * @brief Check if file was already in duplicates
     * @param path - path to file to check
     * @param duplicates - duplicate that was found earlier
     * @return true if file already in duplicates, otherwise false
     */
    bool AlreadyInDuplicates(const boost::filesystem::path& path,
                             std::unordered_map<std::string, std::unordered_set<std::string>>& duplicates);
};