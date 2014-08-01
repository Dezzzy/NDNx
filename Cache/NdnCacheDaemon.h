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

#ifndef __MIXIM_NDNCACHEDAEMON_H_
#define __MIXIM_NDNCACHEDAEMON_H_


#include <set>
#include <map>

#include <omnetpp.h>
#include "MurmurHash.h"
#include "BaseModule.h"
#include "SpookyHash.h"
#include <bitset>
#include "SimpleAddress.h"

class NdnCacheDaemon : public BaseModule
{
public:
    typedef std::bitset<128> BloomFilterMapping;
    typedef BloomFilterMapping bfm;
    typedef std::map<LAddress::L3Type, int> testingMap;
    typedef std::map<LAddress::L3Type, BloomFilterMapping> NeighbourMap;
    typedef std::pair<LAddress::L3Type, BloomFilterMapping> NeighbourPair;
    typedef NeighbourMap::iterator NeighbourIt;
    typedef std::map<LAddress::L3Type, cMessage*> ControlMap;

    typedef struct CacheInstructionContainer{
        int reqTypes;
        int instructionType;
        int cacheType;
        LAddress::L3Type requesterAddress;

    }ciStore;

    struct Cache{
        char** Buffer;
        int* BloomFilter;
        int* CacheMem;
        simtime_t* TTL;
    };

    struct PendingInterestTable{
        int* RequestType;
        LAddress::L3Type* RequesterAddress;

    };

    struct ContentStore{
        int* CompletedReqeusts;
    };

    struct ForwardingInfoBase{
        NeighbourMap pitOneHopList;
        NeighbourMap pitTwoHopList;
        NeighbourMap pitThreeHopList;
        NeighbourMap csOneHopList;
        NeighbourMap csTwoHopList;
        NeighbourMap csThreeHopList;
        ControlMap removeList;
    };

    enum CACHE_DAEMON_MESSAGE_TYPES{
        CACHE_SCHEDULE_MESSAGE,
        FORWARDING_INFO_TIMER
    };
    enum CACHE_TYPES{
        PIT = 0,
        CS = 1,
        FIB = 2
    };

    enum PIT_REQUEST_TYPES{
        SELF_REQ,
        EXT_REQ,
        SELF_EXT_REQ,
        NO_REQ
    };

    enum FIB_INTERFACE_TYPES{
        ONE_HOP,
        TWO_HOP,
        THREE_HOP,
        CONTROL
    };

    enum  INSTRUCTION_TYPES{
        FIND,
        FIND_UPDATE,
        FIND_RETREIVE,
        FIND_DELETE
    };
    enum REQUEST_STATUS_TYPES{
        REQUEST_FAIL,
        ENTRY_UPDATED,
        ENTRY_INSERTED,
        ENTRY_FOUND,
        ENTRY_DELETED
    };

    enum CACHE_DAEMON_STATUS_TYPES{
        NO_HIT = 81031,
        HIT
    };

    int processRequest(const char* Name, ciStore* Container);
    void generateBloomFilterMap(int CacheType,uint32_t* BloomFilterMap);
    std::bitset<128> convertBloomFilterMap(uint32_t* BloomFilterMap);
    void setInterestCompleted(const char* name);
    int getInterestCompleted(const char* name);
    void updateNeighbourMap(LAddress::L3Type neighbourAddress, int neighbourDist, uint32_t* pitBloomFilterMap, uint32_t* csBloomFilterMap);
    void getMap(int hop, bfm* map1, bfm* map2);

protected:

    int CacheSize;
    int WordSize;
    simtime_t timeToLive;
    Cache** CachePtr;
    PendingInterestTable*  Pit;
    ContentStore* Cs;
    ForwardingInfoBase* Fib;
    NeighbourMap testMap;
    testingMap tMap;
    SpookyHash* spookyHasher;
    MurmurHash* mmHasher;
    cMessage* CacheScheduler;


    virtual void initialize(int stage);
    virtual void handleSelfMsg(cMessage *msg);
    virtual void handleMessage(cMessage *msg);


    // Cache related Functions
    void initializeCacheDaemon();
    void initializePendingInterestTable();
    void initializeContentStore();
    void initializeForwardingInfoBase();
    void setupBasicCache(Cache* cachePtr);
    int cacheEntrySearch(const char* Name, Cache* cachePtr, uint32_t* hashValues);
    int bloomfilterSearch(const char* Name, Cache* cPtr, uint32_t* hashValues);
    void bloomfilterInsert(const char* Name,Cache* cPtr, uint32_t* hValues);
    void getHashValues(const char* name,uint32_t* hValues);
    uint32_t resolveCollision(uint32_t i1, uint32_t i2);

    void updateCacheEntry(int index, Cache* cPtr);
    int insertEntryIntoCache(const char* Name, Cache* cPtr);
    void deleteEntry(int index, Cache* cPtr);
    void setRequestType(int reqType, int index);
    void initializeCacheData();
    void setRequesterAddress(int index, LAddress::L3Type reqAddress);
    BloomFilterMapping updateMapElement(BloomFilterMapping n1, BloomFilterMapping n2);
    void getMapElement(NeighbourMap* map,std::bitset<128>* bloom);
    int searchMap(int hopDist, const char* name);
    void CacheMaintenance();



};

#endif
