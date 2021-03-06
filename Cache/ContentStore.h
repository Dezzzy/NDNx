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

#ifndef __MIXIM_CONTENTSTORE_H_
#define __MIXIM_CONTENTSTORE_H_

#include <omnetpp.h>
#include "BaseModule.h"
#include "SimpleAddress.h"
#include "MurmurHash.h"
#include "HashTable.h"
#include "SpookyHash.h"
#include <set>
#include <map>

class ContentStore : public BaseModule
{
public:
    typedef std::map<const char*, cMessage*> csControlMap;
    enum INSTRUCTION_KINDS{

        INSERT_COMPLETED,

        UPDATE_COMPLETED,
        DATA_FOUND,
        DATA_NOT_FOUND,
        DATA_DELETED,

        NAME_FOUND,
        REQ_COMPLETED

    };

    enum CONTENT_STORE_MESSAGE_KINDS{
        CONTENT_STORE_CONTROL_MESSAGE
    };

    int searchContentStore(const char* name);
    int updateContentStore(const char* name, int nonce);
    int retreiveContentStore(const char* name);
    int deleteFromContentStore(cMessage* msg);
    void getCsBloomFilter(int* newBloomFilter);
    void generateContentStore();

protected:

    char** Buffer;
    int* BloomFilter;
    int* CacheMem;
    simtime_t* TTL;
    int* CompletedRequests;
    int** NonceId;
    csControlMap removeList;
    const static int CacheSize = 128;
    const static int WordSize = 100;
    SpookyHash* spookyHasher;
    MurmurHash* mmHasher;
    HashTable*  hTable;
    simtime_t timeToLive;
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage *msg);
    void initializeContentStore();

    int nonceIdCheck(int nonce, int index);
    void insertNewNonce(int nonce, int index);
    void clearNonceList(int index);

};

#endif
