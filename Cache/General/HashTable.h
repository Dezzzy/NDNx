//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "stdint.h"
#include "MurmurHash.h"
#include "SpookyHash.h"

class HashTable {
public:
    enum HASH_TABLE_INSTRUCION_TYPES{
        INSERT_NAME,
        UPDATE_NAME,
        NAME_DELETED,
        NAME_NOT_FOUND
    };

    const static int CacheSize = 128;
    const static int WordSize = 100;
    SpookyHash* spookyHasher;
    MurmurHash* mmHasher;
    HashTable();
    virtual ~HashTable();
    int searchNameCache(const char* name, char** NameCache,int* CacheMemory, int* BloomFilter);
    int updateNameCache(const char* name, char** NameCache, int* CacheMemory,int* cacheIndex, int* BloomFilter);
    int deleteFromNameCache(const char* name, char** NameCache,int* CacheMemory,int* BloomFilter,int* cacheIndex);
    int bloomFilterSearch(uint32_t k1, uint32_t k2, int* BloomFilter);
    void bloomFilterInsert(uint32_t k1, uint32_t k2, int* BloomFilter);
    void getBloomFilter(int* BloomFilter, int* newBloomFilter);
    void getHashValues(const char* name,uint32_t* hashValues);
    uint32_t resolveCollision(uint32_t i1, uint32_t i2);
};

#endif /* HASHTABLE_H_ */
