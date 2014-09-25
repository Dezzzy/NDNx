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

#include "NdnApplLayer.h"
#include "FindModule.h"
#include "NetwControlInfo.h"

using std::make_pair;
using std::endl;

Define_Module(NdnApplLayer);

void NdnApplLayer::initialize(int stage)
{
    BaseApplLayer::initialize(stage);
    if(stage == 0){
        NodeType = par("NodeType");
        TargetWord = par("TargetWord");
        applicationSetupDelay = par("applicationSetupDelay");
        pktTTL = par("pktTimeToLive");
        cDaemon = FindModule<CacheDaemon*>::findSubModule(findHost());
        broadcastTimer = new cMessage("BROADCAST Timer",INTEREST_REBROADCAST_MESSAGE);
        startMsg = new cMessage("Start Message", BASE_NDN_START_MESSAGE);

    }

    if(stage == 0){
        scheduleAt(simTime() + applicationSetupDelay, startMsg);
    }
}

void NdnApplLayer::handleLowerMsg(NdnAppPkt* msg)
{
    NdnAppPkt* m = static_cast<NdnAppPkt*>(msg);
    if(isKnownMsg(m->getMsgId())){
        delete(m);
    }  else if(m->getDestAddr() != LAddress::L3BROADCAST || m->getDestAddr() != myApplAddr()){
        delete(m);
    }else {
        KnownMsgs.insert(m->getMsgId());
        ProcessMapData(m);
        if(msg->getKind() == BASE_NDN_INTEREST_MESSAGE){
            processInterestPkt(m);
        }else if(m->getKind() == BASE_NDN_DATA_MESSAGE){
            processDataPkt(m);
        }
    }
}

void NdnApplLayer::handleSelfMsg(cMessage* msg)
{
    int distance;
    if(msg->getKind() == BASE_NDN_START_MESSAGE){
        if(strcmp(NodeType, "Master")){
            //generateInterestPkt(TargetWord);
            //testApplicationLayer();
        }
        cancelEvent(startMsg);
    }else if(msg->getKind() == INTEREST_REBROADCAST_MESSAGE){
        cancelEvent(msg);
        popMsgInterestQueue(msg);
    }else{
        opp_error(" Wrong message type. recheck configuration");
    }
}

/*
 * sendNextMsg(...) generates the given type, this is the generic method used to set each field of the
 * Application Packet.
 */

void NdnApplLayer::sendNextMsg(const char* name, int pktType, LAddress::L3Type pktCreator,LAddress::L3Type destAddr, int propagationDistance, int hopNum, int msgId)
{
    NdnAppPkt* pkt = new NdnAppPkt(name, pktType);
    pkt->setMaxNbHops(propagationDistance);
    pkt->setNbHops(hopNum);
    pkt->setCreatorAddr(pktCreator);
    pkt->setSrcAddr(myApplAddr());
    pkt->setDestAddr(destAddr);
    pkt->setByteLength(headerLength);


    GenerateMapData(pkt);

    NetwControlInfo::setControlInfo(pkt, pkt->getDestAddr());

    sendDown(pkt);

}

/*
 * sendNextMsg(NdnAppPkt* pkt, LAddress::L3Type dAddr):
 * This is a replication of the generic sendNextMsg Method.
 * However the new destination address is added and all hop numbers are incremented or changed to the
 * appropriate values
 */

void NdnApplLayer::sendNextMsg(NdnAppPkt* pkt, LAddress::L3Type dAddr)
{
    NdnAppPkt* m;
    m = static_cast<NdnAppPkt*>(pkt->dup());
    //NdnAppPkt* m = new NdnAppPkt(pkt->getName(),pkt->getKind());
    m->setNbHops(pkt->getNbHops()+1);
    m->setDestAddr(dAddr);
    GenerateMapData(m);
    NetwControlInfo::setControlInfo(pkt, pkt->getSrcAddr());

    sendDown(m);
}

void NdnApplLayer::sendNextMsg(NdnAppPkt* pkt)
{
    GenerateMapData(pkt);
    NetwControlInfo::setControlInfo(pkt, pkt->getSrcAddr());
    sendDown(pkt);
}

/*
 * generateInterestPkt(const char* name):
 * This is method used to generate new interest packets in conjunction with sendNextMsg
 * The interest is inserted into the Pending Interest Table with given name and
 * the sent
 */
void NdnApplLayer::generateInterestPkt(const char* name)
{
    int instructionStatus = 0;
    int hopDistance = 3;
    int propDistance = STANDARD_HOP;
    int msgId = uniform(0,1000);

    instructionStatus = cDaemon->generateInterestEntry(name, myApplAddr(), myApplAddr());

    if(instructionStatus == cDaemon->SEND_INTEREST){
        KnownMsgs.insert(msgId);
        cMessage* interestTimer = new cMessage(name,BASE_NDN_INTEREST_MESSAGE);
        reMsg* queueMsg = new reMsg;
        queueMsg->msg = interestTimer;
        queueMsg->nbResends = 0;
        queueMsg->msgId = msgId;
        insertMsgInterestQueue(queueMsg);

        NdnAppPkt* newInterestPkt = new NdnAppPkt(name, BASE_NDN_INTEREST_MESSAGE);
        newInterestPkt->setMsgId(msgId);
        newInterestPkt->setCreatorAddr(myApplAddr());
        newInterestPkt->setSrcAddr(myApplAddr());
        newInterestPkt->setDestAddr(LAddress::L3BROADCAST);
        newInterestPkt->setMaxNbHops(STANDARD_HOP);
        newInterestPkt->setNbHops(0);
        newInterestPkt->setByteLength(headerLength);

        sendNextMsg(newInterestPkt);
    }
}

/*
 * generateDataPkt(const char*,LAddress::L3Type, int hopDistance):
 *
 * This function generates a data packet and inserts it into the Content Store
 */
void NdnApplLayer::generateDataPkt(const char* name)
{
    int msgId = uniform(0,1000);

    NdnAppPkt* newApplPkt = new NdnAppPkt(name, BASE_NDN_DATA_MESSAGE);
    newApplPkt->setCreatorAddr(myApplAddr());
    newApplPkt->setSrcAddr(myApplAddr());
    newApplPkt->setMsgId(msgId);
    newApplPkt->setNbHops(0);

    int hopDistance;
    LAddress::L3Type intermediateDestAddress;
    cDaemon->getRoutingInfo(name,&hopDistance,&intermediateDestAddress);
    if(hopDistance>3){
        hopDistance = LONG_HOP;
    } else{
        hopDistance = STANDARD_HOP;
    }

    newApplPkt->setMaxNbHops(hopDistance);
    newApplPkt->setDestAddr(intermediateDestAddress);
    newApplPkt->setByteLength(headerLength);
    KnownMsgs.insert(msgId);
    sendNextMsg(newApplPkt);
}

/*
 * processInterestPkt(NdnAppPkt*):
 * This functions processes the interest packet and completes the necessary functions based on responses
 * the Pending Interest Table and Content Store
 * For full Methodology, see Name Data Protocol Documentation
 */
void NdnApplLayer::processInterestPkt(NdnAppPkt* pkt)
{
    int instructionStatus;
    int nbHops;
    int msgId;
    int hopDistance;

    nbHops = pkt->getNbHops() + 1;
    instructionStatus = cDaemon->processInterest(pkt->getName(), pkt->getCreatorAddr(), pkt->getSrcAddr(),nbHops);
    switch(instructionStatus){

    case CacheDaemon::SEND_DATA:
        generateDataPkt(pkt->getName());
        break;

    case CacheDaemon::SEND_INTEREST:
        sendInterestPkt(pkt);
        break;

    case CacheDaemon::DO_NOTHING:
        // old packet or useless data, either will be cached or deleted, decided by cache daemon or relevant cache entity
        break;
    }
}


/*
 * processDataPkt(NdnAppPkt*):
 * Similar to processInterestPkt, for data packets only
 * see protocol documentation for full description
 */
void NdnApplLayer::processDataPkt(NdnAppPkt* pkt)
{
    int instructionStatus;
    int nbHops;
    int msgId;
    int* hCount = 0;
    LAddress::L3Type* iAddr = new LAddress::L3Type;
    *iAddr = LAddress::L3NULL;

    nbHops = pkt->getNbHops() + 1;
    instructionStatus = cDaemon->processData(pkt->getName(),hCount,iAddr);
    if(instructionStatus == CacheDaemon::SEND_DATA || instructionStatus == CacheDaemon::SEND_DATA_INTEREST_FOUND){
        if(nbHops < pkt->getMaxNbHops()){

            if(*iAddr != LAddress::L3NULL){
                sendDataPkt(pkt);
            }
        } else{

        }
    }
    if(instructionStatus == CacheDaemon::INTEREST_FOUND ||instructionStatus == CacheDaemon::SEND_DATA_INTEREST_FOUND){
        deleteMsgInterestQueue(pkt->getName());
    }
}

void NdnApplLayer::sendInterestPkt(NdnAppPkt* oldPkt)
{
    NdnAppPkt* xGenApplPkt = new NdnAppPkt(oldPkt->getName(),BASE_NDN_INTEREST_MESSAGE);
    int nbHops = oldPkt->getNbHops() + 1;

    if(nbHops >= oldPkt->getMaxNbHops()){
        if(oldPkt->getIsExtendedPkt()){
            delete oldPkt;
            delete xGenApplPkt;
        } else{
            xGenApplPkt->setIsExtendedPkt(1);
        }
    }

    if(xGenApplPkt != NULL){
        xGenApplPkt->setCreatorAddr(oldPkt->getCreatorAddr());
        xGenApplPkt->setSrcAddr(myApplAddr());
        xGenApplPkt->setNbHops(nbHops);
        if(xGenApplPkt->getIsExtendedPkt()){
            xGenApplPkt->setMaxNbHops(LONG_HOP);
        } else{
            xGenApplPkt->setMaxNbHops(STANDARD_HOP);
        }
        xGenApplPkt->setMsgId(oldPkt->getMsgId());

        xGenApplPkt->setDestAddr(LAddress::L3BROADCAST);
        sendNextMsg(xGenApplPkt);
    }
}

void NdnApplLayer::sendDataPkt(NdnAppPkt* oldPkt)
{
    NdnAppPkt* xGenAppPkt = new NdnAppPkt(oldPkt->getName(),BASE_NDN_DATA_MESSAGE);
    int nbHops = oldPkt->getNbHops() + 1;
    if(nbHops >= oldPkt->getMaxNbHops()){
        delete oldPkt;
        delete xGenAppPkt;

    } else{
        xGenAppPkt->setCreatorAddr(oldPkt->getCreatorAddr());
        xGenAppPkt->setSrcAddr(myApplAddr());
        xGenAppPkt->setNbHops(nbHops);
        xGenAppPkt->setMaxNbHops(oldPkt->getMaxNbHops());
        xGenAppPkt->setMsgId(oldPkt->getMsgId());

        int hopDistance;
        LAddress::L3Type interfaceAddress;
        cDaemon->getRoutingInfo(oldPkt->getName(),&hopDistance,&interfaceAddress);
        if(interfaceAddress != LAddress::L3NULL){
            xGenAppPkt->setDestAddr(interfaceAddress);
            delete oldPkt;
            sendNextMsg(xGenAppPkt);
        } else{
            delete xGenAppPkt;
            delete oldPkt;
        }
    }
}

/*
 * processMapData(NdnAppPkt*):
 * This function strips incoming packets of the attached name and name space data, which is used in
 * mapping the used name space in the network.
 */
void NdnApplLayer::ProcessMapData(NdnAppPkt* msg)
{
    uint32_t** oneHopList;
    uint32_t** twoHopList;
    uint32_t** threeHopList;
    uint32_t** xHopList;

    oneHopList = new uint32_t*[2];
    twoHopList = new uint32_t*[2];
    threeHopList = new uint32_t*[2];
    xHopList = new uint32_t*[2];
    for(int i = 0; i < 2;i++){
        oneHopList[i] = new uint32_t[4];
        twoHopList[i] = new uint32_t[4];
        threeHopList[i] = new uint32_t[4];
        xHopList[i] = new uint32_t[4];
    }

    if(msg->getKind() == BASE_NDN_INTEREST_MESSAGE){
        cDaemon->updateInterfaceList(msg->getName(), msg->getNbHops(),msg->getSrcAddr());
    }


    //get one hop list from message
    oneHopList[0][0] = msg->getSelfPitBFM1();
    oneHopList[0][1] = msg->getSelfPitBFM2();
    oneHopList[0][2] = msg->getSelfPitBFM3();
    oneHopList[0][3] = msg->getSelfPitBFM4();

    oneHopList[1][0] = msg->getSelfCsBFM1();
    oneHopList[1][1] = msg->getSelfCsBFM2();
    oneHopList[1][2] = msg->getSelfCsBFM3();
    oneHopList[1][3] = msg->getSelfCsBFM4();

    // get two hop list
    twoHopList[0][0] = msg->getOhPitBFM1();
    twoHopList[0][1] = msg->getOhPitBFM2();
    twoHopList[0][2] = msg->getOhPitBFM3();
    twoHopList[0][3] = msg->getOhPitBFM4();

    twoHopList[1][0] = msg->getOhCsBFM1();
    twoHopList[1][1] = msg->getOhCsBFM2();
    twoHopList[1][2] = msg->getOhCsBFM3();
    twoHopList[1][3] = msg->getOhCsBFM4();

    //get three hop list
    threeHopList[0][0] = msg->getThPitBFM1();
    threeHopList[0][1] = msg->getThPitBFM2();
    threeHopList[0][2] = msg->getThPitBFM3();
    threeHopList[0][3] = msg->getThPitBFM4();

    threeHopList[1][0] = msg->getThCsBFM1();
    threeHopList[1][1] = msg->getThCsBFM2();
    threeHopList[1][2] = msg->getThCsBFM3();
    threeHopList[1][3] = msg->getThCsBFM4();


    cDaemon->updateForwardingInfoBase(msg->getSrcAddr(), oneHopList, twoHopList, threeHopList,xHopList);
}

/*
 * GenerateMapData(NdnAppPkt*):
 * Generates the map data needed to sent along the interest/data packet
 * The data is generated and attached to the packet and then sent out
 */
void NdnApplLayer::GenerateMapData(NdnAppPkt* msg)
{
    uint32_t** oneHopList;
    uint32_t** twoHopList;
    uint32_t** threeHopList;
    uint32_t** xHopList;

    oneHopList = new uint32_t*[2];
    twoHopList = new uint32_t*[2];
    threeHopList = new uint32_t*[2];
    xHopList = new uint32_t*[2];
    for(int i = 0; i < 2;i++){
        oneHopList[i] = new uint32_t[4];
        twoHopList[i] = new uint32_t[4];
        threeHopList[i] = new uint32_t[4];
        xHopList[i] = new uint32_t[4];
    }

    cDaemon->generatePacketMaps(oneHopList,twoHopList,threeHopList,xHopList);

    // set one hop fields of Bloomfilter Map
    msg->setSelfPitBFM1(oneHopList[0][0]);
    msg->setSelfPitBFM1(oneHopList[0][1]);
    msg->setSelfPitBFM1(oneHopList[0][2]);
    msg->setSelfPitBFM1(oneHopList[0][3]);

    msg->setSelfCsBFM1(oneHopList[1][0]);
    msg->setSelfCsBFM1(oneHopList[1][1]);
    msg->setSelfCsBFM1(oneHopList[1][2]);
    msg->setSelfCsBFM1(oneHopList[1][3]);

    // set two hop fields
    msg->setOhPitBFM1(twoHopList[0][0]);
    msg->setOhPitBFM2(twoHopList[0][1]);
    msg->setOhPitBFM3(twoHopList[0][2]);
    msg->setOhPitBFM4(twoHopList[0][3]);

    msg->setOhCsBFM1(twoHopList[1][0]);
    msg->setOhCsBFM2(twoHopList[1][1]);
    msg->setOhCsBFM3(twoHopList[1][2]);
    msg->setOhCsBFM4(twoHopList[1][3]);


    //set three hop fields
    msg->setThPitBFM1(threeHopList[0][0]);
    msg->setThPitBFM2(threeHopList[0][0]);
    msg->setThPitBFM3(threeHopList[0][0]);
    msg->setThPitBFM4(threeHopList[0][0]);

    msg->setThCsBFM1(threeHopList[0][0]);
    msg->setThCsBFM2(threeHopList[0][0]);
    msg->setThCsBFM3(threeHopList[0][0]);
    msg->setThCsBFM4(threeHopList[0][0]);
}

/*
 * isKnownMsg(int):
 * function used to determine whether packet has been received before
 */
bool NdnApplLayer::isKnownMsg(int msgId)
{
    MsgIdSet::iterator it = KnownMsgs.find(msgId);

    return it != KnownMsgs.end();
}

/*
 * popMsgInterestQueue(cMessage*):
 * pops the interest of Interest Queue that must be resent
 */
void NdnApplLayer::popMsgInterestQueue(cMessage* msg)
{
    SentMsgs::iterator it = InterestMsgs.begin();

    if(!InterestMsgs.empty()){
        reMsg* qMsg = it->second;
        //scheduleAt(uniform(0,30), it->second->msg);
        InterestMsgs.erase(it);
        if(it->second->nbResends < 2){
            it->second->nbResends++;
            insertMsgInterestQueue(qMsg);
            //sendNextMsg(qMsg->msg->getName(), BASE_NDN_INTEREST_MESSAGE,myApplAddr(),LAddress::L3BROADCAST, LONG_HOP, 0,qMsg->msgId);
            EV<<"popping msg from interest queue"<<endl;
        }

        if(!InterestMsgs.empty()){
            scheduleAt(simTime() + pktTTL, msg);
        }
    }

}

/*
 * deleteMsgInterestQueue(const char*):
 * deletes interest from Interest Queue, either due to having reached end of life or
 * or the connected data packet was received
 */
void NdnApplLayer::deleteMsgInterestQueue(const char* name)
{
    SentMsgs::iterator it = InterestMsgs.find(name);

    if(it != InterestMsgs.end()){
        InterestMsgs.erase(it);
        if(InterestMsgs.empty()){
            cancelEvent(broadcastTimer);
        }
    }
}

/*
 * insertMsgInterestQueue(reMsg*):
 * upon interest packet generation, a duplicate packet is created and placed in the Interest Queue
 * for resending purposes if TTL on data packet reception is exceeded
 */
void NdnApplLayer::insertMsgInterestQueue(reMsg* qMsg)
{
    if(InterestMsgs.empty()){
        scheduleAt(simTime() + (pktTTL*2), broadcastTimer);
    }
    SentMsgs::value_type intPair = make_pair(qMsg->msg->getName(), qMsg);
    InterestMsgs.insert(intPair);
}

/*
 * Application Layer Test Functions
 */

void NdnApplLayer::testApplicationLayer()
{
    // test interest msg queue
    reMsg* testQMsg = new reMsg;
    int msgId = uniform(0,1000);
    int resend = 0;
    cMessage* testMsg = new cMessage("test-interest-queue-msg",BASE_NDN_INTEREST_MESSAGE);
    testQMsg->msg = testMsg;
    testQMsg->msgId = msgId;
    testQMsg->nbResends = resend;

    insertMsgInterestQueue(testQMsg);

    EV<<"interest Queue testing complete"<<endl;



}
