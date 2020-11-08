#pragma once

#include <string>

class DuplicateFinder
{
public:
    DuplicateFinder() = delete;
    DuplicateFinder(bool searchByHash, bool searchByMeta, std::string hashType);

private:
    bool _searchByHash;
    bool _searchByMeta;
    std::string _hashType;
};