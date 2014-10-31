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

#include "ContentStore.h"

Define_Module(ContentStore);

using std::map;
using std::pair;
using std::make_pair;
using std::endl;

void ContentStore::initialize(int stage)
{
    BaseModule::initialize(stage);
    if(stage == 0){
        spookyHasher = new SpookyHash;
        mmHasher = new MurmurHash;
        hTable = new HashTable;
        timeToLive = par("timeToLive");
    }

    if(stage == 1){
        initializeContentStore();
    }
}

void ContentStore::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);
    }
}

void ContentStore::handleSelfMsg(cMessage *msg)
{
    switch(msg->getKind()){
    case CONTENT_STORE_CONTROL_MESSAGE:
        deleteFromContentStore(msg);
        break;
    }
}

void ContentStore::initializeContentStore()
{
    Buffer = new char*[CacheSize];
    for(int i = 0; i< CacheSize;i++){
        Buffer[i] = new char[WordSize];
    }
    BloomFilter = new int[CacheSize];
    CacheMem = new int[CacheSize];
    CompletedRequests = new int[CacheSize];
    TTL = new simtime_t[CacheSize];
    generateContentStore();
}

int ContentStore::searchContentStore(const char* name)
{
    Enter_Method_Silent("searchContentStore(const char* name)");
    int cacheIndex;
    cacheIndex = hTable->searchNameCache(name, Buffer,CacheMem,BloomFilter);
    return cacheIndex;
}

int ContentStore::updateContentStore(const char* name)
{
    Enter_Method_Silent("updateContentStore(const char* name)");
    int csIndex;
    int updateStatus;
    int instructionStatus;
    updateStatus = hTable->updateNameCache(name, Buffer,CacheMem,BloomFilter,&csIndex);
    if(updateStatus == hTable->INSERT_NAME){
        strcpy(Buffer[csIndex],name);
        TTL[csIndex] = timeToLive;
        instructionStatus = INSERT_COMPLETED;
        cMessage* removeData = new cMessage(name, CONTENT_STORE_CONTROL_MESSAGE);
        scheduleAt(simTime() + timeToLive, removeData);
        csControlMap::key_type hashKey = name;
        csControlMap::value_type controlPair = make_pair(hashKey, removeData);
        pair<csControlMap::iterator, bool> ret = removeList.insert(controlPair);
        (ret.first)->second->setContextPointer((void*)(&(ret.first)->first));
    } else if(updateStatus == hTable->UPDATE_NAME){
        TTL[csIndex] = timeToLive;
        instructionStatus = UPDATE_COMPLETED;
        csControlMap::key_type hashKey = name;
        csControlMap::iterator it = removeList.find(hashKey);
        if(it != removeList.end()){
            cancelEvent(it->second);
            it->second->setContextPointer((void*)(&it->first));
            scheduleAt(simTime()+(timeToLive*2), it->second);
        }
    }

    return instructionStatus;
}

int ContentStore::retreiveContentStore(const char* name)
{
    Enter_Method_Silent("retreiveContentStore(const char* name)");
    int csIndex;
    int instructionStatus;

    csIndex = searchContentStore(name);

    if(csIndex == (CacheSize+1)){
        instructionStatus = DATA_NOT_FOUND;
    } else{
        if(CompletedRequests[csIndex] == 1){
            instructionStatus = REQ_COMPLETED;
        } else{
            instructionStatus = DATA_FOUND;
            CompletedRequests[csIndex] = 1;
            EV<<"this word was found:    "<< name << endl;
        }
    }

    return instructionStatus;
}

int ContentStore::deleteFromContentStore(cMessage* msg)
{

    int csIndex;
    int instructionStatus;

    const csControlMap::key_type& name = *static_cast<csControlMap::key_type*>( msg->getContextPointer() );
    csControlMap::iterator it = removeList.find(name);

    cancelAndDelete(removeList.find(it->first)->second);
    removeList.erase(it);

    csIndex = searchContentStore(msg->getName());
    if(csIndex == (CacheSize+1)){
        instructionStatus = DATA_NOT_FOUND;
    } else{
        instructionStatus = DATA_DELETED;
        CacheMem[csIndex] = 0;
    }

    EV<<"delete complete, Cs"<<endl;
    return instructionStatus;
}

void ContentStore::getCsBloomFilter(int* newBloomFilter)
{
    Enter_Method_Silent("getCsBloomFilter(int* newBloomFilter)");
    hTable->getBloomFilter(BloomFilter, newBloomFilter);
}

void ContentStore::generateContentStore()
{
    updateContentStore("car");
    updateContentStore("speed");
    updateContentStore("location");
    updateContentStore("density");
    updateContentStore("average");
    updateContentStore("Cape Town");
    updateContentStore("medium");
    updateContentStore("nissan");
}


