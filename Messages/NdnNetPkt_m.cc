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
}

void NdnNetPkt::parsimPack(cCommBuffer *b)
{
    ::NetwPkt::parsimPack(b);
    doPacking(b,this->creatorAddr_var);
}

void NdnNetPkt::parsimUnpack(cCommBuffer *b)
{
    ::NetwPkt::parsimUnpack(b);
    doUnpacking(b,this->creatorAddr_var);
}

LAddress::L3Type& NdnNetPkt::getCreatorAddr()
{
    return creatorAddr_var;
}

void NdnNetPkt::setCreatorAddr(const LAddress::L3Type& creatorAddr)
{
    this->creatorAddr_var = creatorAddr;
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
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
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
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
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
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int NdnNetPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "creatorAddr")==0) return base+0;
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
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
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


