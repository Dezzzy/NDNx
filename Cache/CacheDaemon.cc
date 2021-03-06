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
        TestingVariable = par("TestingVariable");
        startMsg = new cMessage("start message", START_MESSAGE);
        spookyHasher = new SpookyHash;
        mmHasher = new MurmurHash;
        DeadNonce = new int*[D_Nonce_List_Size];
        for(int i = 0; i < D_Nonce_List_Size;i++){
            DeadNonce[i] = new int[10];
        }
        DeadNonceListPtr = 0;
        Pit = FindModule<PendingInterestTable*>::findSubModule(findHost());
        Cs = FindModule<ContentStore*>::findSubModule(findHost());
        Fib = FindModule<ForwardingInfoBase*>::findSubModule(findHost());

    }
    if(stage == 0){
        scheduleAt(simTime() + startDelay, startMsg);
    }
}


void CacheDaemon::handleSelfMsg(cMessage* msg)
{

    switch(msg->getKind()){
    case START_MESSAGE:

        EV << " handle Self Msg "<< endl;
        cancelEvent(msg);
        if(TestingVariable){
            CacheTesting();
        }
        break;
    case FORWARDING_INFO_TIMER:
        break;

    }


}

void CacheDaemon::handleLowerMsg(cMessage* msg)
{
    // no layer functionality yet
}

void CacheDaemon::handleLowerControl(cMessage* msg)
{
    // no layer functionality yet
}


/*
 * ***************************************************************************
 * Compound functions
 * ***************************************************************************
 */

int CacheDaemon::processInterest(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr, int nonce)
{
    Enter_Method_Silent("processInterest(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr, int nonce)");
    int instructionStatus;
    int applicationInstruction;

    instructionStatus = Cs->retreiveContentStore(name);
    if(instructionStatus == ContentStore::DATA_FOUND){
        applicationInstruction = SEND_DATA;
        Pit->deleteEntryFromPIT(name);
    } else{

        instructionStatus = Pit->updatePendingInterestTable(name,reqAddr,Pit->EXT_REQ,srcAddr, nonce);
        if(instructionStatus == PendingInterestTable::INSERT_COMPLETED){
            applicationInstruction = SEND_INTEREST;
        } else{
            applicationInstruction = DO_NOTHING;
        }
    }

    return applicationInstruction;
}

int CacheDaemon::generateInterestEntry(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr, int nonce)
{
    Enter_Method_Silent("generateInterestEntry(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr)");
    int instructionStatus;
    int applicationInstruction;
    instructionStatus = Cs->retreiveContentStore(name);

    if(instructionStatus == ContentStore::DATA_NOT_FOUND){
        instructionStatus = Pit->updatePendingInterestTable(name, reqAddr, Pit->SELF_REQ, srcAddr, nonce);
        if(instructionStatus == Pit->INSERT_COMPLETED){
            applicationInstruction = SEND_INTEREST;

        } else{
            applicationInstruction = DO_NOTHING;
        }
    } else{
        //Cs->updateContentStore(name);
    }

    return applicationInstruction;
}

int CacheDaemon::processData(const char* name, int* hopCount, LAddress::L3Type* iAddr,int nonce)
{
    Enter_Method_Silent("processData(const char* name, int* hopCount, LAddress::L3Type* iAddr, int nonce)");
    int instructionStatus;
    int applicationInstruction;
    LAddress::L3Type* reqAddr;
    //LAddress::L3Type* iAddr;
    int* reqType;

    instructionStatus = Pit->retreiveEntryFromPIT(name, reqType,reqAddr,iAddr);
    if(instructionStatus == PendingInterestTable::DATA_FOUND){
        switch(*reqType){
        case PendingInterestTable::SELF_REQ:
            Cs->updateContentStore(name, nonce);
            applicationInstruction = INTEREST_FOUND;
            break;
        case PendingInterestTable::EXT_REQ:
            Fib->retreiveEntryFromNameInterfaceList(name, hopCount, iAddr);
            applicationInstruction = SEND_DATA;

            break;
        case PendingInterestTable::DUAL_TYPE_REQ:
            Cs->updateContentStore(name, nonce);
            Fib->retreiveEntryFromNameInterfaceList(name, hopCount, iAddr);
            applicationInstruction = SEND_DATA_INTEREST_FOUND;
            break;
        }
    } else{
        Cs->updateContentStore(name,nonce);
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
    if(broadcastLimit > 3 && broadcastLimit < 1){
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

int CacheDaemon::searchDeadNonceList(const char* name, int nonce)
{
    int len = strlen(name);

    int nameHash = mmHasher->MurmurHash3(name, len, 1) % DeadNonceHashSeed;
    int unseededNonce = nonce - nameHash;
    int i, nameFound = 0;

    while (!nameFound){
        if(i < D_Nonce_List_Size){
            if(DeadNonce[i][0] == nameHash){
                nameFound = 1;
            } else{
                i++;
            }
        }
    }

    int nonceFound = 0;
    int j = 1;
    while(!nonceFound){
        if(j < 10){
            if(DeadNonce[i][j] = unseededNonce){
                nonceFound = 1;
            } else{
                j++;
            }
        }

    }

    if(nonceFound && nameFound){
        return 1;
    } else{
        return 0;
    }
}

void CacheDaemon::deleteDeadNonceElement(int elementIndex)
{
    for(int i = 0;i < 10;i++){
        DeadNonce[elementIndex][i] = 0;
    }
}

void CacheDaemon::insertDeadNonceElement(const char* name, int* nonce)
{
    // get name hash
    int len =strlen(name);
    int NameHash = mmHasher->MurmurHash3(name, len, 1) % DeadNonceHashSeed;

    //int unseededNonce = nonce - NameHash;

    DeadNonceListPtr += 1;

    deleteDeadNonceElement(DeadNonceListPtr);

    DeadNonce[DeadNonceListPtr][0] = NameHash;

    for(int i = 1; i < 10;i++){
        DeadNonce[DeadNonceListPtr][i] = nonce[i-1];
    }
}

int CacheDaemon::getNonce(const char* name)
{
    int len = strlen(name);
    int NameHash = mmHasher->MurmurHash3(name, len,1);
    int Nonce = uniform(0,1000) + NameHash;

    return Nonce;
}

void CacheDaemon::pitNoncelistHandoff(const char* name, int* nonce)
{
    PendingInterestTable::NonceElement* tmp = new PendingInterestTable::NonceElement;

    int pitInstruction;
    do{
        pitInstruction = Pit->NonceListHandoff(tmp);
        insertDeadNonceElement(tmp->name, tmp->nonce);
        Pit->popElement();
    }while(pitInstruction != PendingInterestTable::NONCELIST_EMPTY);

}

/*
 * ***************************************************************************
 * end of Compound Functions
 * ***************************************************************************
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

void CacheDaemon::CacheTesting()
{
    // test individual caches for any mistakes
    PendingInterestTableTesting();
    ContentStoreTesting();
    ForwardingInfoBaseTesting();

    // test final cache operation
    CacheDaemonTesting();
}

void CacheDaemon::PendingInterestTableTesting()
{
    LAddress::L3Type srcAddress = LAddress::L3Type(100);
    LAddress::L3Type interfaceAddress = LAddress::L3Type(101);
    LAddress::L3Type sAddress = LAddress::L3NULL;
    LAddress::L3Type iAddress = LAddress::L3NULL;
    int requestType = PendingInterestTable::EXT_REQ;
    int reqType = 99;
    //
    Pit->updatePendingInterestTable("test",srcAddress,requestType,interfaceAddress,15122);
    Pit->retreiveEntryFromPIT("test",&reqType,&sAddress,&iAddress);
    int pitCheck = 0;
    while(pitCheck == 0){
        if(reqType == requestType && sAddress == srcAddress && iAddress == interfaceAddress){
            pitCheck = 0;
            EV<<"insertion and retreival of single name is SUCCESSFUL"<<endl;

        }
    }

    if(pitCheck){
        Pit->updatePendingInterestTable("butterfly",srcAddress,requestType,interfaceAddress,66666);
        Pit->retreiveEntryFromPIT("butterfly",&reqType,&sAddress,&iAddress);
        int secondPitCheck = 0;
        if(reqType == requestType && sAddress == srcAddress && iAddress == interfaceAddress){
            EV<<"insertion and retreival of second name is SUCCESSFUL"<<endl;
        }
    }


}

void CacheDaemon::ContentStoreTesting()
{
    Cs->updateContentStore("test", 1512);
    int DaemonInstruction = Cs->retreiveContentStore("test");

    // Insertion and Retrieval Test
    switch(DaemonInstruction){
    case ContentStore::DATA_NOT_FOUND:
        EV<<"test failed, data should be retreived, recheck"<<endl;
        break;
    case ContentStore::DATA_FOUND:
        EV<<"test success, first test complete"<<endl;
        break;
    case ContentStore::REQ_COMPLETED:
        EV<<"test failed, recheck CS module"<<endl;
        break;
    default:
        break;
    }

    // Second Retreival Test
    DaemonInstruction = Cs->retreiveContentStore("test");
    switch(DaemonInstruction){
    case ContentStore::DATA_NOT_FOUND:
        EV<<"test failed, data should be retreived, recheck"<<endl;
        break;
    case ContentStore::DATA_FOUND:
        EV<<"test failed, recheck CS module"<<endl;
        break;
    case ContentStore::REQ_COMPLETED:
        EV<<"test completed, second test complete"<<endl;
        break;
    default:
        break;
    }

    // Third Test, Error Test
    Cs->updateContentStore("Israel",6666);
    DaemonInstruction = Cs->retreiveContentStore("fuckup");
    //DaemonInstruction = Cs->retreiveContentStore("test");
    switch(DaemonInstruction){
    case ContentStore::DATA_NOT_FOUND:
        EV<<"test completed,third test complete"<<endl;
        break;
    case ContentStore::DATA_FOUND:
        EV<<"test failed, recheck CS module"<<endl;
        break;
    case ContentStore::REQ_COMPLETED:
        EV<<"test failed"<<endl;
        break;
    default:
        break;
    }
}

void CacheDaemon::ForwardingInfoBaseTesting()
{
    // test name interface list
    LAddress::L3Type interfaceAddress = LAddress::L3Type(100);
    int hopCount = 2;
    int hCount;
    LAddress::L3Type testAddress = LAddress::L3NULL;
    Fib->updateNameInterfaceList("test",hopCount,interfaceAddress);
    int faceListInstruction = Fib->retreiveEntryFromNameInterfaceList("test",&hCount,&testAddress);

    switch(faceListInstruction){
    case ForwardingInfoBase::NAME_FOUND:
        if(hCount == hopCount && testAddress == interfaceAddress){
            EV<< "FIB interface list test complete" << endl;
        } else{
            EV<< "FIB interface list test Failed "<< endl;
        }
        break;
    case ForwardingInfoBase::NAME_NOT_FOUND:
        EV<< "FIB interface list test Failed "<< endl;
        break;

    }
    // test hop map list


}

void CacheDaemon::CacheDaemonTesting()
{
    // interest processing
    LAddress::L3Type reqeustingAddress  = LAddress::L3Type(100);
    LAddress::L3Type sourceAddress = LAddress::L3Type(101);
    int hopDist = 1;

    int CacheInstruction = processInterest("unknown", reqeustingAddress,sourceAddress,hopDist);

    switch(CacheInstruction){
    case SEND_INTEREST:
        EV<<"CacheDaemon: processInterest  NEW INTEREST test success" <<endl;
        break;
    case SEND_DATA:
        EV<<"CacheDaemon: processInterest NEW INTEREST test FAILURE" <<endl;
        break;
    case DO_NOTHING:
        EV<<"CacheDaemon: processInterest NEW INTEREST test FAILURE" <<endl;
        break;
    }

    CacheInstruction = processInterest("test",reqeustingAddress,sourceAddress,hopDist);


}
