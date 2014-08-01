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

#ifndef __MIXIM_NDNNETWLAYER_H_
#define __MIXIM_NDNNETWLAYER_H_

#include <omnetpp.h>
#include "BaseNetwLayer.h"
#include "NdnNetwPkt_m.h"
#include "NdnDaemon.h"

class NdnNetwPkt;

class NdnNetwLayer : public BaseNetwLayer
{
public:
    enum NDN_NETW_MESSAGE_KINDS{
        NDN_NETW_INTEREST_MESSAGE = 24501,
        NDN_NETW_DATA_MESSAGE,
        NDN_NETW_SELF_MESSAGE
    };

    NdnDaemon::ndStore* store;
    virtual void initialize(int stage);
  protected:
    int CacheSize;
    NdnDaemon* ndnDaemon;

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleLowerMsg(cMessage *msg);
    virtual void handleUpperMsg(cMessage *msg);
    cMessage* decapMsg(NdnNetwPkt* msg);
    NdnNetwPkt* encapMsg(cMessage* msg);

    void sendNetwPkt();
    void generateRoutingInfo();
    void extractBloomFilter(NdnNetwPkt* m, int* pitBloomFilter, int* csBloomFilter);
    void getBloomFilter(NdnNetwPkt* pkt,uint32_t* pitBloomFilter, uint32_t* csBloomFilter);


};

#endif
