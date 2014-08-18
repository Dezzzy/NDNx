//
// Generated file, do not edit! Created by opp_msgc 4.5 from NDNx/Messages/NdnNetPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "NdnNetPkt_m.h"

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

Register_Class(NdnNetPkt);

NdnNetPkt::NdnNetPkt(const char *name, int kind) : ::NetwPkt(name,kind)
{
    this->pitBloomFilterMap1_var = 0;
    this->pitBloomFilterMap2_var = 0;
    this->pitBloomFilterMap3_var = 0;
    this->pitBloomFilterMap4_var = 0;
    this->csBloomFilterMap1_var = 0;
    this->csBloomFilterMap2_var = 0;
    this->csBloomFilterMap3_var = 0;
    this->csBloomFilterMap4_var = 0;
}

NdnNetPkt::NdnNetPkt(const NdnNetPkt& other) : ::NetwPkt(other)
{
    copy(other);
}

NdnNetPkt::~NdnNetPkt()
{
}

NdnNetPkt& NdnNetPkt::operator=(const NdnNetPkt& other)
{
    if (this==&other) return *this;
    ::NetwPkt::operator=(other);
    copy(other);
    return *this;
}

void NdnNetPkt::copy(const NdnNetPkt& other)
{
    this->creatorAddr_var = other.creatorAddr_var;
    this->pitBloomFilterMap1_var = other.pitBloomFilterMap1_var;
    this->pitBloomFilterMap2_var = other.pitBloomFilterMap2_var;
    this->pitBloomFilterMap3_var = other.pitBloomFilterMap3_var;
    this->pitBloomFilterMap4_var = other.pitBloomFilterMap4_var;
    this->csBloomFilterMap1_var = other.csBloomFilterMap1_var;
    this->csBloomFilterMap2_var = other.csBloomFilterMap2_var;
    this->csBloomFilterMap3_var = other.csBloomFilterMap3_var;
    this->csBloomFilterMap4_var = other.csBloomFilterMap4_var;
}

void NdnNetPkt::parsimPack(cCommBuffer *b)
{
    ::NetwPkt::parsimPack(b);
    doPacking(b,this->creatorAddr_var);
    doPacking(b,this->pitBloomFilterMap1_var);
    doPacking(b,this->pitBloomFilterMap2_var);
    doPacking(b,this->pitBloomFilterMap3_var);
    doPacking(b,this->pitBloomFilterMap4_var);
    doPacking(b,this->csBloomFilterMap1_var);
    doPacking(b,this->csBloomFilterMap2_var);
    doPacking(b,this->csBloomFilterMap3_var);
    doPacking(b,this->csBloomFilterMap4_var);
}

void NdnNetPkt::parsimUnpack(cCommBuffer *b)
{
    ::NetwPkt::parsimUnpack(b);
    doUnpacking(b,this->creatorAddr_var);
    doUnpacking(b,this->pitBloomFilterMap1_var);
    doUnpacking(b,this->pitBloomFilterMap2_var);
    doUnpacking(b,this->pitBloomFilterMap3_var);
    doUnpacking(b,this->pitBloomFilterMap4_var);
    doUnpacking(b,this->csBloomFilterMap1_var);
    doUnpacking(b,this->csBloomFilterMap2_var);
    doUnpacking(b,this->csBloomFilterMap3_var);
    doUnpacking(b,this->csBloomFilterMap4_var);
}

LAddress::L3Type& NdnNetPkt::getCreatorAddr()
{
    return creatorAddr_var;
}

void NdnNetPkt::setCreatorAddr(const LAddress::L3Type& creatorAddr)
{
    this->creatorAddr_var = creatorAddr;
}

unsigned int NdnNetPkt::getPitBloomFilterMap1() const
{
    return pitBloomFilterMap1_var;
}

void NdnNetPkt::setPitBloomFilterMap1(unsigned int pitBloomFilterMap1)
{
    this->pitBloomFilterMap1_var = pitBloomFilterMap1;
}

unsigned int NdnNetPkt::getPitBloomFilterMap2() const
{
    return pitBloomFilterMap2_var;
}

void NdnNetPkt::setPitBloomFilterMap2(unsigned int pitBloomFilterMap2)
{
    this->pitBloomFilterMap2_var = pitBloomFilterMap2;
}

unsigned int NdnNetPkt::getPitBloomFilterMap3() const
{
    return pitBloomFilterMap3_var;
}

void NdnNetPkt::setPitBloomFilterMap3(unsigned int pitBloomFilterMap3)
{
    this->pitBloomFilterMap3_var = pitBloomFilterMap3;
}

unsigned int NdnNetPkt::getPitBloomFilterMap4() const
{
    return pitBloomFilterMap4_var;
}

void NdnNetPkt::setPitBloomFilterMap4(unsigned int pitBloomFilterMap4)
{
    this->pitBloomFilterMap4_var = pitBloomFilterMap4;
}

unsigned int NdnNetPkt::getCsBloomFilterMap1() const
{
    return csBloomFilterMap1_var;
}

void NdnNetPkt::setCsBloomFilterMap1(unsigned int csBloomFilterMap1)
{
    this->csBloomFilterMap1_var = csBloomFilterMap1;
}

unsigned int NdnNetPkt::getCsBloomFilterMap2() const
{
    return csBloomFilterMap2_var;
}

void NdnNetPkt::setCsBloomFilterMap2(unsigned int csBloomFilterMap2)
{
    this->csBloomFilterMap2_var = csBloomFilterMap2;
}

unsigned int NdnNetPkt::getCsBloomFilterMap3() const
{
    return csBloomFilterMap3_var;
}

void NdnNetPkt::setCsBloomFilterMap3(unsigned int csBloomFilterMap3)
{
    this->csBloomFilterMap3_var = csBloomFilterMap3;
}

unsigned int NdnNetPkt::getCsBloomFilterMap4() const
{
    return csBloomFilterMap4_var;
}

void NdnNetPkt::setCsBloomFilterMap4(unsigned int csBloomFilterMap4)
{
    this->csBloomFilterMap4_var = csBloomFilterMap4;
}

class NdnNetPktDescriptor : public cClassDescriptor
{
  public:
    NdnNetPktDescriptor();
    virtual ~NdnNetPktDescriptor();

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

Register_ClassDescriptor(NdnNetPktDescriptor);

NdnNetPktDescriptor::NdnNetPktDescriptor() : cClassDescriptor("NdnNetPkt", "NetwPkt")
{
}

NdnNetPktDescriptor::~NdnNetPktDescriptor()
{
}

bool NdnNetPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NdnNetPkt *>(obj)!=NULL;
}

const char *NdnNetPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NdnNetPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int NdnNetPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *NdnNetPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "creatorAddr",
        "pitBloomFilterMap1",
        "pitBloomFilterMap2",
        "pitBloomFilterMap3",
        "pitBloomFilterMap4",
        "csBloomFilterMap1",
        "csBloomFilterMap2",
        "csBloomFilterMap3",
        "csBloomFilterMap4",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int NdnNetPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "creatorAddr")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pitBloomFilterMap1")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pitBloomFilterMap2")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "pitBloomFilterMap3")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "pitBloomFilterMap4")==0) return base+4;
    if (fieldName[0]=='c' && strcmp(fieldName, "csBloomFilterMap1")==0) return base+5;
    if (fieldName[0]=='c' && strcmp(fieldName, "csBloomFilterMap2")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "csBloomFilterMap3")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "csBloomFilterMap4")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NdnNetPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "LAddress::L3Type",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *NdnNetPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NdnNetPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NdnNetPkt *pp = (NdnNetPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NdnNetPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NdnNetPkt *pp = (NdnNetPkt *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getCreatorAddr(); return out.str();}
        case 1: return ulong2string(pp->getPitBloomFilterMap1());
        case 2: return ulong2string(pp->getPitBloomFilterMap2());
        case 3: return ulong2string(pp->getPitBloomFilterMap3());
        case 4: return ulong2string(pp->getPitBloomFilterMap4());
        case 5: return ulong2string(pp->getCsBloomFilterMap1());
        case 6: return ulong2string(pp->getCsBloomFilterMap2());
        case 7: return ulong2string(pp->getCsBloomFilterMap3());
        case 8: return ulong2string(pp->getCsBloomFilterMap4());
        default: return "";
    }
}

bool NdnNetPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NdnNetPkt *pp = (NdnNetPkt *)object; (void)pp;
    switch (field) {
        case 1: pp->setPitBloomFilterMap1(string2ulong(value)); return true;
        case 2: pp->setPitBloomFilterMap2(string2ulong(value)); return true;
        case 3: pp->setPitBloomFilterMap3(string2ulong(value)); return true;
        case 4: pp->setPitBloomFilterMap4(string2ulong(value)); return true;
        case 5: pp->setCsBloomFilterMap1(string2ulong(value)); return true;
        case 6: pp->setCsBloomFilterMap2(string2ulong(value)); return true;
        case 7: pp->setCsBloomFilterMap3(string2ulong(value)); return true;
        case 8: pp->setCsBloomFilterMap4(string2ulong(value)); return true;
        default: return false;
    }
}

const char *NdnNetPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(LAddress::L3Type));
        default: return NULL;
    };
}

void *NdnNetPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NdnNetPkt *pp = (NdnNetPkt *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getCreatorAddr()); break;
        default: return NULL;
    }
}


