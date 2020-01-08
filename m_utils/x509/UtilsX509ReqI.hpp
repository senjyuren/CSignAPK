#pragma once

#ifndef CSIGNAPK_UTILSX509REQI_HPP
#define CSIGNAPK_UTILSX509REQI_HPP

namespace m
{

inline namespace apk
{

class UtilsX509ReqI
{
public:
    virtual ~UtilsX509ReqI();

    virtual Jbool x509ReqInit() = 0;

    virtual UtilsX509ReqI &x509ReqSetPublicKey(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetVersion(UtilsX509Version v) = 0;

    virtual UtilsX509ReqI &x509ReqSetCountry(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetProvince(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetCity(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetOranization(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetOranizationUnit(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetCommon(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetEmail(const Jchar *v) = 0;

    virtual UtilsX509ReqI &x509ReqSetSignObject(UtilsX509SignI *v) = 0;

    virtual Jbool x509ReqEnd(std::string &ret) = 0;
};

UtilsX509ReqI::~UtilsX509ReqI() = default;

}

}

#endif //CSIGNAPK_UTILSX509REQI_HPP
