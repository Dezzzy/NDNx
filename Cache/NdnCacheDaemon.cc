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

#include "NdnCacheDaemon.h"

using std::map;
using std::pair;
using std::make_pair;
using std::endl;

Define_Module(NdnCacheDaemon);

void NdnCacheDaemon::initialize(int stage)
{
    BaseModule::initialize(stage);
    if(stage == 0){
        CacheSize = par("CacheSize");
        WordSize = par("WordSize");
        timeToLive = par("timeToLive");
        CacheScheduler = new cMessage("Cache Scheduler",CACHE_SCHEDULE_MESSAGE);

        CachePtr = new Cache*[3];
        for(int i = 0; i < 3;i++ ){
            CachePtr[i] = new Cache;
        }
        Pit = new PendingInterestTable;
        Cs = new ContentStore;
        Fib = new ForwardingInfoBase;
        spookyHasher = new SpookyHash;
        mmHasher = new MurmurHash;

    }
    if(stage == 1){
        initializeCacheDaemon();
        scheduleAt(simTime() + timeToLive + uniform(0,100),CacheScheduler);
    }
}

void NdnCacheDaemon::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);
    } else{
        opp_error("Cache should not receive messages from other layers, recheck configuration");
    }
}

void NdnCacheDaemon::handleSelfMsg(cMessage* msg)
{
    if(msg->getKind() ==  CACHE_SCHEDULE_MESSAGE){

        cancelEvent(CacheScheduler);
        scheduleAt(simTime() + timeToLive + uniform(0,100),CacheScheduler);
    }
}

void NdnCacheDaemon::initializeCacheDaemon()
{
    for(int i=0;i<3;i++){
        setupBasicCache(CachePtr[i]);
    }

    initializePendingInterestTable();
    initializeContentStore();

    initializeForwardingInfoBase();

}

void NdnCacheDaemon::initializePendingInterestTable()
{

    Pit->RequestType = new int[CacheSize];
    for(int i = 0; i < CacheSize;i++){
        Pit->RequestType[i] = NO_REQ;
    }
    Pit->RequesterAddress = new LAddress::L3Type[CacheSize];

}

void NdnCacheDaemon::initializeContentStore()
{
    Cs->CompletedReqeusts = new int[CacheSize];

}

void NdnCacheDaemon::initializeForwardingInfoBase()
{

}

void NdnCacheDaemon::setupBasicCache(Cache* cachePtr)
{
    cachePtr->Buffer = new char*[CacheSize];
    for(int i = 0; i <  CacheSize; i++ ){
        cachePtr->Buffer[i] = new char[WordSize];

    }
    cachePtr->BloomFilter = new int[CacheSize];
    cachePtr->CacheMem = new int[CacheSize];
    cachePtr->TTL = new simtime_t[CacheSize];
}

int NdnCacheDaemon::processRequest(const char* Name, ciStore* Container)
{
    uint32_t Hash[3];
    uint32_t EntryIndex;
    int requestStatus = 0;
    int CacheType = Container->cacheType;
    int instruction = Container->instructionType;
    EntryIndex = cacheEntrySearch(Name, CachePtr[CacheType], Hash);
    switch(instruction){
    case FIND:// FIND only
        if(EntryIndex != NO_HIT){
            requestStatus = ENTRY_FOUND;
        } else{
            requestStatus = REQUEST_FAIL;
        }
        break;
    case FIND_UPDATE: //FIND and UPDATE/INSERT
        if(EntryIndex != NO_HIT){
            updateCacheEntry(EntryIndex, CachePtr[CacheType]);
            requestStatus = ENTRY_UPDATED;
        } else{
            EntryIndex = insertEntryIntoCache(Name, CachePtr[CacheType]);
            requestStatus = ENTRY_INSERTED;
        }
        if(CacheType == PIT){
            setRequestType(Container->reqTypes, EntryIndex);
            if(requestStatus == ENTRY_INSERTED){
                setRequesterAddress(EntryIndex, Container->requesterAddress);
            }
        }
        break;
    case FIND_RETREIVE://FIND and RETREIVE
        if(EntryIndex != NO_HIT){
            if(CacheType == PIT){
                // get request types
                Container->reqTypes = Pit->RequestType[EntryIndex];
                Container->requesterAddress = Pit->RequesterAddress[EntryIndex];
            }
            if(CacheType == CS){

            }
            if(CacheType == FIB){

            }
            requestStatus = ENTRY_FOUND;
        } else{
            requestStatus = REQUEST_FAIL;
        }
        break;
    case FIND_DELETE: //FIND and DELETE
        if(EntryIndex != NO_HIT){
            deleteEntry(EntryIndex, CachePtr[CacheType]);
            requestStatus = ENTRY_DELETED;
        } else{
            requestStatus = REQUEST_FAIL;
        }
        break;
    }
    return requestStatus;
}

int NdnCacheDaemon::cacheEntrySearch(const char* Name, Cache* cachePtr, uint32_t* hashValues)
{
    int EntryIndex;
    int CurrentSearchIndex;
    int firstIndex, secondIndex;
    int collisionCounter;
    int DataFound;
    // uint32_t hashValues[3];

    if(bloomfilterSearch(Name, cachePtr, hashValues) == HIT){
        CurrentSearchIndex = hashValues[2];
        firstIndex = hashValues[0];
        secondIndex = hashValues[1];
        while(!DataFound && collisionCounter < 6){
            if(cachePtr->CacheMem[CurrentSearchIndex] == 1 && strcmp(Name, cachePtr->Buffer[CurrentSearchIndex]))
                DataFound = 1;
            else {
                collisionCounter++;
                CurrentSearchIndex = resolveCollision(firstIndex, secondIndex);
            }
        }

        if(DataFound)
            EntryIndex = CurrentSearchIndex;
        else
            EntryIndex = NO_HIT;
    } else
        EntryIndex = NO_HIT;

    return EntryIndex;
}

void NdnCacheDaemon::updateCacheEntry(int index, Cache* cPtr)
{
    cPtr->TTL[index] = simTime() + timeToLive;

}

int NdnCacheDaemon::insertEntryIntoCache(const char* Name, Cache* cPtr)
{
    int SpaceFound;
    int collisionCounter = 0;
    uint32_t CurrentSearchIndex;
    uint32_t i1,i2;
    uint32_t k[3];
    // find first available slot
    getHashValues(Name,k);
    CurrentSearchIndex = k[2];
    i1 = k[0];
    i2 = k[1];
    bloomfilterInsert(Name, cPtr, k);
    while(!SpaceFound && collisionCounter < 6){
        if(cPtr->CacheMem[CurrentSearchIndex] == 0){
            strcpy(cPtr->Buffer[CurrentSearchIndex],Name);
            cPtr->CacheMem[CurrentSearchIndex] = 1;
            cPtr->TTL[CurrentSearchIndex] = simTime() + timeToLive;
            SpaceFound = 1;

        } else{
            collisionCounter++;
            CurrentSearchIndex = resolveCollision(i1,i2);
        }
    }
    return CurrentSearchIndex;
}

void NdnCacheDaemon::deleteEntry(int index, Cache* cPtr)
{
    cPtr->CacheMem[index] = 0;
}

int NdnCacheDaemon::bloomfilterSearch(const char* Name, Cache* cPtr,uint32_t* hValues)
{
    getHashValues(Name,hValues);
    if(cPtr->BloomFilter[hValues[0]] && cPtr->BloomFilter[hValues[1]]){
        EV << "Record found" << endl;
        return HIT;

    } else{
        EV << "Record not found" << endl;
        return NO_HIT;
    }
}
void NdnCacheDaemon::bloomfilterInsert(const char* Name,Cache* cPtr, uint32_t* hValues)
{
    getHashValues(Name, hValues);
    cPtr->BloomFilter[hValues[0]] = 1;
    cPtr->BloomFilter[hValues[1]] = 1;

}
void NdnCacheDaemon::getHashValues(const char* name,uint32_t* hValues)
{
    int len = strlen(name);
    hValues[0] = mmHasher->MurmurHash3(name,len,1)%CacheSize;
    hValues[1] = spookyHasher->SpookHash(name,len,1)%CacheSize;
    hValues[2] = hValues[0];
}

void NdnCacheDaemon::setRequestType(int reqType, int index)
{
    if(Pit->RequestType[index] == NO_REQ){
        Pit->RequestType[index] = reqType;

    } else if(Pit->RequestType[index] != reqType) {
        Pit->RequestType[index] = SELF_EXT_REQ;
    }
}

uint32_t NdnCacheDaemon::resolveCollision(uint32_t i1, uint32_t i2)
{
    i1 = i1 + i2;
    if(i1 > CacheSize){
        i1 = i1 % CacheSize;
    }
    return i1;
}

void NdnCacheDaemon::generateBloomFilterMap(int CacheType,uint32_t* BloomFilterMap)
{
    std::bitset<32> i1;
    std::bitset<32> i2;
    std::bitset<32> i3;
    std::bitset<32> i4;
    Cache* cPtr = new Cache;
    cPtr = CachePtr[CacheType];

    for(int i = 0; i < 32;i++){
        if(cPtr->BloomFilter[i]){
            i1[i] = 1;
        } else{
            i1[i] = 0;
        }
        if(cPtr->BloomFilter[i+32]){
            i2[i+32] = 1;
        } else{
            i2[i+32] = 0;
        }
        if(cPtr->BloomFilter[i+64]){
            i3[i+64] = 1;
        } else{
            i3[i+64] = 0;
        }
        if(cPtr->BloomFilter[i+96]){
            i4[i+96] = 1;
        } else{
            i4[i+96] = 0;
        }

    }
    BloomFilterMap[0] = i1.to_ulong();
    BloomFilterMap[1] = i2.to_ulong();
    BloomFilterMap[2] = i3.to_ulong();
    BloomFilterMap[3] = i4.to_ulong();

}

std::bitset<128> NdnCacheDaemon::convertBloomFilterMap(uint32_t* BloomFilterMap)
{
    std::bitset<32> i1(BloomFilterMap[0]);
    std::bitset<32> i2(BloomFilterMap[1]);
    std::bitset<32> i3(BloomFilterMap[2]);
    std::bitset<32> i4(BloomFilterMap[3]);
    std::bitset<128> i5;

    return i5;
}

void NdnCacheDaemon::setInterestCompleted(const char* name)
{
    uint32_t hashValues[3];
    int index;
    index = cacheEntrySearch(name, CachePtr[CS],hashValues);
    Cs->CompletedReqeusts[index] = 1;
}

int NdnCacheDaemon::getInterestCompleted(const char* name)
{
    uint32_t hashValues[3];
    int index;
    int isInterestCompleted;

    index = cacheEntrySearch(name, CachePtr[CS],hashValues);
    isInterestCompleted = Cs->CompletedReqeusts[index];
    return isInterestCompleted;
}

void NdnCacheDaemon::setRequesterAddress(int index, LAddress::L3Type reqAddress)
{
    Pit->RequesterAddress[index] = reqAddress;
}

void NdnCacheDaemon::updateNeighbourMap(LAddress::L3Type neighbourAddress, int neighbourDist, uint32_t* pitBloomFilterMap, uint32_t* csBloomFilterMap)
{
    std::bitset<128> pitBloomFilter;
    std::bitset<128> csBloomFilter;
    std::bitset<128> tempBloomFilter;

    NeighbourMap::key_type nodeAddress = neighbourAddress;
    NeighbourMap::mapped_type currentBloomFilter;
    BloomFilterMapping newBloomFilter;
    NeighbourMap::iterator testIt;
    testingMap::iterator tIt;
    NeighbourMap::iterator pitIt;
    NeighbourMap::iterator csIt;

    pitBloomFilter = convertBloomFilterMap(pitBloomFilterMap);
    csBloomFilter = convertBloomFilterMap(csBloomFilterMap);


    switch(neighbourDist){
    case ONE_HOP:
        pitIt = Fib->pitOneHopList.find(nodeAddress);
        csIt  = Fib->csOneHopList.find(nodeAddress);
        if(pitIt == Fib->pitOneHopList.end()){
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitBloomFilter);
            Fib->pitOneHopList.insert(pitMapPair);
        }else{
            currentBloomFilter = Fib->pitOneHopList[nodeAddress];
            newBloomFilter = updateMapElement(currentBloomFilter, pitBloomFilter);
            Fib->pitOneHopList[nodeAddress] = newBloomFilter;
        }
        if(csIt == Fib->csOneHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress, csBloomFilter);
            Fib->csOneHopList.insert(csMapPair);
        }else{
            currentBloomFilter = Fib->csOneHopList[nodeAddress];
            newBloomFilter = updateMapElement(currentBloomFilter, csBloomFilter);
            Fib->csOneHopList[nodeAddress] = newBloomFilter;
        }

        break;

    case TWO_HOP:
        pitIt = Fib->pitTwoHopList.find(nodeAddress);
        csIt = Fib->csTwoHopList.find(nodeAddress);


        if(pitIt == Fib->pitTwoHopList.end()){
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitBloomFilter);
            Fib->pitTwoHopList.insert(pitMapPair);
        } else{
            currentBloomFilter = Fib->pitThreeHopList[nodeAddress];
            newBloomFilter = updateMapElement(currentBloomFilter, pitBloomFilter);
            Fib->pitTwoHopList[nodeAddress] = newBloomFilter;
        }
        if(csIt == Fib->csTwoHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress, csBloomFilter);
            Fib->csTwoHopList.insert(csMapPair);
        } else{
            currentBloomFilter =Fib->csTwoHopList[nodeAddress];
            newBloomFilter = updateMapElement(currentBloomFilter,csBloomFilter);
            Fib->csTwoHopList[nodeAddress] = newBloomFilter;
        }

        break;

    case THREE_HOP:
        pitIt = Fib->pitThreeHopList.find(nodeAddress);
        csIt = Fib->csThreeHopList.find(nodeAddress);

        if(pitIt == Fib->pitThreeHopList.end()){
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitBloomFilter);
            Fib->pitThreeHopList.insert(pitMapPair);
        }else{
            currentBloomFilter = Fib->pitThreeHopList[nodeAddress];
            newBloomFilter = updateMapElement(currentBloomFilter, pitBloomFilter);
            Fib->pitThreeHopList[nodeAddress] = newBloomFilter;
        }
        if(csIt == Fib->csThreeHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress, csBloomFilter);

            Fib->csThreeHopList.insert(csMapPair);
        }else{
            currentBloomFilter = Fib->csThreeHopList[nodeAddress];
            newBloomFilter = updateMapElement(currentBloomFilter, csBloomFilter);
            Fib->csThreeHopList[nodeAddress] = newBloomFilter;
        }

        break;

    case CONTROL:
        ControlMap::key_type nodeAddress = neighbourAddress;
        ControlMap::iterator its = Fib->removeList.find(nodeAddress);

        if (its == Fib->removeList.end()) {
            EV << "updateControlMap(): The message came from a new neighbor! " << endl;

            // insert key value pair <node address, event> in neighborhood map.
            cMessage *removeEvent = new cMessage("removeEvent", FORWARDING_INFO_TIMER);

            // schedule the event to remove the entry after initT seconds
            scheduleAt(simTime()+(timeToLive*2), removeEvent);

            ControlMap::value_type pairToInsert = make_pair(nodeAddress, removeEvent);
            pair<ControlMap::iterator, bool> ret = Fib->removeList.insert(pairToInsert);

            // set the context pointer to point to the integer that resembles to the address of
            // the node to be removed when the corresponding event occurs
            (ret.first)->second->setContextPointer((void*)(&(ret.first)->first));


        }
        //if the node is NOT a "new" neighbor update its timer
        else {
            EV << "updateControlMap(): The message came from an already known neighbor! " << endl;
            //cancel the event that was scheduled to remove the entry for this neighbor
            cancelEvent(its->second);
            // Define a new event in order to remove the entry after initT seconds
            // Set the context pointer to point to the integer that resembles to the address of
            // the node to be removed when the corresponding event occurs
            its->second->setContextPointer((void*)(&its->first));
            its->first;

            scheduleAt(simTime()+(timeToLive*2), its->second);
        }
    }
}

std::bitset<128> NdnCacheDaemon::updateMapElement(BloomFilterMapping n1, BloomFilterMapping n2)
{
    BloomFilterMapping n3;
    for(int i = 0;i < 128 ;i++){
        n3[i] = n1[i] & n2[i];
    }

    return n3;
}

void NdnCacheDaemon::getMap(int hop, bfm* map1, bfm* map2)
{
    NeighbourMap* mapPtr;
    std::bitset<128> pitBloom;
    std::bitset<128> csBloom;
    mapPtr = new NeighbourMap;


    switch(hop){
    case ONE_HOP:
        *mapPtr = Fib->pitOneHopList;
        getMapElement(mapPtr, &pitBloom);
        *mapPtr = Fib->csOneHopList;
        getMapElement(mapPtr, &csBloom);
        break;
    case TWO_HOP:
        *mapPtr = Fib->pitTwoHopList;
        getMapElement(mapPtr, &pitBloom);
        *mapPtr = Fib->csTwoHopList;
        getMapElement(mapPtr, &csBloom);
        break;
    case THREE_HOP:
        *mapPtr = Fib->pitThreeHopList;
        getMapElement(mapPtr, &pitBloom);
        *mapPtr = Fib->csThreeHopList;
        getMapElement(mapPtr, &csBloom);
        break;
    }

    *map1 = pitBloom;
    *map2 = csBloom;

}
void NdnCacheDaemon::getMapElement(NeighbourMap* map, std::bitset<128>* bloom)
{
    std::bitset<128> holder;
    std::bitset<128> temp;
    NeighbourMap::iterator it;
    int mapSize = map->size();
    if(mapSize > 0){
        for(it = map->begin();it != map->end();++it){
            temp = it->second;
            it->second;
            holder = holder&temp;
        }
    }else{
        bloom->reset();
    }

    *bloom = holder;
}

int NdnCacheDaemon::searchMap(int hopDist, const char* name)
{
    uint32_t hash[3];
    std::bitset<128> searchedFilter;
    NeighbourMap* mapPtr;
    int searchDist;
    mapPtr = new NeighbourMap;

    switch(hopDist){
    case ONE_HOP:
        *mapPtr = Fib->csOneHopList;
        break;
    case TWO_HOP:
        *mapPtr = Fib->csTwoHopList;
        break;
    case THREE_HOP:
        *mapPtr = Fib->csThreeHopList;
        break;
    }

    //getMap(mapPtr, &searchedFilter);
    getHashValues(name, hash);
    if(searchedFilter[hash[0]] == 1 && searchedFilter[hash[1]] == 1){
        searchDist = hopDist;
    }
    return searchDist;
}




void NdnCacheDaemon::CacheMaintenance()

{

}
