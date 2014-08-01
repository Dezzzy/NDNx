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

#ifndef __MIXIM_NDNDAEMON_H_
#define __MIXIM_NDNDAEMON_H_

#include <omnetpp.h>
#include "BaseModule.h"
#include "NdnCacheDaemon.h"


class NdnDaemon : public BaseModule
{
public:

    typedef NdnCacheDaemon::BloomFilterMapping BFM;

    typedef struct BloomFilterContainer{
        BFM firstPitBloomFilter;
        BFM firstCsBloomFilter;
        BFM secondPitBloomFilter;
        BFM secondCsBloomFilter;
        BFM thirdPitBloomFilter;
        BFM thirdCsBloomFilter;

    }bfStore;

    typedef struct NdnDaemonContainer{
        uint32_t* fBloomFilter;
        uint32_t* sBloomFilter;
        int instruction;
        int hopDist;
        int* fullBloomFilter;
        LAddress::L3Type requesterAddress;
        bfStore* BloomFilterContainer;
    }ndStore;

    enum NDN_DAEMON_MESSAGE_TYPES{
        SETUP_MESSAGE
    };
    enum NDN_DAEMON_INSTRUCTION_TYPES{
        PROCESS_INTEREST,
        PROCESS_DATA,
        CHECK_FIB_INTEREST,
        CHECK_FIB_DATA,
        GENERATE_INTEREST,
        GENERATE_BLOOMFILTER_MAP,
        CONVERT_BLOOMFILTER_MAP,
        UPDATE_NEIGHBOUR_MAP,
        GET_NEIGHBOUR_MAP
    };
    enum INSTRUCTION_STATUS_TYPES{
        SEND_INTEREST,
        SEND_DATA,
        DO_NOTHING
    };
    void genBFMap(uint32_t* pitBFMap, uint32_t* csBFMap);
    std::bitset<128> convertBFMap(uint32_t* BloomfilterMap);
    int processInstruction(const char* name, ndStore* Container);
    void getNeighbourMap(int hopList, uint32_t* pitBloomFilter, uint32_t* csBloomFilter);
    int searchForwardingInfoBase(int hopDist, const char* name);

protected:
    NdnCacheDaemon::ciStore* store;
    NdnCacheDaemon* CacheDaemon;
    cMessage* delayMsg;
    simtime_t setupDelay;
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage* msg);

    int processInterest(const char* name, LAddress::L3Type reqAddr);
    int processData(const char* name);
    int genInterest(const char* name, LAddress::L3Type reqAddress);
    void updateNeighMap(LAddress::L3Type neighbourAddress, uint32_t* pit, uint32_t* cs, int hop);
    int generateData();



};

#endif
