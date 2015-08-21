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

#ifndef __MIXIM_CACHEDAEMON_H_
#define __MIXIM_CACHEDAEMON_H_


#include <set>
#include <map>

#include <omnetpp.h>
#include "BaseModule.h"
#include <bitset>
#include "SimpleAddress.h"
#include "PendingInterestTable.h"
#include "ContentStore.h"
#include "ForwardingInfoBase.h"

class CacheDaemon : public BaseModule
{
public:
    typedef std::bitset<128> BloomFilterMap;
    typedef std::map<LAddress::L3Type, BloomFilterMap> NeighbourMap;
    typedef std::map<LAddress::L3Type, cMessage*> ControlMap;



    enum CACHE_OPERATION_TYPES{
        INSERT_NAME,
        UPDATE_NAME,
        NAME_NOT_FOUND,
        NAME_DELETED,
        DATA_FOUND,
        DATA_NOT_FOUND,
        DATA_DELETED,
        INSERT_COMPLETED,
        UPDATE_COMPLETED,
        SEARCH_PIT_LIST,
        SEARCH_CS_LIST,
        SEARCH_COMPLETE
    };

    enum COMPLEX_OPERATION_TYPES{
        SEND_DATA,
        SEND_INTEREST,
        DO_NOTHING,
        STORE_DATA,
        STORE_AND_SEND_DATA,
        SEND_DATA_INTEREST_FOUND,
        INTEREST_FOUND
    };
    enum PENDING_INTEREST_REQUEST_TYPES{
        NO_REQ,
        SELF_REQ,
        EXT_REQ,
        DUAL_TYPE_REQ
    };

    enum FORWARDING_INFO_BASE_HOP_TYPES{
        ONE_HOP,
        TWO_HOP,
        THREE_HOP,
        X_HOP,
        CONTROL
    };
    enum CACHE_DAEMON_MESSAGE_TYPES{
        START_MESSAGE,
        FORWARDING_INFO_TIMER
    };

    // Compound Functions
    int processInterest(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr,int nonce);
    int generateInterestEntry(const char* name, LAddress::L3Type reqAddr, LAddress::L3Type srcAddr, int nonce);
    int processData(const char* name, int* hopCount, LAddress::L3Type* iAddr,int nonce);
    void updateForwardingInfoBase(LAddress::L3Type hopAddress, uint32_t** oneHop,uint32_t** twoHop,uint32_t** threeHop, uint32_t** xHop);
    void updateInterfaceList(const char* name, int hopCount, LAddress::L3Type hopAddress);
    void generatePacketMaps(uint32_t** oneHopList, uint32_t** twoHopList, uint32_t** threeHopList, uint32_t** xHopList);
    int getRoutingInfo(const char* name, int* Distance, LAddress::L3Type* iAddr);
    int searchForInterest(const char* name, LAddress::L3Type* iAddr, int* minMax);
    int searchForData(const char* name, LAddress::L3Type* iAddr, int* minMax);

    int searchDeadNonceList(const char* name, int nonce);
    void deleteDeadNonceElement(int elementIndex);
    void insertDeadNonceElement(const char* name, int* nonce);
    int getNonce(const char* name);
    void pitNoncelistHandoff(const char* name, int* nonce);


protected:
    int CacheSize;
    int WordSize;
    int TestingVariable;
    int** DeadNonce;
    int D_Nonce_List_Size = 50;
    int DeadNonceHashSeed = 5000;
    int DeadNonceListPtr;
    simtime_t timeToLive;
    simtime_t startDelay;
    SpookyHash* spookyHasher;
    MurmurHash* mmHasher;
    PendingInterestTable* Pit;
    ContentStore* Cs;
    ForwardingInfoBase* Fib;
    cMessage* startMsg;

    virtual void initialize(int stage);
    virtual void handleSelfMsg(cMessage *msg);
    virtual void handleUpperMsg(cMessage *msg){
        opp_error("no upper layer for CacheDaemon, check configuration");
    }

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage *msg);

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage *msg);

    /** @brief Handle control messages from upper layer */
    virtual void handleUpperControl(cMessage *msg){
        opp_error("no upper layer control for CacheDaemon, check configuration");
    }

    void initializeCache();




    // Compound Functions

    // extra functions
    void ulongToArray(int* array, uint32_t* ulong);
    void arrayToUlong(int* array, uint32_t* ulong);
    // testing functions

    void CacheTesting();
    void PendingInterestTableTesting();
    void ContentStoreTesting();
    void ForwardingInfoBaseTesting();
    void CacheDaemonTesting();


};

#endif
