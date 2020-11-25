#pragma once

#include "hash.h"

#include <QFile>
#include <boost/filesystem.hpp>
#include <hash.h>
#include <iostream>
#include <vector>
/**
 * @brief Структура с хешем блока данных
 */
struct HashNode
{
    std::vector<unsigned int> data;
    bool operator==(const HashNode& other) const;
    bool operator!=(const HashNode& other) const;
};

class HashedFile
{
public:
    /**
     * @brief Конструктор класса HashedFile
     * @param path - путь к файлу
     * @param hash_blocksize - размер блока для хеширования, в байтах
     * @param hasher - тип используемого хешера. @see hash.h
     */
    HashedFile(boost::filesystem::path path, uintmax_t hash_blocksize, std::shared_ptr<IHash> hasher);

    /**
     * @brief Конструктор копирования
     * @param file - файл для копирования
     */
    HashedFile(const HashedFile& file);

    /**
     * @brief Произвести сравнение двух файлов
     * @param other - Файл, с которым производится сравнение
     * @return true, если файлы совпадают, иначе false
     */
    bool Equal(HashedFile& other);

    /**
     * @brief Возвращает путь к файлу
     * @return Путь к файлу
     */
    boost::filesystem::path GetFilePath() const;

private:
    const boost::filesystem::path filepath_;
    const uintmax_t filesize_;
    const size_t max_blocks_amount_;
    const uintmax_t blocksize_;
    std::shared_ptr<IHash> hasher_;
    std::vector<HashNode> hash_data_;
    //    std::unique_ptr<std::ifstream> file_handle_;

    /**
     * @brief Открыть файл и перейти в место последнего нехешированного блока
     */
    void OpenHandle();

    /**
     * @brief Закрыть файл
     */
    void CloseHandle();

    /**
     * @brief Прочитать следующий блок данных из файла
     * @return Указатель на следующий блок данных
     */
    std::unique_ptr<char[]> GetNextBlock();

    /**
     * @brief Посчитать хеш до блока с индексом block_index включительно
     * @param block_index - блок, по который считается хеш
     */
    void CalcHashUntil(size_t block_index);

    /**
     * @brief Получить индекс блока с индексом block_index
     * @param block_index - индекс блока, хеш которого надо получить
     * @return хеш блока
     */
    HashNode GetHashNode(size_t block_index);
};

struct SetHash
{
    std::size_t operator()(const HashedFile& _node) const
    {
        return std::hash<std::string>()(_node.GetFilePath().string());
    }
};