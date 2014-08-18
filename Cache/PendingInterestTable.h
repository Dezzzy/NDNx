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

#ifndef __MIXIM_PENDINGINTERESTTABLE_H_
#define __MIXIM_PENDINGINTERESTTABLE_H_

#include <omnetpp.h>
#include "BaseModule.h"
#include "MurmurHash.h"
#include "SpookyHash.h"
#include "SimpleAddress.h"
#include "HashTable.h"


class PendingInterestTable : public BaseModule
{
public:

    typedef std::map<const char*, cMessage*> pitControlMap;

    enum REQUEST_KINDS{
        SELF_REQ,
        EXT_REQ,
        DUAL_TYPE_REQ,
        NO_REQ
    };

    enum INSTRUCTION_KINDS{

        INSERT_COMPLETED,

        UPDATE_COMPLETED,
        DATA_FOUND,
        DATA_NOT_FOUND,
        DATA_DELETED,

        NAME_FOUND,


    };

    enum PENDINGI_INTEREST_MESSAGE_TYPES{
        PIT_MAINTENANCE,
        PIT_CONTROL_MESSAGE
    };

    int searchPendingInterestTable(const char* name);
    int updatePendingInterestTable(const char* name, LAddress::L3Type Address, int requestType, LAddress::L3Type iAddress);
    int retreiveEntryFromPIT(const char* name, int* ReqType, LAddress::L3Type* ReqAddress,LAddress::L3Type* iAddress);
    int deleteEntryFromPIT(cMessage* msg);
    int deleteEntryFromPIT(const char* name);
    void updateRequestType(int index, int reqType);
    void getPitBloomFilter(int* newBloomFilter);


protected:

    char** Buffer;
    int* BloomFilter;
    int* CacheMem;
    simtime_t* TTL;
    int* RequestType;
    int* hopCount;
    LAddress::L3Type* RequesterAddress;
    LAddress::L3Type* InterfaceAddress;

    const static int CacheSize = 128;
    const static int WordSize = 100;
    SpookyHash* spookyHasher;
    MurmurHash* mmHasher;
    HashTable* hTable;
    pitControlMap removeList;
    simtime_t timeToLive;
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage *msg);
    void initializePendingInterestTable();


};

#endif
