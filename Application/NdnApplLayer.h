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

#ifndef __MIXIM_NDNAPPLLAYER_H_
#define __MIXIM_NDNAPPLLAYER_H_

#include <omnetpp.h>
#include "NdnDaemon.h"
#include "NdnAppPkt_m.h"
#include "BaseApplLayer.h"
#include "CacheDaemon.h"

#include <set>
#include <map>

#define STANDARD_HOP 3
#define MEDIUM_HOP 2
#define SHORT_HOP 1
#define LONG_HOP 6

class NdnAppPkt;

class NdnApplLayer : public BaseApplLayer
{
public:
    typedef struct reMsg{
        cMessage* msg;
        int nbResends;
        int msgId;
    };
    typedef std::set<int>MsgIdSet;
    typedef std::multimap<const char*, reMsg*>SentMsgs;



    enum BASE_NDN_MESSAGE_KINDS{
        BASE_NDN_DATA_MESSAGE = 25251,
        BASE_NDN_INTEREST_MESSAGE,
        BASE_NDN_START_MESSAGE,
        BASE_TEST_MESSAGE,
        BASE_NDN_MESSAGE_FOUND,
        BASE_NDN_CHECK_MESSAGE,
        INTEREST_REBROADCAST_MESSAGE
    };



    CacheDaemon* cDaemon;
    virtual void initialize(int stage);

protected:
    simtime_t applicationSetupDelay;
    simtime_t pktTTL;
    cMessage* startMsg;
    cMessage* broadcastTimer;
    const char* NodeType;
    const char* TargetWord;
    MsgIdSet KnownMsgs;
    SentMsgs InterestMsgs;

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleLowerMsg(NdnAppPkt *msg);
    virtual void handleLowerControl(cMessage *msg){
        error("no lower control");
    }
    virtual void handleUpperMsg(cMessage* msg){
        error("NdnApplLayer does not handle control messages");
    };
    virtual void handleUpperControl(cMessage* msg){
        error("NdnApplLayer does not handle control messages");
    };



    void ProcessMapData(NdnAppPkt* msg);
    void GenerateMapData(NdnAppPkt* msg);

    bool isKnownMsg(int msgId);
    void popMsgInterestQueue(cMessage* msg);
    void deleteMsgInterestQueue(const char* name);
    void insertMsgInterestQueue(reMsg* qMsg);

    // working function list
    //functions to be added
    void sendInterestPkt(NdnAppPkt* oldPkt);
    void sendDataPkt(NdnAppPkt* oldPkt);

    // functions to be modified
    void sendNextMsg(const char* name, int pktType, LAddress::L3Type pktCreator,LAddress::L3Type destAddr, int propagationDistance, int hopNum,int msgId);
    void sendNextMsg(NdnAppPkt* pkt, LAddress::L3Type dAddr);
    void sendNextMsg(NdnAppPkt* pkt);
    void generateInterestPkt(const char* name);
    void generateDataPkt(const char* name,LAddress::L3Type destAddr, int hopDistance);
    void processInterestPkt(NdnAppPkt* pkt);
    void processDataPkt(NdnAppPkt* pkt);

};

#endif
