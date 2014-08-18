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

#ifndef __MIXIM_FORWARDINGINFOBASE_H_
#define __MIXIM_FORWARDINGINFOBASE_H_

#include <omnetpp.h>
#include "BaseModule.h"
#include "SimpleAddress.h"
#include "HashTable.h"
#include <bitset>
#include <set>
#include <map>

class ForwardingInfoBase : public BaseModule
{
public:
    typedef std::bitset<128> BloomFilterMap;
    typedef std::map<LAddress::L3Type, BloomFilterMap> NeighbourMap;
    typedef std::map<LAddress::L3Type, cMessage*> ControlMap;
    typedef std::multimap<LAddress::L3Type,cMessage*>NameControlMap;
    typedef std::map<const char*, cMessage*> NameMap;

    enum FORWARDING_INFO_BASE_HOP_TYPES{
        ONE_HOP,
        TWO_HOP,
        THREE_HOP,
        X_HOP,
        CONTROL
    };

    enum INSTRUCTION_KINDS{
        INSERT_NAME,
        INSERT_COMPLETED,
        UPDATE_NAME,
        UPDATE_COMPLETED,
        DATA_FOUND,
        DATA_NOT_FOUND,
        DATA_DELETED,
        NAME_NOT_FOUND,
        NAME_FOUND,
        NAME_DELETED,
        SEARCH_PIT_LIST,
        SEARCH_CS_LIST,INTERFACE_DELETED

    };

    enum CACHE_DAEMON_MESSAGE_TYPES{
        START_MESSAGE,
        FORWARDING_INFO_TIMER,
        NAME_INTERFACE_CONTROL_MESSAGE
    };

    int searchNameInterfaceList(const char* name);
    void updateNameInterfaceList(const char* name, int hCount, LAddress::L3Type iAddress);
    void deleteEntryFromNameInterfaceList(cMessage* msg);
    int retreiveEntryFromNameInterfaceList(const char* name, int* returnHopCount, LAddress::L3Type* returnIAddress);

    void updateHopMap(LAddress::L3Type hopAddress, int* pitBloomFilter, int* csBloomFilter,int hopDistance);
    void getHopMap(int hopDistance, int* pitBloomFilter, int* csBloomFilter);
    void searchMap(const char* name, LAddress::L3Type** hopMap, int neighbourList, int* addressLimit);
    void removeMapFromFib(cMessage* msg);

protected:
    const static int CacheSize = 128;
    const static int WordSize = 100;
    SpookyHash* spookyHasher;
    MurmurHash* mmHasher;
    HashTable* hTable;
    simtime_t timeToLive;

    // name interface
    char** Buffer;
    int* BloomFilter;
    int* CacheMemory;
    int** HopCount;
    int* noOfInterfaces;
    int** TTL;
    LAddress::L3Type** InterfaceAddress;
    int InterfaceSize;

    // Name Space Fields
    NeighbourMap pitOneHopList;
    NeighbourMap pitTwoHopList;
    NeighbourMap pitThreeHopList;
    NeighbourMap csOneHopList;
    NeighbourMap csTwoHopList;
    NeighbourMap csThreeHopList;
    NeighbourMap pitXHopList;
    NeighbourMap csXHopList;
    ControlMap removeList;
    NameControlMap nameInterfaceRemoveList;

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage *msg);
    void initializeForwardingInfoBase();


};

#endif
