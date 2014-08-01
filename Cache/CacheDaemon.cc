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

#include "CacheDaemon.h"
#include "FindModule.h"

using std::map;
using std::pair;
using std::make_pair;
using std::endl;

Define_Module(CacheDaemon);

void CacheDaemon::initialize(int stage)
{
    BaseModule::initialize(stage);
    if(stage == 0){
        CacheSize = par("CacheSize");
        WordSize = par("WordSize");
        timeToLive = par("timeToLive");
        startDelay = par("startDelay");
        startMsg = new cMessage("start message", START_MESSAGE);
        spookyHasher = new SpookyHash;
        mmHasher = new MurmurHash;
        Pit = FindModule<PendingInterestTable*>::findSubModule(findHost());
        Cs = FindModule<ContentStore*>::findSubModule(findHost());
        Fib = FindModule<ForwardingInfoBase*>::findSubModule(findHost());

    }
    if(stage == 0){
        scheduleAt(simTime() + startDelay, startMsg);
    }
}

void CacheDaemon::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);

    } else{
        opp_error("Cache should not receive messages from other layers, recheck configuration");
    }
}

void CacheDaemon::handleSelfMsg(cMessage* msg)
{

    switch(msg->getKind()){
    case START_MESSAGE:

        EV << " handle Self Msg "<< endl;
        cancelEvent(msg);
        testPit();
        break;
    case FORWARDING_INFO_TIMER:
        break;

    }


}



/*
 * ***************************************************************************
 * Compound functions
 * ***************************************************************************
 */

int CacheDaemon::processInterest(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr, int hopCount)
{
    Enter_Method_Silent("processInterest(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr, int hopCount)");
    int instructionStatus;
    int applicationInstruction;

    instructionStatus = Cs->retreiveContentStore(name);
    if(instructionStatus == DATA_FOUND){
        applicationInstruction = SEND_DATA;
        Pit->deleteEntryFromPIT(name);
    } else{
        //Pit->
        instructionStatus = Pit->updatePendingInterestTable(name,reqAddr,Pit->EXT_REQ,srcAddr);
        if(instructionStatus == INSERT_COMPLETED){
            applicationInstruction = SEND_INTEREST;
        } else{
            applicationInstruction = DO_NOTHING;
        }
    }

    return applicationInstruction;
}

int CacheDaemon::generateInterestEntry(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr)
{
    Enter_Method_Silent("generateInterestEntry(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr)");
    int instructionStatus;
    int applicationInstruction;
    instructionStatus = Cs->retreiveContentStore(name);

    if(instructionStatus == DATA_NOT_FOUND){
        instructionStatus = Pit->updatePendingInterestTable(name, reqAddr, Pit->SELF_REQ, srcAddr);
        if(instructionStatus == INSERT_COMPLETED){
            applicationInstruction = SEND_INTEREST;

        } else{
            applicationInstruction = DO_NOTHING;
        }
    } else{
        Cs->updateContentStore(name);
    }

    return applicationInstruction;
}

int CacheDaemon::processData(const char* name, int* hopCount, LAddress::L3Type* iAddr)
{
    Enter_Method_Silent("processData(const char* name, int* hopCount, LAddress::L3Type* iAddr)");
    int instructionStatus;
    int applicationInstruction;
    LAddress::L3Type* reqAddr;
    //LAddress::L3Type* iAddr;
    int* reqType;

    instructionStatus = Pit->retreiveEntryFromPIT(name, reqType,reqAddr,iAddr);
    if(instructionStatus == DATA_FOUND){
        switch(*reqType){
        case SELF_REQ:
            Cs->updateContentStore(name);
            applicationInstruction = INTEREST_FOUND;
            break;
        case EXT_REQ:
            Fib->retreiveEntryFromNameInterfaceList(name, hopCount, iAddr);
            applicationInstruction = SEND_DATA;

            break;
        case DUAL_TYPE_REQ:
            Cs->updateContentStore(name);
            Fib->retreiveEntryFromNameInterfaceList(name, hopCount, iAddr);
            applicationInstruction = SEND_DATA_INTEREST_FOUND;
            break;
        }
    } else{
        Cs->updateContentStore(name);
        applicationInstruction = DO_NOTHING;
    }

    return applicationInstruction;
}

void CacheDaemon::updateForwardingInfoBase(LAddress::L3Type hopAddress, uint32_t** oneHop,uint32_t** twoHop,uint32_t** threeHop, uint32_t** xHop)
{
    Enter_Method_Silent("updateForwardingInfoBase(LAddress::L3Type hopAddress, uint32_t** oneHop,uint32_t** twoHop,uint32_t** threeHop, uint32_t** xHop)");
    int hopDistance;
    int pitBloomFilter[128];
    int csBloomFilter[128];

    // update one hop list
    ulongToArray(pitBloomFilter, oneHop[0]);
    ulongToArray(csBloomFilter, oneHop[1]);
    hopDistance = ONE_HOP;
    Fib->updateHopMap(hopAddress, pitBloomFilter,csBloomFilter,hopDistance);

    // update two hop list
    ulongToArray(pitBloomFilter, twoHop[0]);
    ulongToArray(csBloomFilter, twoHop[1]);
    hopDistance = TWO_HOP;
    Fib->updateHopMap(hopAddress, pitBloomFilter,csBloomFilter,hopDistance);

    // update three hop list
    ulongToArray(pitBloomFilter, threeHop[0]);
    ulongToArray(csBloomFilter, threeHop[1]);
    hopDistance = THREE_HOP;
    Fib->updateHopMap(hopAddress, pitBloomFilter,csBloomFilter,hopDistance);

    // update xHop list
    ulongToArray(pitBloomFilter, xHop[0]);
    ulongToArray(csBloomFilter, xHop[1]);
    hopDistance = X_HOP;
    Fib->updateHopMap(hopAddress, pitBloomFilter,csBloomFilter,hopDistance);

    // update control map
    hopDistance = CONTROL;
    Fib->updateHopMap(hopAddress, pitBloomFilter, csBloomFilter, hopDistance);



}

void CacheDaemon::updateInterfaceList(const char* name, int hopCount, LAddress::L3Type hopAddress)
{
    Enter_Method_Silent("updateInterfaceList(const char* name, int hopCount, LAddress::L3Type hopAddress)");
    Fib->updateNameInterfaceList(name, hopCount, hopAddress);
}

void CacheDaemon::generatePacketMaps(uint32_t** oneHopList, uint32_t** twoHopList, uint32_t** threeHopList, uint32_t** xHopList)
{
    Enter_Method_Silent("generatePacketMaps(uint32_t** oneHopList, uint32_t** twoHopList, uint32_t** threeHopList, uint32_t** xHopList)");
    int pitBloomFilter[128];
    int csBloomFilter[128];
    // get Cache BloomFilters
    Pit->getPitBloomFilter(pitBloomFilter);
    Cs->getCsBloomFilter(csBloomFilter);
    arrayToUlong(pitBloomFilter, oneHopList[0]);
    arrayToUlong(csBloomFilter, oneHopList[1]);

    // get one hop list map
    Fib->getHopMap(ONE_HOP, pitBloomFilter, csBloomFilter);
    arrayToUlong(pitBloomFilter, twoHopList[0]);
    arrayToUlong(csBloomFilter, twoHopList[1]);

    // get two hop list maps
    Fib->getHopMap(TWO_HOP, pitBloomFilter, csBloomFilter);
    arrayToUlong(pitBloomFilter, threeHopList[0]);
    arrayToUlong(csBloomFilter, threeHopList[1]);

    // get X Hop list
    Fib->getHopMap(THREE_HOP, pitBloomFilter, csBloomFilter);    // get three hop list
    int tempPitBloomFilter[128];
    int tempCsBloomFilter[128];
    Fib->getHopMap(X_HOP, tempPitBloomFilter,tempCsBloomFilter); //  get x-hop list
    // compress x and three hop list into one lsit
    for(int i = 0; i < 128;i++){
        pitBloomFilter[i] = pitBloomFilter[i] | tempPitBloomFilter[i];
        csBloomFilter[i] = csBloomFilter[i] | tempCsBloomFilter[i];
    }
    arrayToUlong(pitBloomFilter, xHopList[0]);
    arrayToUlong(csBloomFilter, xHopList[1]);
}

int CacheDaemon::getRoutingInfo(const char* name, int* Distance, LAddress::L3Type* iAddr)
{
    Enter_Method_Silent("getRoutingInfo(const char* name, int* Distance, LAddress::L3Type* iAddr)");
    int applicationStatus;
    applicationStatus = Fib->retreiveEntryFromNameInterfaceList(name,Distance, iAddr);

    return applicationStatus;
}

int CacheDaemon::searchForInterest(const char* name, LAddress::L3Type* iAddr, int* minMax)
{
    Enter_Method_Silent("searchForInterest(const char* name, LAddress::L3Type* iAddr, int* minMax)");
    int neighbourList = Fib->SEARCH_PIT_LIST;
    int limit[4];
    int a,b,c,d;
    int min,max;
    a = 1;
    b = 0.5;
    c = 0.25;
    d = 0.1;
    LAddress::L3Type** hopList = new LAddress::L3Type*[4];
    for(int i = 0;i < 4;i++){
        hopList[i] = new LAddress::L3Type[3];
        for(int o = 0;o<3;o++){
            hopList[i][o] = LAddress::L3NULL;
        }
    }
    // search for interface clostest to node containing matching data

    Fib->searchMap(name,hopList, neighbourList,limit);

    // broadcast heuristic
    int broadcastLimit = (a*limit[0])+(b*limit[1])+(c*limit[2])+(d*limit[3]);
    if(broadcastLimit == 0){

    }else if(broadcastLimit > 3 && broadcastLimit < 1){
        *iAddr = LAddress::L3BROADCAST;
    } else{
        // determine mix and max distances
        for(int j = 0 ;j < 4;j++){
            if(limit[j]){
                max = j+1;
            }
        }

        for(int k = 4; k > 0;k--){
            if(limit[k-1]){
                min = k;
            }
        }

        *iAddr = hopList[min-1][0];
    }

    minMax[0] = min;
    minMax[1] = max;


    return SEARCH_COMPLETE;
}

int CacheDaemon::searchForData(const char* name, LAddress::L3Type* iAddr, int* minMax)
{
    Enter_Method_Silent("searchForData(const char* name, LAddress::L3Type* iAddr, int* minMax)");
    int neighbourList = Fib->SEARCH_CS_LIST;
    int limit[4];
    float a,b,c,d;
    int min,max;

    a = 1;
    b = 0.5;
    c = 0.25;
    d = 0.1;

    LAddress::L3Type** hopList = new LAddress::L3Type*[4];
    for(int i = 0;i < 4;i++){
        hopList[i] = new LAddress::L3Type[4];
    }
    // search for interface clostest to node containing matching data
    Fib->searchMap(name,hopList, neighbourList,limit);

    // broadcast heuristic
    float broadcastLimit = (a*limit[0])+(b*limit[1])+(c*limit[2])+(d*limit[3]);
    if(broadcastLimit > 3){
        *iAddr = LAddress::L3BROADCAST;
    } else{
        // determine mix and max distances
        for(int j = 0 ;j < 4;j++){
            if(limit[j]){
                max = j+1;
            }
        }

        for(int k = 4; k > 0;k--){
            if(limit[k-1]){
                min = k;
            }
        }
        *iAddr = hopList[min-1][0];
    }

    minMax[0] = min;
    minMax[1] = max;

    *iAddr = hopList[min-1][0];

    return SEARCH_COMPLETE;
}

/*
 * ***************************************************************************
 * end of Compound Functions
 * ***************************************************************************
 */

/*
 * test functions
 */

void CacheDaemon::testFib()
{
    EV<<"starting fib test"<<endl;
    // name space test function

    // generate a dummy set of hop lists
    int pitBloomFilter[128];
    int csBloomFilter[128];
    char** word;
    uint32_t** oneHop;
    uint32_t** twoHop;
    uint32_t** threeHop;
    uint32_t** xHop;

    oneHop = new uint32_t*[2];
    twoHop = new uint32_t*[2];
    threeHop = new uint32_t*[2];
    xHop = new uint32_t*[2];

    for(int j = 0 ;j< 2;j++){
        oneHop[j] = new uint32_t[4];
        twoHop[j] = new uint32_t[4];
        threeHop[j] = new uint32_t[4];
        xHop[j] = new uint32_t[4];
    }


    word = new char*[8];
    for(int i = 0;i<8;i++){
        word[i] = new char[100];
    }

    strcpy(word[0], "Thylamus");
    strcpy(word[1], "Obscesive");
    strcpy(word[2], "DogFood");
    strcpy(word[3], "Foolish");
    strcpy(word[4], "thingy");
    strcpy(word[5], "person");
    strcpy(word[6], "ocd");
    strcpy(word[7], "danebrentcrowe");

    uint32_t h1;
    uint32_t h2;

    h1 = mmHasher->MurmurHash3(word[0],strlen(word[0]),1)%CacheSize;
    h2 = spookyHasher->SpookHash(word[0],strlen(word[0]),1)%CacheSize;
    pitBloomFilter[h1] = 1;
    pitBloomFilter[h2] = 1;
    csBloomFilter[h1] = 1;
    csBloomFilter[h2] = 1;

    h1 = mmHasher->MurmurHash3(word[4],strlen(word[4]),1)%CacheSize;
    h2 = spookyHasher->SpookHash(word[4],strlen(word[4]),1)%CacheSize;
    pitBloomFilter[h1] = 1;
    pitBloomFilter[h2] = 1;
    csBloomFilter[h1] = 1;
    csBloomFilter[h2] = 1;


    h1 = mmHasher->MurmurHash3(word[5],strlen(word[5]),1)%CacheSize;
    h2 = spookyHasher->SpookHash(word[5],strlen(word[5]),1)%CacheSize;
    pitBloomFilter[h1] = 1;
    pitBloomFilter[h2] = 1;
    csBloomFilter[h1] = 1;
    csBloomFilter[h2] = 1;

    arrayToUlong(pitBloomFilter,oneHop[0]);
    arrayToUlong(csBloomFilter,oneHop[1]);

    /*
    h1 = mmHasher->MurmurHash3(word[1],strlen(word[1]),1)%CacheSize;
    h2 = spookyHasher->SpookHash(word[1],strlen(word[1]),1)%CacheSize;
    pitBloomFilter[h1] = 1;
    pitBloomFilter[h2] = 1;
    csBloomFilter[h1] = 1;
    csBloomFilter[h2] = 1;
    arrayToUlong(pitBloomFilter,twoHop[0]);
    arrayToUlong(csBloomFilter,twoHop[1]);
    pitBloomFilter[h1] = 0;
    pitBloomFilter[h2] = 0;
    csBloomFilter[h1] = 0;
    csBloomFilter[h2] = 0;


    h1 = mmHasher->MurmurHash3(word[2],strlen(word[2]),1)%CacheSize;
    h2 = spookyHasher->SpookHash(word[2],strlen(word[2]),1)%CacheSize;
    pitBloomFilter[h1] = 1;
    pitBloomFilter[h2] = 1;
    csBloomFilter[h1] = 1;
    csBloomFilter[h2] = 1;
    arrayToUlong(pitBloomFilter,threeHop[0]);
    arrayToUlong(csBloomFilter,threeHop[1]);
    pitBloomFilter[h1] = 0;
    pitBloomFilter[h2] = 0;
    csBloomFilter[h1] = 0;
    csBloomFilter[h2] = 0;


    h1 = mmHasher->MurmurHash3(word[3],strlen(word[3]),1)%CacheSize;
    h2 = spookyHasher->SpookHash(word[3],strlen(word[3]),1)%CacheSize;
    pitBloomFilter[h1] = 1;
    pitBloomFilter[h2] = 1;
    csBloomFilter[h1] = 1;
    csBloomFilter[h2] = 1;
    arrayToUlong(pitBloomFilter,xHop[0]);
    arrayToUlong(csBloomFilter,xHop[1]);
    pitBloomFilter[h1] = 0;
    pitBloomFilter[h2] = 0;
    csBloomFilter[h1] = 0;
    csBloomFilter[h2] = 0;

     */
    LAddress::L3Type hopAddress = LAddress::L3Type(100);
    LAddress::L3Type dummyAddress = LAddress::L3Type(56);
    updateForwardingInfoBase(hopAddress,oneHop,twoHop,threeHop,xHop);

    EV<<"UPDATE Test is now complete"<<endl;

    // second test
    LAddress::L3Type searchAddress = LAddress::L3NULL;
    int mm[2];
    searchForInterest(word[4],&searchAddress,mm);
    if(searchAddress == hopAddress){
        EV<<"success for SEARCH"<<endl;
        EV<<"at min distance:"<<mm[0]<<endl;
    } else{
        EV<<"address was not found"<<endl;
    }
    Fib->getHopMap(Fib->ONE_HOP,pitBloomFilter,csBloomFilter);
    uint32_t checkList[4];
    arrayToUlong(pitBloomFilter,checkList);
    EV<<"check list:"<<checkList[0]<<";"<<checkList[1]<<";"<<checkList[2]<<";"<<checkList[3]<<";"<<endl;

    int testStatus;
    updateInterfaceList(word[0],3,hopAddress);
    updateInterfaceList(word[0],5, dummyAddress);
    EV<<"update name interface list complete"<<endl;
    int hopDistance;
    searchAddress = LAddress::L3NULL;
    testStatus = getRoutingInfo(word[1],&hopDistance,&searchAddress);
    if(testStatus == Fib->NAME_NOT_FOUND){
        EV<<"name not found"<<endl;
    } else if(hopDistance == 3 && searchAddress == hopAddress){

        EV<<"name interface search successful"<<endl;

    } else{
        EV<<"interface search FAILURE"<<endl;
    }
}

void CacheDaemon::testPit()
{
    LAddress::L3Type reqAddress, iAddress,requester,interface;
    int reqType = Pit->EXT_REQ;
    int checkType = Pit->NO_REQ;

    reqAddress = LAddress::L3Type(10);
    iAddress = LAddress::L3Type(55);
    requester = LAddress::L3NULL;
    interface = LAddress::L3NULL;
    char** word;

    word = new char*[8];
    for(int i = 0;i<8;i++){
        word[i] = new char[100];
    }

    strcpy(word[0], "Thylamus");
    strcpy(word[1], "Obscesive");
    strcpy(word[2], "DogFood");
    strcpy(word[3], "Foolish");
    strcpy(word[4], "thingy");
    strcpy(word[5], "person");
    strcpy(word[6], "ocd");
    strcpy(word[7], "danebrentcrowe");
    int instructionStatus = Pit->updatePendingInterestTable(word[0], reqAddress,reqType,iAddress);
    if(instructionStatus == Pit->INSERT_COMPLETED){
        EV<<"name insertion at Pit SUCCESS"<<endl;
        instructionStatus = Pit->updatePendingInterestTable(word[0], reqAddress,reqType,iAddress);
        if(instructionStatus == Pit->UPDATE_COMPLETED){
            EV<<"UPDATE METHOD IS WORKING COMPLETELY"<<endl;
            instructionStatus = Pit->retreiveEntryFromPIT(word[0],&checkType,&requester,&interface);
            if(instructionStatus == Pit->DATA_FOUND){
                if(checkType == reqType){
                    EV<<"requestType match!";
                }
                if(requester == reqAddress){
                    EV<<"creator match!"<<endl;
                }
                if(interface == iAddress){
                    EV<<"interface MATCH!"<<endl;
                }
            }
        }
    }

}
/*
 * end of test functions
 */


/*
 * ***************************************************************************
 * Extra Functions
 * ***************************************************************************
 */

void CacheDaemon::ulongToArray(int* array, uint32_t* ulong)
{
    std::bitset<32> uLong1 (ulong[0]);
    std::bitset<32> uLong2 (ulong[1]);
    std::bitset<32> uLong3 (ulong[2]);
    std::bitset<32> uLong4 (ulong[3]);

    for(int i = 0; i< 32; i++){
        array[i] = uLong1[i];
        array[i+32] = uLong2[i];
        array[i+64] = uLong3[i];
        array[i+96] = uLong4[i];
    }
}

void CacheDaemon::arrayToUlong(int* array, uint32_t* ulong)
{
    std::bitset<32> uLong1 (0);
    std::bitset<32> uLong2 (0);
    std::bitset<32> uLong3 (0);
    std::bitset<32> uLong4 (0);

    for(int i = 0; i< 32 ;i++){
        uLong1[i] = array[i];
        uLong2[i] = array[i+32];
        uLong3[i] = array[i+64];
        uLong4[i] = array[i+96];
    }

    ulong[0] = uLong1.to_ulong();
    ulong[1] = uLong2.to_ulong();
    ulong[2] = uLong3.to_ulong();
    ulong[3] = uLong4.to_ulong();
}
