#pragma once

#include <QCryptographicHash>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <vector>

class IHash
{
public:
    virtual std::vector<unsigned int> Hash(const char* buffer) = 0;
    virtual ~IHash() = default;
    // Rule of five methods. @see https://en.cppreference.com/w/cpp/language/rule_of_three
    IHash() = default;
    IHash(const IHash& other) = delete;            // Can't copy
    IHash(IHash&& other) = delete;                 // Can't move
    IHash& operator=(const IHash& other) = delete; // No copy assignment
    IHash& operator=(IHash&& other) = delete;      // No move assignment
};

/**
 * @brief Класс для хеширования методом SHA1
 */
class SHA1Hasher : public IHash
{
public:
    SHA1Hasher() = default;

    /**
     * @brief Хеширование методом SHA1
     * @param buffer - буффер для хеширования
     * @return std::vector<unsigned int> - результат хеширования
     */
    std::vector<unsigned int> Hash(const char* buffer) override;
};

/**
 * @brief Класс для хеширования методом CRC32
 */
class CRC32Hasher : public IHash
{
public:
    CRC32Hasher() = default;

    /**
     * @brief Хеширование методом CRC32
     * @param buffer - буффер для хеширования
     * @return std::vector<unsigned int> - результат хеширования
     */
    std::vector<unsigned int> Hash(const char* buffer) override;
};

/**
 * @brief Класс для хеширования методом MD5
 */
class MD5Hasher : public IHash
{
public:
    MD5Hasher() = default;

    /**
     * @brief Хеширование методом MD5
     * @param buffer - буффер для хеширования
     * @return std::vector<unsigned int> - результат хеширования
     */
    std::vector<unsigned int> Hash(const char* buffer) override;
};