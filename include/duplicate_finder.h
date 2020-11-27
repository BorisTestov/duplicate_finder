#pragma once

#include "hashed_file.h"

#include <QPointer>
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
                    QCryptographicHash::Algorithm hash_method,
                    size_t depth,
                    unsigned int minSize,
                    const QStringList& includeDirectories,
                    const QStringList& excludeDirectories,
                    const QStringList& includeMasks,
                    const QStringList& excludeMasks);

    /**
     * @brief Find duplicate files
     * @return List of duplicates
     */
    std::unordered_map<std::string, std::unordered_set<std::string>> Find();

private:
    bool _searchByHash;
    bool _searchByMeta;

    std::vector<boost::filesystem::path> _include_dirs;
    std::vector<boost::filesystem::path> _exclude_dirs;
    std::vector<boost::regex> _includeMasks;
    std::vector<boost::regex> _excludeMasks;
    const size_t _scan_depth;
    const unsigned long long int _min_file_size;
    const QCryptographicHash::Algorithm _hash_method;

    std::unordered_set<std::string> _scanned_file_paths;

    std::unordered_map<uintmax_t, std::vector<QPointer<HashedFile>>> _filesToScan;
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
    void FindDuplicates(std::vector<QPointer<HashedFile>>& files);

    /**
     * @brief Scan directory for adding files for future duplicate finding or running scan of subdirectories
     * @param path - path to scanning
     * @param depth - depth of scanning
     */
    void ScanDirectory(const boost::filesystem::path& path, size_t depth);

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