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

#include "PendingInterestTable.h"

Define_Module(PendingInterestTable);

using std::map;
using std::pair;
using std::make_pair;
using std::endl;

void PendingInterestTable::initialize(int stage)
{
    BaseModule::initialize(stage);
    if(stage == 0){
        spookyHasher = new SpookyHash;
        mmHasher = new MurmurHash;
        hTable = new HashTable;
        timeToLive = par("timeToLive");
    }
    if(stage == 1){
        initializePendingInterestTable();
    }
}

void PendingInterestTable::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);
    }
}

void PendingInterestTable::handleSelfMsg(cMessage *msg)
{
    switch(msg->getKind()){
    case PIT_MAINTENANCE:
        deleteEntryFromPIT(msg);
        break;
    case PIT_CONTROL_MESSAGE:
        deleteEntryFromPIT(msg);
        break;

    }
}

void PendingInterestTable::initializePendingInterestTable()
{
    Buffer = new char*[CacheSize];
    for(int i = 0; i < CacheSize; i++){
        Buffer[i] = new char[WordSize];
    }
    BloomFilter = new int[CacheSize];
    CacheMem = new int[CacheSize];
    RequestType = new int[CacheSize];
    TTL = new simtime_t[CacheSize];
    RequesterAddress = new LAddress::L3Type[CacheSize];
    hopCount = new int[CacheSize];
    InterfaceAddress = new LAddress::L3Type[CacheSize];
    for(int i = 0;i< CacheSize;i++){
        RequesterAddress[i] = LAddress::L3NULL;
        BloomFilter[i] = 0;
        CacheMem[i] = 0;
        hopCount[i] = 0;
        InterfaceAddress[i] = LAddress::L3NULL;
        RequestType[i] = NO_REQ;
    }
}

int PendingInterestTable::searchPendingInterestTable(const char* name)
{
    Enter_Method_Silent("searchPendingInterestTable(const char* name)");
    int cacheIndex;
    cacheIndex = hTable->searchNameCache(name,  Buffer, CacheMem, BloomFilter);
    return cacheIndex;
}

int PendingInterestTable::updatePendingInterestTable(const char* name, LAddress::L3Type Address, int requestType, LAddress::L3Type iAddress)
{
    Enter_Method_Silent("updatePendingInterestTable(const char* name, LAddress::L3Type Address, int requestType, LAddress::L3Type iAddress)");
    int cacheIndex;
    int updateType;
    int instructionStatus;
    EV<< "starting pit update" << endl;
    updateType = hTable->updateNameCache(name, Buffer,CacheMem,BloomFilter,&cacheIndex);
    EV<< "pit name update finished" << endl;
    EV << cacheIndex << endl;


    if(updateType == hTable->INSERT_NAME){

        strcpy(Buffer[cacheIndex],name);
        RequesterAddress[cacheIndex] = Address;
        if(InterfaceAddress[cacheIndex] == LAddress::L3NULL){
            InterfaceAddress[cacheIndex] = iAddress;
        }
        TTL[cacheIndex] = simTime() + timeToLive;
        updateRequestType(cacheIndex, requestType);
        cMessage* removeInterest = new cMessage(name, PIT_CONTROL_MESSAGE);
        scheduleAt(simTime() + timeToLive, removeInterest );
        pitControlMap::key_type Key = name;
        pitControlMap::value_type pitPair = make_pair(name, removeInterest);
        pair<pitControlMap::iterator, bool> ret = removeList.insert(pitPair);
        (ret.first)->second->setContextPointer((void*)(&(ret.first)->first));
        instructionStatus = INSERT_COMPLETED;
    } else if(updateType == hTable->UPDATE_NAME){

        TTL[cacheIndex] = simTime() + timeToLive;
        updateRequestType(cacheIndex, requestType);
        instructionStatus = UPDATE_COMPLETED;
        pitControlMap::key_type Key = name;
        pitControlMap::iterator it = removeList.find(name);
        if(it != removeList.end()){
            cancelEvent(it->second);
            it->second->setContextPointer((void*)(&it->first));
            scheduleAt(simTime()+(timeToLive*2), it->second);
        }
    }

    return instructionStatus;
}

int PendingInterestTable::retreiveEntryFromPIT(const char* name, int* ReqType, LAddress::L3Type* ReqAddress,LAddress::L3Type* iAddress)
{
    Enter_Method_Silent("retreiveEntryFromPIT(const char* name, int* ReqType, LAddress::L3Type* ReqAddress,LAddress::L3Type* interfaceAddress)");
    int cacheIndex;
    int instructionStatus;
    cacheIndex = searchPendingInterestTable(name);
    if(cacheIndex != (CacheSize + 1)){
        *ReqType = RequestType[cacheIndex];
        *ReqAddress = RequesterAddress[cacheIndex];
        *iAddress = InterfaceAddress[cacheIndex];
        instructionStatus = DATA_FOUND;
    } else{
        instructionStatus = DATA_NOT_FOUND;
    }

    return instructionStatus;
}

int PendingInterestTable::deleteEntryFromPIT(cMessage* msg)
{
    int instructionStatus;
    int cacheIndex;
    instructionStatus = hTable->deleteFromNameCache(msg->getName(),  Buffer,  CacheMem,  BloomFilter, &cacheIndex);
    if(instructionStatus == hTable->NAME_DELETED){
        const pitControlMap::key_type& name = *static_cast<pitControlMap::key_type*>( msg->getContextPointer() );
        pitControlMap::iterator it = removeList.find(name);

        cancelAndDelete(removeList.find(it->first)->second);
        removeList.erase(it);
        updateRequestType(cacheIndex, NO_REQ);
        TTL[cacheIndex] = 0;
        RequesterAddress[cacheIndex] = LAddress::L3NULL;
        InterfaceAddress[cacheIndex] = LAddress::L3NULL;
    } else{
        opp_error("name could not be found in Pending Interest Table");
    }

    if(CacheMem[cacheIndex] == 0){
        EV<<"Pit delete works"<<endl;
    }
}

int PendingInterestTable::deleteEntryFromPIT(const char* name)
{
    Enter_Method_Silent("deleteEntryFromPIT(const char* name)");
    int instructionStatus;
    int cacheIndex;

    instructionStatus = hTable->deleteFromNameCache(name, Buffer, CacheMem,BloomFilter, &cacheIndex);

    if(instructionStatus == hTable->NAME_DELETED){
        const pitControlMap::key_type& keyName = name;
        pitControlMap::iterator it = removeList.find(keyName);

        cancelAndDelete(removeList.find(it->first)->second);
        removeList.erase(it);
        updateRequestType(cacheIndex, NO_REQ);
        TTL[cacheIndex] = 0;
        RequesterAddress[cacheIndex] = LAddress::L3NULL;
        InterfaceAddress[cacheIndex] = LAddress::L3NULL;
    } else{
        opp_error("name could not be found in Pending Interest Table");
    }

    return DATA_DELETED;
}

void PendingInterestTable::updateRequestType(int cacheIndex, int reqType)
{
    if(RequestType[cacheIndex] == NO_REQ){
        EV<<"changing from NO_REQ to new type"<<endl;
        RequestType[cacheIndex] = reqType;
    } else if(reqType == NO_REQ){
        EV<<"changing to NO_REQ"<<endl;
        RequestType[cacheIndex] = reqType;
    } else if(RequestType[cacheIndex] != reqType){
        EV<<"changing to DUAL_REQ"<<endl;
        RequestType[cacheIndex] = DUAL_TYPE_REQ;
    } else{
        EV<<"no request type change"<<endl;
    }

}

void PendingInterestTable::getPitBloomFilter(int* newBloomFilter)
{
    Enter_Method_Silent("getPitBloomFilter(int* newBloomFilter)");
    hTable->getBloomFilter(BloomFilter, newBloomFilter);
}





