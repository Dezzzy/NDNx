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

#include "NdnDaemon.h"
#include "FindModule.h"

Define_Module(NdnDaemon);

void NdnDaemon::initialize(int stage)
{
    BaseModule::initialize(stage);
    if(stage == 0){
        CacheDaemon = FindModule<NdnCacheDaemon*>::findSubModule(findHost());
        store = new NdnCacheDaemon::ciStore;

        delayMsg = new cMessage("Setup Message",SETUP_MESSAGE);
    }
    if(stage == 1){
        scheduleAt(simTime() + setupDelay, delayMsg );
    }
}

void NdnDaemon::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);
    } else{
        opp_error("Cache should not receive messages from other layers, recheck configuration");
    }
}

void NdnDaemon::handleSelfMsg(cMessage* msg)
{
    if(msg->getKind() == SETUP_MESSAGE){
        generateData();
        cancelEvent(delayMsg);
    }
}

int  NdnDaemon::processInstruction(const char* name, ndStore* Container)
{

    int instructionStatus;
    int instructionType;
    instructionType = Container->instruction;
    switch(instructionType){
    case PROCESS_INTEREST:
        instructionStatus = processInterest(name, Container->requesterAddress);
        break;
    case PROCESS_DATA:
        instructionStatus = processData(name);
        break;
    case GENERATE_INTEREST:
        instructionStatus = genInterest(name, Container->requesterAddress);
        break;
    case GENERATE_BLOOMFILTER_MAP:
        genBFMap(Container->fBloomFilter,Container->sBloomFilter);
        break;
    case CONVERT_BLOOMFILTER_MAP:
        convertBFMap(Container->fBloomFilter);
        break;
    case UPDATE_NEIGHBOUR_MAP:
        updateNeighMap(Container->requesterAddress, Container->fBloomFilter, Container->sBloomFilter, Container->hopDist);

        break;
    case GET_NEIGHBOUR_MAP:
        getNeighbourMap(Container->hopDist,Container->fBloomFilter, Container->sBloomFilter);
        break;
    }

    return instructionStatus;
}

int NdnDaemon::processInterest(const char* name, LAddress::L3Type reqAddr)
{
    int instructionStatus;
    int requestStatus;

    store->instructionType = CacheDaemon->FIND_RETREIVE;
    store->cacheType = CacheDaemon->CS;
    requestStatus = CacheDaemon->processRequest(name,store);
    if(requestStatus ==  CacheDaemon->ENTRY_FOUND){


        if(!CacheDaemon->getInterestCompleted(name)){
            instructionStatus = SEND_DATA;
            CacheDaemon->setInterestCompleted(name);
        } else{
            instructionStatus = DO_NOTHING;
        }
    } else{
        store->instructionType = CacheDaemon->FIND_UPDATE;
        store->cacheType = CacheDaemon->PIT;
        store->reqTypes = CacheDaemon->EXT_REQ;
        store->requesterAddress = reqAddr;
        requestStatus = CacheDaemon->processRequest(name, store);
        if(requestStatus == CacheDaemon->ENTRY_UPDATED){
            instructionStatus = DO_NOTHING;
        }else if(requestStatus == CacheDaemon->ENTRY_INSERTED)
            instructionStatus = SEND_INTEREST;
    }

    return instructionStatus;
}

int NdnDaemon::processData(const char* name)
{
    int instructionStatus;
    int requestStatus;
    store->instructionType = CacheDaemon->FIND_RETREIVE;
    store->cacheType = CacheDaemon->PIT;
    requestStatus = CacheDaemon->processRequest(name, store);
    if(requestStatus ==  CacheDaemon->ENTRY_FOUND){
        switch(store->reqTypes){
        case NdnCacheDaemon::SELF_REQ:
            store->cacheType = CacheDaemon->CS;
            store->instructionType = CacheDaemon->FIND_UPDATE;
            CacheDaemon->processRequest(name, store);

            instructionStatus  = DO_NOTHING;
            break;
        case NdnCacheDaemon::EXT_REQ:
            instructionStatus = SEND_DATA;
            break;
        case NdnCacheDaemon::SELF_EXT_REQ:
            instructionStatus = SEND_DATA;
            store->cacheType = CacheDaemon->CS;
            store->instructionType = CacheDaemon->FIND_UPDATE;
            CacheDaemon->processRequest(name, store);

            break;
        }
        store->cacheType = CacheDaemon->PIT;
        store->instructionType = CacheDaemon->FIND_DELETE;
        CacheDaemon->processRequest(name, store);
    } else {
        store->instructionType = CacheDaemon->FIND_UPDATE;
        store->cacheType = CacheDaemon->CS;
        requestStatus = CacheDaemon->processRequest(name, store);
        instructionStatus = DO_NOTHING;
    }
    return  instructionStatus;
}

void NdnDaemon::genBFMap(uint32_t* pitBFMap, uint32_t* csBFMap)
{
    CacheDaemon->generateBloomFilterMap(CacheDaemon->PIT,pitBFMap);
    CacheDaemon->generateBloomFilterMap(CacheDaemon->CS, csBFMap);
}

std::bitset<128> NdnDaemon::convertBFMap(uint32_t* BloomfilterMap)
{
    std::bitset<128> map;
    map = CacheDaemon->convertBloomFilterMap(BloomfilterMap);
    return map;
}

int NdnDaemon::genInterest(const char* name, LAddress::L3Type reqAddress)
{
    int instructionStatus;
    int requestStatus;

    store->instructionType = CacheDaemon->FIND_UPDATE;
    store->cacheType = CacheDaemon->PIT;
    store->reqTypes = CacheDaemon->SELF_REQ;
    store->requesterAddress = reqAddress;

    requestStatus = CacheDaemon->processRequest(name, store);

    if(requestStatus == CacheDaemon->ENTRY_INSERTED){
        instructionStatus = SEND_INTEREST;
    }

    return instructionStatus;

}
int NdnDaemon::generateData()
{
    store->cacheType = CacheDaemon->CS;
    store->instructionType = CacheDaemon->FIND_UPDATE;
    CacheDaemon->processRequest("car", store);
    CacheDaemon->processRequest("pen", store);
    CacheDaemon->processRequest("Cape Town", store);
    CacheDaemon->processRequest("Average speed", store);
    CacheDaemon->processRequest("America", store);
    CacheDaemon->processRequest("Named Data", store);
    CacheDaemon->processRequest("target word", store);
    CacheDaemon->processRequest("dummy1", store);
    CacheDaemon->processRequest("dummy2", store);
    CacheDaemon->processRequest("dummy3", store);
    CacheDaemon->processRequest("dummy4", store);
    CacheDaemon->processRequest("dummy5", store);

    return 1;
}

void NdnDaemon::updateNeighMap(LAddress::L3Type neighbourAddress, uint32_t* pit, uint32_t* cs, int hop)
{
    int hopDist;

    if(hop == 1){
        hopDist = CacheDaemon->ONE_HOP;

    } else if(hop == 2){
        hopDist = CacheDaemon->TWO_HOP;

    } else{
        hopDist = CacheDaemon->THREE_HOP;
    }

    CacheDaemon->updateNeighbourMap(neighbourAddress, hopDist, pit, cs);

}

void NdnDaemon::getNeighbourMap(int hopList, uint32_t* pitBloomFilter, uint32_t* csBloomFilter)
{
    std::bitset<128> pitFilter;
    std::bitset<128> csFilter;
    std::bitset<32> Filter1,Filter2, Filter3, Filter4;
    int hopDist;
    if(hopList == 1){
        hopDist = CacheDaemon->ONE_HOP;

    } else if(hopList == 2){
        hopDist = CacheDaemon->TWO_HOP;

    } else{
        hopDist = CacheDaemon->THREE_HOP;
    }
    CacheDaemon->getMap(hopDist, &pitFilter, &csFilter);

    for(int i = 0; i < 32;i++){
        Filter1[i] = pitFilter[i];
        Filter2[i] = pitFilter[i+32];
        Filter3[i] = pitFilter[i+64];
        Filter4[i] = pitFilter[i+96];
    }

    pitBloomFilter[0] = Filter1.to_ulong();
    pitBloomFilter[1] = Filter2.to_ulong();
    pitBloomFilter[2] = Filter3.to_ulong();
    pitBloomFilter[3] = Filter4.to_ulong();

    for(int i = 0; i < 32;i++){
        Filter1[i] = csFilter[i];
        Filter2[i] = csFilter[i+32];
        Filter3[i] = csFilter[i+64];
        Filter4[i] = csFilter[i+96];
    }

    csBloomFilter[0] = Filter1.to_ulong();
    csBloomFilter[1] = Filter2.to_ulong();
    csBloomFilter[2] = Filter3.to_ulong();
    csBloomFilter[3] = Filter4.to_ulong();

}

int NdnDaemon::searchForwardingInfoBase(int hopDist, const char* name)
{

}




