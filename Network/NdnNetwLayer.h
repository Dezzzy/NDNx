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
#include "NdnNetPkt_m.h"


class NdnNetPkt;

class NdnNetwLayer : public BaseNetwLayer
{
public:
    enum NDN_NETW_MESSAGE_KINDS{
        NDN_NETW_INTEREST_MESSAGE = 24501,
        NDN_NETW_DATA_MESSAGE,
        NDN_NETW_SELF_MESSAGE
    };


    virtual void initialize(int stage);
  protected:
    int CacheSize;
    simtime_t timeToLive;


    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleLowerMsg(cMessage *msg);
    virtual void handleUpperMsg(cMessage *msg);
    cMessage* decapMsg(NdnNetPkt* msg);
    NdnNetPkt* encapMsg(cMessage* msg);
    int getNetMsgKind(int applMsgKind);



};

#endif
