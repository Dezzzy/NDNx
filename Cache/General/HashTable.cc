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

#include "HashTable.h"
#include "MurmurHash.h"
#include "SpookyHash.h"
#include "SimpleAddress.h"

HashTable::HashTable() {
    mmHasher = new MurmurHash;
    spookyHasher = new SpookyHash;

}

HashTable::~HashTable() {

}

int HashTable::searchNameCache(const char* name, char** NameCache,int* CacheMemory, int* BloomFilter)
{
    uint32_t currentSearchIndex,firstIndex,secondIndex;
    int nameIsNotFound;
    int dataFound = 0;
    int collisionCounter = 0;
    int entryIndex;
    uint32_t hashValues[3];
    getHashValues(name,hashValues);
    currentSearchIndex = hashValues[2];
    firstIndex = hashValues[0];
    secondIndex = hashValues[1];

    if(bloomFilterSearch(firstIndex,secondIndex, BloomFilter) == 1){
        while(!dataFound && collisionCounter < 6){
            if(CacheMemory[currentSearchIndex] == 1){
                nameIsNotFound = strcmp(name, NameCache[currentSearchIndex]);
                if(nameIsNotFound){
                    currentSearchIndex = resolveCollision(firstIndex, secondIndex);
                    collisionCounter++;
                } else{
                    dataFound = 1;
                }
            }
        }
    } else{
        dataFound = 0;
    }

    if(dataFound){
        entryIndex = currentSearchIndex;
    } else{
        entryIndex = CacheSize + 1;
    }

    EV<< "the word being searched for is:"<< name << "and the value of the search function is: " << dataFound << endl;
    return entryIndex;

}

/*
 * updateNameCache:
 * This functions determines whether the given name exists with the Hash Table and then inserts or updates the entry to the Hash Table
 * INSERT:
 * adds name to the char Buffer
 * reserves the memory, by setting CacheMemory
 * and returns index pointing to new entry
 *
 * UPDATE:
 * finds cache index pointing to the mathcing Hash Entry
 */
int HashTable::updateNameCache(const char* name, char** NameCache, int* CacheMemory,int* BloomFilter,int* cacheIndex)
{
    uint32_t currentSearchIndex,firstIndex,secondIndex;
    uint32_t hashValues[3];
    int collisionCounter = 0;
    int spaceFound = 0;
    int instructionType;
    getHashValues(name,hashValues);
    currentSearchIndex = hashValues[2];
    firstIndex = hashValues[0];
    secondIndex = hashValues[1];

    currentSearchIndex = searchNameCache(name, NameCache, CacheMemory,BloomFilter);


    if(currentSearchIndex == (CacheSize + 1)){
        //find space and insert
        instructionType = INSERT_NAME;
        currentSearchIndex = hashValues[2];
        bloomFilterInsert(firstIndex, secondIndex, BloomFilter);
        while(!spaceFound && collisionCounter < 6){
            if(CacheMemory[currentSearchIndex] == 1){
                collisionCounter++;
                currentSearchIndex = resolveCollision(firstIndex, secondIndex);
                EV<<"collision found"<<endl;
            } else{
                EV<<"space found"<<endl;
                strcpy(NameCache[currentSearchIndex], name);
                CacheMemory[currentSearchIndex] = 1;
                spaceFound = 1;
                *cacheIndex = currentSearchIndex;
                EV<< currentSearchIndex<< endl;
            }
        }
    } else{
        //update at index
        instructionType = UPDATE_NAME;
        *cacheIndex = currentSearchIndex;
    }

    EV<<"the word to be updated or inserted is: "<< name << "at hash values:  " << currentSearchIndex << endl;

    return instructionType;

}

int HashTable::deleteFromNameCache(const char* name, char** NameCache,int* CacheMemory,int* BloomFilter,int* cacheIndex)
{

    *cacheIndex = searchNameCache(name, NameCache, CacheMemory, BloomFilter);
    if(*cacheIndex == (CacheSize + 1)){
        return NAME_NOT_FOUND;
    } else{
        CacheMemory[*cacheIndex] = 0;
        return NAME_DELETED;
    }
}

int HashTable::bloomFilterSearch(uint32_t k1, uint32_t k2, int* BloomFilter)
{

    if(BloomFilter[k1] == 1 && BloomFilter[k2] == 1){
        return 1;
    } else{
        return 0;
    }

}

void HashTable::bloomFilterInsert(uint32_t k1, uint32_t k2, int* BloomFilter)
{
    BloomFilter[k1] = 1;
    BloomFilter[k2] = 1;
}

void HashTable::getHashValues(const char* name,uint32_t* hashValues)
{
    int len = strlen(name);
    hashValues[0] = mmHasher->MurmurHash3(name, len, 1) % CacheSize;
    hashValues[1] = spookyHasher->SpookHash(name, len, 1) % CacheSize;
    hashValues[2] = hashValues[0];
    EV << "print out of hash values"<< hashValues[0] << "           " <<hashValues[1] << endl;
}

uint32_t HashTable::resolveCollision(uint32_t i1, uint32_t i2)
{
    i1 = i1 + i2;
    if(i1 > CacheSize){
        i1 = i1 % CacheSize;
    }
    EV <<"resolve collision value " << i1 <<endl;
    return i1;
}

void HashTable::getBloomFilter(int* BloomFilter, int* newBloomFilter)
{
    for(int i = 0; i < CacheSize; i++){
        newBloomFilter[i] = BloomFilter[i];
    }
}

