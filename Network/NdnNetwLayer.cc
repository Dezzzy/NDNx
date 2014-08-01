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

#include "NdnNetwLayer.h"
#include "FindModule.h"

Define_Module(NdnNetwLayer);

void NdnNetwLayer::initialize(int stage)
{
    BaseNetwLayer::initialize(stage);
    if(stage == 0){
        CacheSize = par("CacheSize");
        store = new NdnDaemon::ndStore;
        ndnDaemon = FindModule<NdnDaemon*>::findSubModule(findHost());
    }
    if(stage == 1){

    }
}

void NdnNetwLayer::handleSelfMsg(cMessage *msg)
{
    if(msg->getKind() == NDN_NETW_SELF_MESSAGE){

    } else{
        opp_error("recheck configuration of network module");
    }
}

void NdnNetwLayer::handleLowerMsg(cMessage* msg)
{
    int csBloomFilterMap[CacheSize];
    int pitBloomFilterMap[CacheSize];
    NdnNetwPkt* pkt = check_and_cast<NdnNetwPkt*>(msg);
    cObject* cInfo = pkt->removeControlInfo();
    // do something with cInfo
    delete cInfo;
    //extractBloomFilter(pkt, pitBloomFilterMap,csBloomFilterMap);

    sendUp(decapMsg(pkt));
}

void NdnNetwLayer::handleUpperMsg(cMessage* msg)
{
    sendDown(encapMsg(msg));
}

cMessage* NdnNetwLayer::decapMsg(NdnNetwPkt* msg)
{
    cMessage* m = msg->decapsulate();
    //m->setControlInfo(msg->getDestAddr());
    delete msg;
    return m;
}

NdnNetwPkt* NdnNetwLayer::encapMsg(cMessage* msg)
{
    uint32_t pitBloomFilter[4];
    uint32_t csBloomFilter[4];
    cPacket* m = static_cast<cPacket*>(msg);
    NdnNetwPkt* pkt = new NdnNetwPkt(msg->getName(),msg->getKind());
    //getBloomFilter(pkt, pitBloomFilter, csBloomFilter);
    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(LAddress::L3BROADCAST);
    pkt->setCreatorAddr(myNetwAddr);

    pkt->encapsulate(m);
    return pkt;
}


