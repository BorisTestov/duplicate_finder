#include "duplicate_finder.h"

#include <iostream>

DuplicateFinder::DuplicateFinder(bool searchByHash, bool searchByMeta, std::string hashType) :
    _searchByHash(searchByHash),
    _searchByMeta(searchByMeta),
    _hashType(hashType)
{
    std::cout << "Duplicate finder created: " << std::endl;
    std::cout << std::boolalpha << "Search by hash: " << _searchByHash << std::endl;
    if (_searchByHash)
    {
        std::cout << "Chosen hash type: " << _hashType << std::endl;
    }
    std::cout << std::boolalpha << "Search by metadata: " << _searchByMeta << std::endl;
}
