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
    /**
     * @brief Конструктор класса для поиска файлов-дубликатов
     * @param searchByHash - Поиск по хешу. Файл дубликат, если совпадают хеши
     * @param searchByMeta  - Поиск по метаданным (имя и размер). Файл дубликат, если совпадают имя и размер
     * @param hash_method - метод хеширования
     * @param depth - глубина сканироания директорий
     * @param minSize - минимальный размер сканируемого файла, в байтах
     * @param includeDirectories - директории для сканирования
     * @param excludeDirectories - директории, исключенные из сканирования
     * @param includeMasks - маски, включенные в сканирование. Если не пустой список, то сканирует только то, что подходит под маску
     * @param excludeMasks - маски, исключенные из сканирования
     */
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
     * @brief Найти файлы-дубликаты
     * @return Список дубликатов в формате: [оригинал: набор дубликатов]
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
    * @brief Установить список директорий из вектора строк (QStringList)
    * @param dirs - директории
    * @throw std::runtime_error, если одна из директорий не существует
    * @return вектор boost::filesystem::path
    * @todo обрабатывать исключение с помощью msgbox
    */
    std::vector<boost::filesystem::path> TrySetDirs(const std::vector<std::string>& dirs);

    /**
     * @brief - искать дубликаты в добавленных для анализа файлов
     * @param files - Вектор указателей на файлы для анализа
     */
    void FindDuplicates(std::vector<QPointer<HashedFile>>& files);

    /**
     * @brief Сканировать директорию для добавления файлов или для запуска сканированию поддиректорий
     * @param path - Путь для сканирования
     * @param depth - Глубина сканирования
     */
    void ScanDirectory(const boost::filesystem::path& path, size_t depth);

    /**
     * @brief Добавить файл для абудущего анализа на дубликаты
     * @param path - путь к файлу
     */
    void AddFile(const boost::filesystem::path& path);

    /**
     * @brief Проверить, находится ли сканируемая директория в списке исключенных
     * @param path - директория для проверки
     * @return true если находится, иначе false
     */
    bool InExcludeDirs(const boost::filesystem::path& path);

    /**
     * @brief Проверить, что файл удовлетворяет минимум одной маске из вектора масок
     * @param path - путь к файлу
     * @param masksToCheck  - вектор масок для проверки
     * @return true если удовлетворяет, иначе false
     */
    bool MasksSatisfied(const boost::filesystem::path& path, const std::vector<boost::regex>& masksToCheck);

    /**
     * @brief Проверить, записан ли файл как дубликат
     * @param path - Путь к файлу для проверки
     * @param duplicates - Дубликаты для поиска
     * @return true если файл уже является дубликатом, иначе false
     * @todo можно заменить на флаг
     */
    bool AlreadyInDuplicates(const boost::filesystem::path& path,
                             std::unordered_map<std::string, std::unordered_set<std::string>>& duplicates);
};