//
// Generated file, do not edit! Created by opp_msgc 4.5 from NDNx/Messages/NdnAppPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "NdnAppPkt_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(NdnAppPkt);

NdnAppPkt::NdnAppPkt(const char *name, int kind) : ::ApplPkt(name,kind)
{
    this->maxNbHops_var = 0;
    this->nbHops_var = 0;
    this->msgId_var = 0;
    this->isExtendedPkt_var = 0;
    this->selfPitBFM1_var = 0;
    this->selfPitBFM2_var = 0;
    this->selfPitBFM3_var = 0;
    this->selfPitBFM4_var = 0;
    this->selfCsBFM1_var = 0;
    this->selfCsBFM2_var = 0;
    this->selfCsBFM3_var = 0;
    this->selfCsBFM4_var = 0;
    this->ohPitBFM1_var = 0;
    this->ohPitBFM2_var = 0;
    this->ohPitBFM3_var = 0;
    this->ohPitBFM4_var = 0;
    this->ohCsBFM1_var = 0;
    this->ohCsBFM2_var = 0;
    this->ohCsBFM3_var = 0;
    this->ohCsBFM4_var = 0;
    this->thPitBFM1_var = 0;
    this->thPitBFM2_var = 0;
    this->thPitBFM3_var = 0;
    this->thPitBFM4_var = 0;
    this->thCsBFM1_var = 0;
    this->thCsBFM2_var = 0;
    this->thCsBFM3_var = 0;
    this->thCsBFM4_var = 0;
}

NdnAppPkt::NdnAppPkt(const NdnAppPkt& other) : ::ApplPkt(other)
{
    copy(other);
}

NdnAppPkt::~NdnAppPkt()
{
}

NdnAppPkt& NdnAppPkt::operator=(const NdnAppPkt& other)
{
    if (this==&other) return *this;
    ::ApplPkt::operator=(other);
    copy(other);
    return *this;
}

void NdnAppPkt::copy(const NdnAppPkt& other)
{
    this->maxNbHops_var = other.maxNbHops_var;
    this->nbHops_var = other.nbHops_var;
    this->msgId_var = other.msgId_var;
    this->isExtendedPkt_var = other.isExtendedPkt_var;
    this->creatorAddr_var = other.creatorAddr_var;
    this->selfPitBFM1_var = other.selfPitBFM1_var;
    this->selfPitBFM2_var = other.selfPitBFM2_var;
    this->selfPitBFM3_var = other.selfPitBFM3_var;
    this->selfPitBFM4_var = other.selfPitBFM4_var;
    this->selfCsBFM1_var = other.selfCsBFM1_var;
    this->selfCsBFM2_var = other.selfCsBFM2_var;
    this->selfCsBFM3_var = other.selfCsBFM3_var;
    this->selfCsBFM4_var = other.selfCsBFM4_var;
    this->ohPitBFM1_var = other.ohPitBFM1_var;
    this->ohPitBFM2_var = other.ohPitBFM2_var;
    this->ohPitBFM3_var = other.ohPitBFM3_var;
    this->ohPitBFM4_var = other.ohPitBFM4_var;
    this->ohCsBFM1_var = other.ohCsBFM1_var;
    this->ohCsBFM2_var = other.ohCsBFM2_var;
    this->ohCsBFM3_var = other.ohCsBFM3_var;
    this->ohCsBFM4_var = other.ohCsBFM4_var;
    this->thPitBFM1_var = other.thPitBFM1_var;
    this->thPitBFM2_var = other.thPitBFM2_var;
    this->thPitBFM3_var = other.thPitBFM3_var;
    this->thPitBFM4_var = other.thPitBFM4_var;
    this->thCsBFM1_var = other.thCsBFM1_var;
    this->thCsBFM2_var = other.thCsBFM2_var;
    this->thCsBFM3_var = other.thCsBFM3_var;
    this->thCsBFM4_var = other.thCsBFM4_var;
}

void NdnAppPkt::parsimPack(cCommBuffer *b)
{
    ::ApplPkt::parsimPack(b);
    doPacking(b,this->maxNbHops_var);
    doPacking(b,this->nbHops_var);
    doPacking(b,this->msgId_var);
    doPacking(b,this->isExtendedPkt_var);
    doPacking(b,this->creatorAddr_var);
    doPacking(b,this->selfPitBFM1_var);
    doPacking(b,this->selfPitBFM2_var);
    doPacking(b,this->selfPitBFM3_var);
    doPacking(b,this->selfPitBFM4_var);
    doPacking(b,this->selfCsBFM1_var);
    doPacking(b,this->selfCsBFM2_var);
    doPacking(b,this->selfCsBFM3_var);
    doPacking(b,this->selfCsBFM4_var);
    doPacking(b,this->ohPitBFM1_var);
    doPacking(b,this->ohPitBFM2_var);
    doPacking(b,this->ohPitBFM3_var);
    doPacking(b,this->ohPitBFM4_var);
    doPacking(b,this->ohCsBFM1_var);
    doPacking(b,this->ohCsBFM2_var);
    doPacking(b,this->ohCsBFM3_var);
    doPacking(b,this->ohCsBFM4_var);
    doPacking(b,this->thPitBFM1_var);
    doPacking(b,this->thPitBFM2_var);
    doPacking(b,this->thPitBFM3_var);
    doPacking(b,this->thPitBFM4_var);
    doPacking(b,this->thCsBFM1_var);
    doPacking(b,this->thCsBFM2_var);
    doPacking(b,this->thCsBFM3_var);
    doPacking(b,this->thCsBFM4_var);
}

void NdnAppPkt::parsimUnpack(cCommBuffer *b)
{
    ::ApplPkt::parsimUnpack(b);
    doUnpacking(b,this->maxNbHops_var);
    doUnpacking(b,this->nbHops_var);
    doUnpacking(b,this->msgId_var);
    doUnpacking(b,this->isExtendedPkt_var);
    doUnpacking(b,this->creatorAddr_var);
    doUnpacking(b,this->selfPitBFM1_var);
    doUnpacking(b,this->selfPitBFM2_var);
    doUnpacking(b,this->selfPitBFM3_var);
    doUnpacking(b,this->selfPitBFM4_var);
    doUnpacking(b,this->selfCsBFM1_var);
    doUnpacking(b,this->selfCsBFM2_var);
    doUnpacking(b,this->selfCsBFM3_var);
    doUnpacking(b,this->selfCsBFM4_var);
    doUnpacking(b,this->ohPitBFM1_var);
    doUnpacking(b,this->ohPitBFM2_var);
    doUnpacking(b,this->ohPitBFM3_var);
    doUnpacking(b,this->ohPitBFM4_var);
    doUnpacking(b,this->ohCsBFM1_var);
    doUnpacking(b,this->ohCsBFM2_var);
    doUnpacking(b,this->ohCsBFM3_var);
    doUnpacking(b,this->ohCsBFM4_var);
    doUnpacking(b,this->thPitBFM1_var);
    doUnpacking(b,this->thPitBFM2_var);
    doUnpacking(b,this->thPitBFM3_var);
    doUnpacking(b,this->thPitBFM4_var);
    doUnpacking(b,this->thCsBFM1_var);
    doUnpacking(b,this->thCsBFM2_var);
    doUnpacking(b,this->thCsBFM3_var);
    doUnpacking(b,this->thCsBFM4_var);
}

int NdnAppPkt::getMaxNbHops() const
{
    return maxNbHops_var;
}

void NdnAppPkt::setMaxNbHops(int maxNbHops)
{
    this->maxNbHops_var = maxNbHops;
}

int NdnAppPkt::getNbHops() const
{
    return nbHops_var;
}

void NdnAppPkt::setNbHops(int nbHops)
{
    this->nbHops_var = nbHops;
}

int NdnAppPkt::getMsgId() const
{
    return msgId_var;
}

void NdnAppPkt::setMsgId(int msgId)
{
    this->msgId_var = msgId;
}

int NdnAppPkt::getIsExtendedPkt() const
{
    return isExtendedPkt_var;
}

void NdnAppPkt::setIsExtendedPkt(int isExtendedPkt)
{
    this->isExtendedPkt_var = isExtendedPkt;
}

LAddress::L3Type& NdnAppPkt::getCreatorAddr()
{
    return creatorAddr_var;
}

void NdnAppPkt::setCreatorAddr(const LAddress::L3Type& creatorAddr)
{
    this->creatorAddr_var = creatorAddr;
}

unsigned int NdnAppPkt::getSelfPitBFM1() const
{
    return selfPitBFM1_var;
}

void NdnAppPkt::setSelfPitBFM1(unsigned int selfPitBFM1)
{
    this->selfPitBFM1_var = selfPitBFM1;
}

unsigned int NdnAppPkt::getSelfPitBFM2() const
{
    return selfPitBFM2_var;
}

void NdnAppPkt::setSelfPitBFM2(unsigned int selfPitBFM2)
{
    this->selfPitBFM2_var = selfPitBFM2;
}

unsigned int NdnAppPkt::getSelfPitBFM3() const
{
    return selfPitBFM3_var;
}

void NdnAppPkt::setSelfPitBFM3(unsigned int selfPitBFM3)
{
    this->selfPitBFM3_var = selfPitBFM3;
}

unsigned int NdnAppPkt::getSelfPitBFM4() const
{
    return selfPitBFM4_var;
}

void NdnAppPkt::setSelfPitBFM4(unsigned int selfPitBFM4)
{
    this->selfPitBFM4_var = selfPitBFM4;
}

unsigned int NdnAppPkt::getSelfCsBFM1() const
{
    return selfCsBFM1_var;
}

void NdnAppPkt::setSelfCsBFM1(unsigned int selfCsBFM1)
{
    this->selfCsBFM1_var = selfCsBFM1;
}

unsigned int NdnAppPkt::getSelfCsBFM2() const
{
    return selfCsBFM2_var;
}

void NdnAppPkt::setSelfCsBFM2(unsigned int selfCsBFM2)
{
    this->selfCsBFM2_var = selfCsBFM2;
}

unsigned int NdnAppPkt::getSelfCsBFM3() const
{
    return selfCsBFM3_var;
}

void NdnAppPkt::setSelfCsBFM3(unsigned int selfCsBFM3)
{
    this->selfCsBFM3_var = selfCsBFM3;
}

unsigned int NdnAppPkt::getSelfCsBFM4() const
{
    return selfCsBFM4_var;
}

void NdnAppPkt::setSelfCsBFM4(unsigned int selfCsBFM4)
{
    this->selfCsBFM4_var = selfCsBFM4;
}

unsigned int NdnAppPkt::getOhPitBFM1() const
{
    return ohPitBFM1_var;
}

void NdnAppPkt::setOhPitBFM1(unsigned int ohPitBFM1)
{
    this->ohPitBFM1_var = ohPitBFM1;
}

unsigned int NdnAppPkt::getOhPitBFM2() const
{
    return ohPitBFM2_var;
}

void NdnAppPkt::setOhPitBFM2(unsigned int ohPitBFM2)
{
    this->ohPitBFM2_var = ohPitBFM2;
}

unsigned int NdnAppPkt::getOhPitBFM3() const
{
    return ohPitBFM3_var;
}

void NdnAppPkt::setOhPitBFM3(unsigned int ohPitBFM3)
{
    this->ohPitBFM3_var = ohPitBFM3;
}

unsigned int NdnAppPkt::getOhPitBFM4() const
{
    return ohPitBFM4_var;
}

void NdnAppPkt::setOhPitBFM4(unsigned int ohPitBFM4)
{
    this->ohPitBFM4_var = ohPitBFM4;
}

unsigned int NdnAppPkt::getOhCsBFM1() const
{
    return ohCsBFM1_var;
}

void NdnAppPkt::setOhCsBFM1(unsigned int ohCsBFM1)
{
    this->ohCsBFM1_var = ohCsBFM1;
}

unsigned int NdnAppPkt::getOhCsBFM2() const
{
    return ohCsBFM2_var;
}

void NdnAppPkt::setOhCsBFM2(unsigned int ohCsBFM2)
{
    this->ohCsBFM2_var = ohCsBFM2;
}

unsigned int NdnAppPkt::getOhCsBFM3() const
{
    return ohCsBFM3_var;
}

void NdnAppPkt::setOhCsBFM3(unsigned int ohCsBFM3)
{
    this->ohCsBFM3_var = ohCsBFM3;
}

unsigned int NdnAppPkt::getOhCsBFM4() const
{
    return ohCsBFM4_var;
}

void NdnAppPkt::setOhCsBFM4(unsigned int ohCsBFM4)
{
    this->ohCsBFM4_var = ohCsBFM4;
}

unsigned int NdnAppPkt::getThPitBFM1() const
{
    return thPitBFM1_var;
}

void NdnAppPkt::setThPitBFM1(unsigned int thPitBFM1)
{
    this->thPitBFM1_var = thPitBFM1;
}

unsigned int NdnAppPkt::getThPitBFM2() const
{
    return thPitBFM2_var;
}

void NdnAppPkt::setThPitBFM2(unsigned int thPitBFM2)
{
    this->thPitBFM2_var = thPitBFM2;
}

unsigned int NdnAppPkt::getThPitBFM3() const
{
    return thPitBFM3_var;
}

void NdnAppPkt::setThPitBFM3(unsigned int thPitBFM3)
{
    this->thPitBFM3_var = thPitBFM3;
}

unsigned int NdnAppPkt::getThPitBFM4() const
{
    return thPitBFM4_var;
}

void NdnAppPkt::setThPitBFM4(unsigned int thPitBFM4)
{
    this->thPitBFM4_var = thPitBFM4;
}

unsigned int NdnAppPkt::getThCsBFM1() const
{
    return thCsBFM1_var;
}

void NdnAppPkt::setThCsBFM1(unsigned int thCsBFM1)
{
    this->thCsBFM1_var = thCsBFM1;
}

unsigned int NdnAppPkt::getThCsBFM2() const
{
    return thCsBFM2_var;
}

void NdnAppPkt::setThCsBFM2(unsigned int thCsBFM2)
{
    this->thCsBFM2_var = thCsBFM2;
}

unsigned int NdnAppPkt::getThCsBFM3() const
{
    return thCsBFM3_var;
}

void NdnAppPkt::setThCsBFM3(unsigned int thCsBFM3)
{
    this->thCsBFM3_var = thCsBFM3;
}

unsigned int NdnAppPkt::getThCsBFM4() const
{
    return thCsBFM4_var;
}

void NdnAppPkt::setThCsBFM4(unsigned int thCsBFM4)
{
    this->thCsBFM4_var = thCsBFM4;
}

class NdnAppPktDescriptor : public cClassDescriptor
{
  public:
    NdnAppPktDescriptor();
    virtual ~NdnAppPktDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NdnAppPktDescriptor);

NdnAppPktDescriptor::NdnAppPktDescriptor() : cClassDescriptor("NdnAppPkt", "ApplPkt")
{
}

NdnAppPktDescriptor::~NdnAppPktDescriptor()
{
}

bool NdnAppPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NdnAppPkt *>(obj)!=NULL;
}

const char *NdnAppPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NdnAppPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 29+basedesc->getFieldCount(object) : 29;
}

unsigned int NdnAppPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<29) ? fieldTypeFlags[field] : 0;
}

const char *NdnAppPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "maxNbHops",
        "nbHops",
        "msgId",
        "isExtendedPkt",
        "creatorAddr",
        "selfPitBFM1",
        "selfPitBFM2",
        "selfPitBFM3",
        "selfPitBFM4",
        "selfCsBFM1",
        "selfCsBFM2",
        "selfCsBFM3",
        "selfCsBFM4",
        "ohPitBFM1",
        "ohPitBFM2",
        "ohPitBFM3",
        "ohPitBFM4",
        "ohCsBFM1",
        "ohCsBFM2",
        "ohCsBFM3",
        "ohCsBFM4",
        "thPitBFM1",
        "thPitBFM2",
        "thPitBFM3",
        "thPitBFM4",
        "thCsBFM1",
        "thCsBFM2",
        "thCsBFM3",
        "thCsBFM4",
    };
    return (field>=0 && field<29) ? fieldNames[field] : NULL;
}

int NdnAppPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "maxNbHops")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nbHops")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgId")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "isExtendedPkt")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "creatorAddr")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfPitBFM1")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfPitBFM2")==0) return base+6;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfPitBFM3")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfPitBFM4")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfCsBFM1")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfCsBFM2")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfCsBFM3")==0) return base+11;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfCsBFM4")==0) return base+12;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohPitBFM1")==0) return base+13;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohPitBFM2")==0) return base+14;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohPitBFM3")==0) return base+15;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohPitBFM4")==0) return base+16;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohCsBFM1")==0) return base+17;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohCsBFM2")==0) return base+18;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohCsBFM3")==0) return base+19;
    if (fieldName[0]=='o' && strcmp(fieldName, "ohCsBFM4")==0) return base+20;
    if (fieldName[0]=='t' && strcmp(fieldName, "thPitBFM1")==0) return base+21;
    if (fieldName[0]=='t' && strcmp(fieldName, "thPitBFM2")==0) return base+22;
    if (fieldName[0]=='t' && strcmp(fieldName, "thPitBFM3")==0) return base+23;
    if (fieldName[0]=='t' && strcmp(fieldName, "thPitBFM4")==0) return base+24;
    if (fieldName[0]=='t' && strcmp(fieldName, "thCsBFM1")==0) return base+25;
    if (fieldName[0]=='t' && strcmp(fieldName, "thCsBFM2")==0) return base+26;
    if (fieldName[0]=='t' && strcmp(fieldName, "thCsBFM3")==0) return base+27;
    if (fieldName[0]=='t' && strcmp(fieldName, "thCsBFM4")==0) return base+28;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NdnAppPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "LAddress::L3Type",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<29) ? fieldTypeStrings[field] : NULL;
}

const char *NdnAppPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NdnAppPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NdnAppPkt *pp = (NdnAppPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NdnAppPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NdnAppPkt *pp = (NdnAppPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getMaxNbHops());
        case 1: return long2string(pp->getNbHops());
        case 2: return long2string(pp->getMsgId());
        case 3: return long2string(pp->getIsExtendedPkt());
        case 4: {std::stringstream out; out << pp->getCreatorAddr(); return out.str();}
        case 5: return ulong2string(pp->getSelfPitBFM1());
        case 6: return ulong2string(pp->getSelfPitBFM2());
        case 7: return ulong2string(pp->getSelfPitBFM3());
        case 8: return ulong2string(pp->getSelfPitBFM4());
        case 9: return ulong2string(pp->getSelfCsBFM1());
        case 10: return ulong2string(pp->getSelfCsBFM2());
        case 11: return ulong2string(pp->getSelfCsBFM3());
        case 12: return ulong2string(pp->getSelfCsBFM4());
        case 13: return ulong2string(pp->getOhPitBFM1());
        case 14: return ulong2string(pp->getOhPitBFM2());
        case 15: return ulong2string(pp->getOhPitBFM3());
        case 16: return ulong2string(pp->getOhPitBFM4());
        case 17: return ulong2string(pp->getOhCsBFM1());
        case 18: return ulong2string(pp->getOhCsBFM2());
        case 19: return ulong2string(pp->getOhCsBFM3());
        case 20: return ulong2string(pp->getOhCsBFM4());
        case 21: return ulong2string(pp->getThPitBFM1());
        case 22: return ulong2string(pp->getThPitBFM2());
        case 23: return ulong2string(pp->getThPitBFM3());
        case 24: return ulong2string(pp->getThPitBFM4());
        case 25: return ulong2string(pp->getThCsBFM1());
        case 26: return ulong2string(pp->getThCsBFM2());
        case 27: return ulong2string(pp->getThCsBFM3());
        case 28: return ulong2string(pp->getThCsBFM4());
        default: return "";
    }
}

bool NdnAppPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NdnAppPkt *pp = (NdnAppPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setMaxNbHops(string2long(value)); return true;
        case 1: pp->setNbHops(string2long(value)); return true;
        case 2: pp->setMsgId(string2long(value)); return true;
        case 3: pp->setIsExtendedPkt(string2long(value)); return true;
        case 5: pp->setSelfPitBFM1(string2ulong(value)); return true;
        case 6: pp->setSelfPitBFM2(string2ulong(value)); return true;
        case 7: pp->setSelfPitBFM3(string2ulong(value)); return true;
        case 8: pp->setSelfPitBFM4(string2ulong(value)); return true;
        case 9: pp->setSelfCsBFM1(string2ulong(value)); return true;
        case 10: pp->setSelfCsBFM2(string2ulong(value)); return true;
        case 11: pp->setSelfCsBFM3(string2ulong(value)); return true;
        case 12: pp->setSelfCsBFM4(string2ulong(value)); return true;
        case 13: pp->setOhPitBFM1(string2ulong(value)); return true;
        case 14: pp->setOhPitBFM2(string2ulong(value)); return true;
        case 15: pp->setOhPitBFM3(string2ulong(value)); return true;
        case 16: pp->setOhPitBFM4(string2ulong(value)); return true;
        case 17: pp->setOhCsBFM1(string2ulong(value)); return true;
        case 18: pp->setOhCsBFM2(string2ulong(value)); return true;
        case 19: pp->setOhCsBFM3(string2ulong(value)); return true;
        case 20: pp->setOhCsBFM4(string2ulong(value)); return true;
        case 21: pp->setThPitBFM1(string2ulong(value)); return true;
        case 22: pp->setThPitBFM2(string2ulong(value)); return true;
        case 23: pp->setThPitBFM3(string2ulong(value)); return true;
        case 24: pp->setThPitBFM4(string2ulong(value)); return true;
        case 25: pp->setThCsBFM1(string2ulong(value)); return true;
        case 26: pp->setThCsBFM2(string2ulong(value)); return true;
        case 27: pp->setThCsBFM3(string2ulong(value)); return true;
        case 28: pp->setThCsBFM4(string2ulong(value)); return true;
        default: return false;
    }
}

const char *NdnAppPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 4: return opp_typename(typeid(LAddress::L3Type));
        default: return NULL;
    };
}

void *NdnAppPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NdnAppPkt *pp = (NdnAppPkt *)object; (void)pp;
    switch (field) {
        case 4: return (void *)(&pp->getCreatorAddr()); break;
        default: return NULL;
    }
}


