//
// Generated file, do not edit! Created by opp_msgc 4.3 from NDNx/Messages/NdnNetwPkt.msg.
//

#ifndef _NDNNETWPKT_M_H_
#define _NDNNETWPKT_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0403
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "NetwPkt_m.h"
// }}



/**
 * Class generated from <tt>NDNx/Messages/NdnNetwPkt.msg</tt> by opp_msgc.
 * <pre>
 * packet NdnNetwPkt extends NetwPkt {
 *         LAddress::L3Type creatorAddr;
 *         unsigned int pitBloomFilterMap1;
 *         unsigned int pitBloomFilterMap2;
 *         unsigned int pitBloomFilterMap3;
 *         unsigned int pitBloomFilterMap4;
 *         
 *         unsigned int csBloomFilterMap1;
 *         unsigned int csBloomFilterMap2;
 *         unsigned int csBloomFilterMap3;
 *         unsigned int csBloomFilterMap4;
 * }
 * </pre>
 */
class NdnNetwPkt : public ::NetwPkt
{
  protected:
    LAddress::L3Type creatorAddr_var;
    unsigned int pitBloomFilterMap1_var;
    unsigned int pitBloomFilterMap2_var;
    unsigned int pitBloomFilterMap3_var;
    unsigned int pitBloomFilterMap4_var;
    unsigned int csBloomFilterMap1_var;
    unsigned int csBloomFilterMap2_var;
    unsigned int csBloomFilterMap3_var;
    unsigned int csBloomFilterMap4_var;

  private:
    void copy(const NdnNetwPkt& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NdnNetwPkt&);

  public:
    NdnNetwPkt(const char *name=NULL, int kind=0);
    NdnNetwPkt(const NdnNetwPkt& other);
    virtual ~NdnNetwPkt();
    NdnNetwPkt& operator=(const NdnNetwPkt& other);
    virtual NdnNetwPkt *dup() const {return new NdnNetwPkt(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual LAddress::L3Type& getCreatorAddr();
    virtual const LAddress::L3Type& getCreatorAddr() const {return const_cast<NdnNetwPkt*>(this)->getCreatorAddr();}
    virtual void setCreatorAddr(const LAddress::L3Type& creatorAddr);
    virtual unsigned int getPitBloomFilterMap1() const;
    virtual void setPitBloomFilterMap1(unsigned int pitBloomFilterMap1);
    virtual unsigned int getPitBloomFilterMap2() const;
    virtual void setPitBloomFilterMap2(unsigned int pitBloomFilterMap2);
    virtual unsigned int getPitBloomFilterMap3() const;
    virtual void setPitBloomFilterMap3(unsigned int pitBloomFilterMap3);
    virtual unsigned int getPitBloomFilterMap4() const;
    virtual void setPitBloomFilterMap4(unsigned int pitBloomFilterMap4);
    virtual unsigned int getCsBloomFilterMap1() const;
    virtual void setCsBloomFilterMap1(unsigned int csBloomFilterMap1);
    virtual unsigned int getCsBloomFilterMap2() const;
    virtual void setCsBloomFilterMap2(unsigned int csBloomFilterMap2);
    virtual unsigned int getCsBloomFilterMap3() const;
    virtual void setCsBloomFilterMap3(unsigned int csBloomFilterMap3);
    virtual unsigned int getCsBloomFilterMap4() const;
    virtual void setCsBloomFilterMap4(unsigned int csBloomFilterMap4);
};

inline void doPacking(cCommBuffer *b, NdnNetwPkt& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, NdnNetwPkt& obj) {obj.parsimUnpack(b);}


#endif // _NDNNETWPKT_M_H_