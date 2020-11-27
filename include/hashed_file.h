#pragma once

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>

class HashedFile
{
public:
    /**
     * @brief Конструктор класса HashedFile
     * @param path - путь к файлу
     * @param hash_blocksize - размер блока для хеширования, в байтах
     */
    HashedFile(boost::filesystem::path path, QCryptographicHash::Algorithm hash_method);

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
    const uintmax_t blocksize_ = 512;
    const boost::filesystem::path filepath_;
    const uintmax_t filesize_;
    const size_t max_blocks_amount_;
    const QDateTime birthTime_;
    const QCryptographicHash::Algorithm hash_method_;
    std::vector<QByteArray> hash_data_;

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
    QByteArray GetHashNode(size_t block_index);
};