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

}

void NdnNetwLayer::handleUpperMsg(cMessage* msg)
{
    //sendDown(encapMsg(msg));
}

cMessage* NdnNetwLayer::decapMsg(cMessage* msg)
{

}

cMessage* NdnNetwLayer::encapMsg(cMessage* msg)
{

}


