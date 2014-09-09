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
        timeToLive = par("timeToLive");

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
    NdnNetPkt* netPkt = check_and_cast<NdnNetPkt*>(msg);
    NdnNetPkt* mCopy = static_cast<NdnNetPkt*>(netPkt->dup());
    sendUp(decapMsg(mCopy));
}

void NdnNetwLayer::handleUpperMsg(cMessage* msg)
{
    NdnNetPkt* netPkt;
    netPkt = encapMsg(msg);

    sendDown(netPkt);
}

cMessage* NdnNetwLayer::decapMsg(NdnNetPkt* msg)
{
    cMessage* decappedPkt = msg->decapsulate();
    //decappedPkt->setControlInfo(new ProbBcastNetwControlInfo(msg->getSrcAddr()));
    delete msg;
    return decappedPkt;
}

NdnNetPkt* NdnNetwLayer::encapMsg(cMessage* msg)
{
    NdnNetPkt* encappedPkt = new NdnNetPkt(msg->getName(),getNetMsgKind(msg->getKind()));

    encappedPkt->setByteLength(headerLength);
    encappedPkt->setSrcAddr(myNetwAddr);
    encappedPkt->setDestAddr(LAddress::L3BROADCAST);
    encappedPkt->setCreatorAddr(myNetwAddr);

    cPacket* m = static_cast<cPacket*>(msg);
    encappedPkt->encapsulate(m);


    return encappedPkt;
}

int NdnNetwLayer::getNetMsgKind(int applMsgKind)
{
    int convertedMsgKind;
    if(applMsgKind == 25251){
        convertedMsgKind = NDN_NETW_DATA_MESSAGE;
    } else if(applMsgKind == 25252){
        convertedMsgKind = NDN_NETW_INTEREST_MESSAGE;
    } else{
        opp_error("configuration error, message sent from application layer was not DATA/INTEREREST");
    }

    return convertedMsgKind;
}

