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

#include "ForwardingInfoBase.h"

using std::map;
using std::pair;
using std::make_pair;
using std::endl;

Define_Module(ForwardingInfoBase);

void ForwardingInfoBase::initialize(int stage)
{
    BaseModule::initialize(stage);

    if(stage == 0){
        spookyHasher = new SpookyHash;
        mmHasher = new MurmurHash;
        hTable = new HashTable;
        timeToLive = par("timeToLive");
        InterfaceSize = 128;
    }

    if(stage == 1){
        initializeForwardingInfoBase();
    }
}

void ForwardingInfoBase::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);
    }
}

void ForwardingInfoBase::handleSelfMsg(cMessage *msg)
{
    switch(msg->getKind()){
    case NAME_INTERFACE_CONTROL_MESSAGE:
        deleteEntryFromNameInterfaceList(msg);
        //cancelAndDelete(msg);
        break;
    case FORWARDING_INFO_TIMER:
        removeMapFromFib(msg);
        break;
    }
}

void ForwardingInfoBase::initializeForwardingInfoBase()
{
    NeighbourMap::key_type nodeAddress = LAddress::L3NULL;
    std::bitset<128> emptyBloomFilter;
    emptyBloomFilter.reset();
    NeighbourMap::value_type emptyPair = make_pair(nodeAddress, emptyBloomFilter);

    // initialize one hop list
    pitOneHopList.insert(emptyPair);
    csOneHopList.insert(emptyPair);
    pitTwoHopList.insert(emptyPair);
    csTwoHopList.insert(emptyPair);
    pitThreeHopList.insert(emptyPair);
    csThreeHopList.insert(emptyPair);
    pitXHopList.insert(emptyPair);
    csXHopList.insert(emptyPair);

    // Name interface initialize
    Buffer = new char*[InterfaceSize];
    for(int i = 0; i <  InterfaceSize; i++){
        Buffer[i] = new char[WordSize];
    }
    CacheMemory = new int[InterfaceSize];
    BloomFilter = new int[InterfaceSize];
    HopCount = new int*[InterfaceSize];
    TTL = new int*[InterfaceSize];
    noOfInterfaces = new int[InterfaceSize];
    InterfaceAddress = new LAddress::L3Type*[InterfaceSize];
    for(int j = 0;j < InterfaceSize; j++){
        HopCount[j] = new int[10];
        InterfaceAddress[j] = new LAddress::L3Type[10];
        TTL[j] = new int[10];
    }
}

int ForwardingInfoBase::searchNameInterfaceList(const char* name)
{
    Enter_Method_Silent("searchNameInterfaceList(const char* name)");
    int instructionStatus;
    instructionStatus = hTable->searchNameCache(name, Buffer, CacheMemory, BloomFilter);
    return instructionStatus;
}

void ForwardingInfoBase::updateNameInterfaceList(const char* name, int hCount, LAddress::L3Type iAddress)
{
    Enter_Method_Silent("updateNameInterfaceList(const char* name, int hCount, LAddress::L3Type iAddress)");
    int instructionStatus;
    int collisionCounter;
    int fibInstructionStatus;
    int SpaceFound;
    int cacheIndex;
    int InterfaceEntrySize;

    instructionStatus = hTable->updateNameCache(name, Buffer, CacheMemory, BloomFilter,&cacheIndex);

    if(instructionStatus ==  hTable->INSERT_NAME){
        EV<<"name being inserted into Name Interface list"<<endl;
        HopCount[cacheIndex][0] = hCount;
        InterfaceAddress[cacheIndex][0] = iAddress;
        cMessage* removeName = new cMessage(name, NAME_INTERFACE_CONTROL_MESSAGE);
        scheduleAt(simTime() + timeToLive, removeName);
        ControlMap::key_type nodeAddress = iAddress;
        ControlMap::value_type pairToInsert = make_pair(nodeAddress, removeName);
        pair<ControlMap::iterator, bool> ret = nameInterfaceRemoveList.insert(pairToInsert);

        // set the context pointer to point to the integer that resembles to the address of
        // the node to be removed when the corresponding event occurs
        (ret.first)->second->setContextPointer((void*)(&(ret.first)->first));
        noOfInterfaces[cacheIndex]++;
        EV<<"name interface list insertion complete"<<endl;
    } else if(instructionStatus == hTable->UPDATE_NAME){
        InterfaceEntrySize = noOfInterfaces[cacheIndex];
        int interfaceFound = 0;
        int counter = 0;
        while(!interfaceFound && counter < InterfaceEntrySize ){
            if(iAddress == InterfaceAddress[cacheIndex][counter]){
                interfaceFound = 1;
            } else{
                counter++;
            }
        }
        if(interfaceFound){
            HopCount[cacheIndex][counter] = hCount;
            ControlMap::key_type nodeAddress = iAddress;
            ControlMap::iterator it = nameInterfaceRemoveList.find(nodeAddress);
            if(it != nameInterfaceRemoveList.end()){
                cancelEvent(it->second);
                it->second->setContextPointer((void*)(&it->first));
                scheduleAt(simTime()+(timeToLive*2), it->second);
            }
        }  else if(!interfaceFound){
            HopCount[cacheIndex][counter] = hCount;
            InterfaceAddress[cacheIndex][counter] = iAddress;
            cMessage* removeName = new cMessage(name, NAME_INTERFACE_CONTROL_MESSAGE);
            scheduleAt(simTime() + timeToLive, removeName);
            ControlMap::key_type nodeAddress = iAddress;
            ControlMap::value_type pairToInsert = make_pair(nodeAddress, removeName);
            pair<ControlMap::iterator, bool> ret = nameInterfaceRemoveList.insert(pairToInsert);

            // set the context pointer to point to the integer that resembles to the address of
            // the node to be removed when the corresponding event occurs
            (ret.first)->second->setContextPointer((void*)(&(ret.first)->first));
            noOfInterfaces[cacheIndex]++;

        }
    }


}

void ForwardingInfoBase::deleteEntryFromNameInterfaceList(cMessage* msg)
{
    int CacheIndex;
    int  instructionStatus;
    LAddress::L3Type iAddress;

    EV<<"starting delete of name interface list entry"<<endl;
    const ControlMap::key_type& node = *static_cast<ControlMap::key_type*>( msg->getContextPointer() );
    EV << "handleSelfMsg(): Remove node "<< node <<" from NeighMap!" << endl;
    ControlMap::iterator it = nameInterfaceRemoveList.find(node);

    cancelAndDelete(msg);
    nameInterfaceRemoveList.erase(it);


    iAddress = node;

    CacheIndex = hTable->searchNameCache(msg->getName(), Buffer, CacheMemory, BloomFilter);
    if(CacheIndex == (CacheSize + 1)){
        instructionStatus = NAME_NOT_FOUND;
    } else{
        int counter = 0;
        int interfaceFound = 0;
        int iSize = noOfInterfaces[CacheIndex];
        while(counter < iSize && !interfaceFound){
            if(InterfaceAddress[CacheIndex][counter] == iAddress){
                interfaceFound = 1;
            } else{
                counter++;
            }
        }
        if(interfaceFound){
            InterfaceAddress[CacheIndex][counter] = LAddress::L3NULL;
            HopCount[CacheIndex][counter] = 0;
            noOfInterfaces[CacheIndex]--;
            instructionStatus = NAME_DELETED;
        } else{
            instructionStatus = NAME_NOT_FOUND;
        }

        if(noOfInterfaces[CacheIndex] == 0){
            // delete entire interface
            CacheMemory[CacheIndex] = 0;
            instructionStatus = INTERFACE_DELETED;
        } else{

        }

    }

    EV<<"end of name interface list entry delete"<<endl;
}

int ForwardingInfoBase::retreiveEntryFromNameInterfaceList(const char* name, int* returnHopCount, LAddress::L3Type* returnIAddress)
{
    Enter_Method_Silent("retreiveEntryFromNameInterfaceList(const char* name, int* returnHopCount, LAddress::L3Type* returnIAddress)");
    int CacheIndex;
    int instructionStatus;
    uint32_t hashValues[3];
    hTable->getHashValues(name, hashValues);
    int iSize;
    int lowestInterface;
    CacheIndex = searchNameInterfaceList(name);
    if(CacheIndex != (CacheSize + 1)){
        instructionStatus = NAME_FOUND;
        iSize = noOfInterfaces[CacheIndex];
        for(int i = 0; i <  iSize;i++){
            if(i == 0){
                lowestInterface = i;
            } else{
                if(lowestInterface > InterfaceAddress[CacheIndex][i]){
                    lowestInterface = i;
                }
            }
        }
        *returnHopCount = HopCount[CacheIndex][lowestInterface];
        *returnIAddress = InterfaceAddress[CacheIndex][lowestInterface];
    } else{
        instructionStatus = NAME_NOT_FOUND;
    }



    return instructionStatus;
}

void ForwardingInfoBase::updateHopMap(LAddress::L3Type hopAddress, int* pitBloomFilter, int* csBloomFilter,int hopDistance)
{
    Enter_Method_Silent("updateHopMap(LAddress::L3Type hopAddress, int* pitBloomFilter, int* csBloomFilter,int hopDistance)");
    std::bitset<128> pitMap;
    std::bitset<128> csMap;
    std::bitset<128> newMap;

    NeighbourMap::iterator pitIt;
    NeighbourMap::iterator csIt;
    NeighbourMap::key_type nodeAddress = hopAddress;
    //NeighbourMap::value_type pitMapPair;
    //NeighbourMap::value_type csMapPair;

    // Convert BloomFilter from array to bitset for easier manipulation
    for(int i = 0; i < 128; i++){
        pitMap[i] = pitBloomFilter[i];
        csMap[i] = csBloomFilter[i];
    }

    std::bitset<32> uLong1 (0);
    std::bitset<32> uLong2 (0);
    std::bitset<32> uLong3 (0);
    std::bitset<32> uLong4 (0);
    uint32_t ulong[4];

    for(int k = 0; k< 32 ;k++){
        uLong1[k] = pitMap[k];
        uLong2[k] = pitMap[k+32];
        uLong3[k] = pitMap[k+64];
        uLong4[k] = pitMap[k+96];
    }

    ulong[0] = uLong1.to_ulong();
    ulong[1] = uLong2.to_ulong();
    ulong[2] = uLong3.to_ulong();
    ulong[3] = uLong4.to_ulong();

    switch(hopDistance){
    case ONE_HOP:
        pitIt = pitOneHopList.find(nodeAddress);
        csIt = csOneHopList.find(nodeAddress);
        if(pitIt == pitOneHopList.end()){
            EV<<"new map being inserted with address::"<<nodeAddress<<endl;
            EV<<"with values:"<<ulong[0]<<";"<<ulong[1]<<";"<<ulong[2]<<";"<<ulong[3]<<";"<<endl;
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitMap);
            pitOneHopList.insert(pitMapPair);
        } else{
            EV<<"old address, updating old entry"<<endl;
            newMap = pitIt->second | pitMap;
            pitOneHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        if(csIt == csOneHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress, csMap);
            csOneHopList.insert(csMapPair);
        } else{
            newMap = csIt->second | csMap;
            csOneHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        break;
    case TWO_HOP:
        pitIt = pitTwoHopList.find(nodeAddress);
        csIt = csTwoHopList.find(nodeAddress);
        if(pitIt == pitTwoHopList.end()){
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitMap);
            pitTwoHopList.insert(pitMapPair);
        } else{
            newMap = pitIt->second | pitMap;
            pitTwoHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        if(csIt == csTwoHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress,csMap);
            csTwoHopList.insert(csMapPair);
        } else{
            newMap = csIt->second | csMap;
            csTwoHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        break;
    case THREE_HOP:
        pitIt = pitThreeHopList.find(nodeAddress);
        csIt = csThreeHopList.find(nodeAddress);
        if(pitIt == pitThreeHopList.end()){
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitMap);
            pitThreeHopList.insert(pitMapPair);
        } else{
            newMap = pitIt->second | csMap;
            pitThreeHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        if(csIt == csThreeHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress, csMap);
            csThreeHopList.insert(csMapPair);
        } else{
            newMap = csIt->second | csMap;
            csThreeHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        break;

    case X_HOP:
        pitIt = pitXHopList.find(nodeAddress);
        csIt = csXHopList.find(nodeAddress);
        if(pitIt == pitThreeHopList.end()){
            NeighbourMap::value_type pitMapPair = make_pair(nodeAddress, pitMap);
            pitXHopList.insert(pitMapPair);
        } else{
            newMap = pitIt->second | csMap;
            pitXHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        if(csIt == csXHopList.end()){
            NeighbourMap::value_type csMapPair = make_pair(nodeAddress, csMap);
            csThreeHopList.insert(csMapPair);
        } else{
            newMap = csIt->second | csMap;
            csXHopList[nodeAddress] = newMap;
            newMap.reset();
        }
        break;
    case CONTROL:
        ControlMap::key_type nodeAddress = hopAddress;
        ControlMap::iterator its = removeList.find(nodeAddress);

        if (its == removeList.end()) {
            EV << "updateControlMap(): The message came from a new neighbor! " << endl;

            // insert key value pair <node address, event> in neighborhood map.
            cMessage *removeEvent = new cMessage("removeEvent", FORWARDING_INFO_TIMER);

            // schedule the event to remove the entry after initT seconds
            scheduleAt(simTime()+(timeToLive*2), removeEvent);

            ControlMap::value_type pairToInsert = make_pair(nodeAddress, removeEvent);
            pair<ControlMap::iterator, bool> ret = removeList.insert(pairToInsert);

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


            scheduleAt(simTime()+(timeToLive*2), its->second);

        }
        break;
    }
}

void ForwardingInfoBase::getHopMap(int hopDistance, int* pitBloomFilter, int* csBloomFilter)
{
    int mapSize;
    std::bitset<128> pitMap;
    std::bitset<128> csMap;

    NeighbourMap::iterator pitIt;
    NeighbourMap::iterator csIt;

    switch(hopDistance){
    case ONE_HOP:
        mapSize = pitOneHopList.size();
        if(mapSize){
            for(pitIt = pitOneHopList.begin(); pitIt != pitOneHopList.end(); ++pitIt){
                pitMap = pitMap | pitIt->second;
            }
        }
        mapSize = csTwoHopList.size();{
            if(mapSize){
                for(csIt = csOneHopList.begin(); csIt != csOneHopList.end(); ++csIt){
                    csMap = csMap | csIt->second;
                }
            }
        }
        break;
    case TWO_HOP:
        mapSize = pitTwoHopList.size();
        if(mapSize){
            for(pitIt = pitTwoHopList.begin(); pitIt != pitTwoHopList.end(); ++pitIt){
                pitMap = pitMap | pitIt->second;
            }
        }
        mapSize = csTwoHopList.size();{
            if(mapSize){
                for(csIt = csTwoHopList.begin(); csIt != csTwoHopList.end(); ++csIt){
                    csMap = csMap | csIt->second;
                }
            }
        }
        break;
    case THREE_HOP:
        mapSize = pitThreeHopList.size();
        if(mapSize){
            for(pitIt = pitThreeHopList.begin(); pitIt != pitThreeHopList.end(); ++pitIt){
                pitMap = pitMap | pitIt->second;
            }
        }
        mapSize = csThreeHopList.size();{
            if(mapSize){
                for(csIt = csThreeHopList.begin(); csIt != csThreeHopList.end(); ++csIt){
                    csMap = csMap | csIt->second;
                }
            }
        }
        break;
    case X_HOP:
        mapSize = pitXHopList.size();
        if(mapSize){
            for(pitIt = pitXHopList.begin(); pitIt != pitXHopList.end(); ++pitIt){
                pitMap = pitMap | pitIt->second;
            }
        }
        mapSize = csXHopList.size();{
            if(mapSize){
                for(csIt = csXHopList.begin(); csIt != csXHopList.end(); ++csIt){
                    csMap = csMap | csIt->second;
                }
            }
        }
        break;
    }

    for(int i = 0; i < 128 ;i++){
        pitBloomFilter[i] = pitMap[i];
        csBloomFilter[i] = csMap[i];
    }
}

void ForwardingInfoBase::searchMap(const char* name, LAddress::L3Type** hopMap, int neighbourList, int* addressLimit)
{
    Enter_Method_Silent("searchMap(const char* name, LAddress::L3Type** hopMap, int neighbourList, int* addressLimit)");
    uint32_t fHash;
    uint32_t sHash;
    uint32_t hashValues[3];
    NeighbourMap::iterator oneHopIt;
    NeighbourMap::iterator twoHopIt;
    NeighbourMap::iterator threeHopIt;
    NeighbourMap::iterator xHopIt;


    for(int i = 0; i < 4 ;i++){
        addressLimit[i] = 0;
    }

    hTable->getHashValues(name, hashValues);
    fHash = hashValues[0];
    sHash = hashValues[1];

    switch(neighbourList){
    case SEARCH_PIT_LIST:
        oneHopIt = pitOneHopList.begin();
        twoHopIt = pitTwoHopList.begin();
        threeHopIt = pitThreeHopList.begin();
        xHopIt = pitXHopList.begin();

        while(oneHopIt != pitOneHopList.end() && addressLimit[0] != 3){

            if(oneHopIt->second[fHash] == 1 && oneHopIt->second[sHash] == 1){
                EV<<"match was found"<<endl;
                hopMap[0][addressLimit[0]] = oneHopIt->first;
                addressLimit[0]++;
            }
            oneHopIt++;
        }

        while(twoHopIt != pitTwoHopList.end() && addressLimit[1] != 3){

            if(twoHopIt->second[fHash] == 1 && twoHopIt->second[sHash] == 1){
                hopMap[1][addressLimit[1]] = twoHopIt->first;
                addressLimit[1]++;
            }
            twoHopIt++;
        }

        while(threeHopIt != pitThreeHopList.end() && addressLimit[2] != 3){

            if(threeHopIt->second[fHash] == 1 && threeHopIt->second[sHash] == 1){
                hopMap[2][addressLimit[2]] = threeHopIt->first;
                addressLimit[2]++;
            }
            threeHopIt++;
        }
        while(xHopIt != pitXHopList.end() && addressLimit[2] != 3){
            if(xHopIt->second[fHash] == 1 && xHopIt->second[sHash] == 1){
                hopMap[3][addressLimit[3]] = xHopIt->first;
                addressLimit[3]++;
            }
            xHopIt++;
        }
        break;
    case SEARCH_CS_LIST:
        oneHopIt = csOneHopList.begin();
        twoHopIt = csTwoHopList.begin();
        threeHopIt = csThreeHopList.begin();

        while(oneHopIt != csOneHopList.end() && addressLimit[0] != 3){

            if(oneHopIt->second[fHash] == 1 && oneHopIt->second[sHash] == 1){
                hopMap[0][addressLimit[0]] = oneHopIt->first;
                addressLimit[0]++;
            }
            oneHopIt++;
        }
        addressLimit = 0;
        while(twoHopIt != csTwoHopList.end() && addressLimit[1] != 3){

            if(twoHopIt->second[fHash] == 1 && twoHopIt->second[sHash] == 1){
                hopMap[1][addressLimit[1]] = twoHopIt->first;
                addressLimit[1]++;
            }
            twoHopIt++;
        }
        addressLimit = 0;
        while(threeHopIt != csThreeHopList.end() && addressLimit[2] != 3){

            if(threeHopIt->second[fHash] == 1 && threeHopIt->second[sHash] == 1){
                hopMap[2][addressLimit[2]] = threeHopIt->first;
                addressLimit[2]++;
            }
            threeHopIt++;
        }
        while(xHopIt != csXHopList.end() && addressLimit[2] != 3){

            if(xHopIt->second[fHash] == 1 && xHopIt->second[sHash] == 1){
                hopMap[3][addressLimit[3]] = xHopIt->first;
                addressLimit[3]++;
            }
            xHopIt++;
        }
        break;
    }
}

void ForwardingInfoBase::removeMapFromFib(cMessage* msg)
{
    EV<<"removing name map"<<endl;
    const ControlMap::key_type& nodeAddress = *static_cast<ControlMap::key_type*>( msg->getContextPointer() );
    ControlMap::iterator it = removeList.find(nodeAddress);

    cancelAndDelete(removeList.find(it->first)->second);
    removeList.erase(it);
    NeighbourMap::iterator its;

    // erase one hop list entry
    its = pitOneHopList.find(nodeAddress);
    pitOneHopList.erase(its);
    its = csOneHopList.find(nodeAddress);
    csOneHopList.erase(its);

    // erase two hop list entry
    its = pitTwoHopList.find(nodeAddress);
    pitTwoHopList.erase(its);
    its = csTwoHopList.find(nodeAddress);
    csTwoHopList.erase(its);

    // erase three hop list entry
    its = pitThreeHopList.find(nodeAddress);
    pitThreeHopList.erase(its);
    its = csThreeHopList.find(nodeAddress);
    csThreeHopList.erase(its);

    // erase x hop list entry
    its = pitXHopList.find(nodeAddress);
    pitXHopList.erase(its);
    its = csXHopList.find(nodeAddress);
    csXHopList.erase(its);
}


