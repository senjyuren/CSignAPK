#pragma once

#ifndef CSIGNAPK_UTILSX509ADAPTER_HPP
#define CSIGNAPK_UTILSX509ADAPTER_HPP

#include "UtilsX509Version.hpp"
#include "UtilsX509RSAPadding.hpp"

#include "UtilsX509SignI.hpp"
#include "UtilsX509I.hpp"
#include "UtilsX509ReqI.hpp"

#include "UtilsX509WithOpenSSL.hpp"
#include "UtilsX509ReqWithOpenSSL.hpp"
#include "UtilsX509SignWithOpenSSL.hpp"

namespace m
{

inline namespace apk
{

class UtilsX509Adapter
{
public:
    class Builder;

private:
    Builder *mBuilder;

    UtilsX509I     *mX509Ctx;
    UtilsX509ReqI  *mX509Req;
    UtilsX509SignI *mX509CtxSign;
    UtilsX509SignI *mX509ReqSign;

    std::string mX509CtxPEM;
    std::string mX509ReqPEM;

public:
    class Builder
    {
    private:
        std::string mCountry;
        std::string mProvince;
        std::string mCity;
        std::string mOrganization;
        std::string mOrganizationUnit;
        std::string mCommon;
        std::string mEmail;

        Jlong            mExpDate;
        UtilsX509Version mVersion;

        std::string mPubKey;
        std::string mPriKey;
        std::string mCAPriKey;
        std::string mCA;

        UtilsX509I     *mX509Ctx;
        UtilsX509ReqI  *mX509Req;
        UtilsX509SignI *mX509CtxSign;
        UtilsX509SignI *mX509ReqSign;

    public:
        friend UtilsX509Adapter;

        Builder();

        Builder &setCountry(const Jchar *v);

        Builder &setProvince(const Jchar *v);

        Builder &setCity(const Jchar *v);

        Builder &setOrganization(const Jchar *v);

        Builder &setOrganizationUnit(const Jchar *v);

        Builder &setCommon(const Jchar *v);

        Builder &setEmail(const Jchar *v);

        Builder &setExpDate(Jlong v);

        Builder &setVersion(UtilsX509Version v);

        Builder &setPublicKey(const Jchar *v);

        Builder &setPrivateKey(const Jchar *v);

        Builder &setCAPrivateKey(const Jchar *v);

        Builder &setCA(const Jchar *v);

        Builder &setX509Ctx(UtilsX509I *v);

        Builder &setX509Req(UtilsX509ReqI *v);

        Builder &setX509CtxSign(UtilsX509SignI *v);

        Builder &setX509ReqSign(UtilsX509SignI *v);

        std::shared_ptr<UtilsX509Adapter> build();
    };

    explicit UtilsX509Adapter(Builder *builder);

    ~UtilsX509Adapter();

    const std::string &getCertREQ();

    const std::string &getCert();
};

UtilsX509Adapter::Builder::Builder()
        : mCountry{}
          , mProvince{}
          , mCity{}
          , mOrganization{}
          , mOrganizationUnit{}
          , mCommon{}
          , mEmail{}
          , mExpDate{}
          , mVersion{}
          , mPubKey{}
          , mPriKey{}
          , mCAPriKey{}
          , mCA{}
          , mX509Ctx{}
          , mX509Req{}
          , mX509CtxSign{}
          , mX509ReqSign{}
{
    this->mVersion = UtilsX509Version::V3;
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setCountry(const Jchar *v)
{
    this->mCountry.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setProvince(const Jchar *v)
{
    this->mProvince.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setCity(const Jchar *v)
{
    this->mCity.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setOrganization(const Jchar *v)
{
    this->mOrganization.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setOrganizationUnit(const Jchar *v)
{
    this->mOrganizationUnit.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setCommon(const Jchar *v)
{
    this->mCommon.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setEmail(const Jchar *v)
{
    this->mEmail.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setExpDate(Jlong v)
{
    this->mExpDate = v;
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setVersion(UtilsX509Version v)
{
    this->mVersion = v;
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setPublicKey(const Jchar *v)
{
    this->mPubKey.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setPrivateKey(const Jchar *v)
{
    this->mPriKey.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setCAPrivateKey(const Jchar *v)
{
    this->mCAPriKey.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setCA(const Jchar *v)
{
    this->mCA.append(v);
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setX509Ctx(UtilsX509I *v)
{
    this->mX509Ctx = v;
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setX509Req(UtilsX509ReqI *v)
{
    this->mX509Req = v;
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setX509CtxSign(UtilsX509SignI *v)
{
    this->mX509CtxSign = v;
    return (*this);
}

UtilsX509Adapter::Builder &UtilsX509Adapter::Builder::setX509ReqSign(UtilsX509SignI *v)
{
    this->mX509ReqSign = v;
    return (*this);
}

std::shared_ptr<UtilsX509Adapter> UtilsX509Adapter::Builder::build()
{
    return std::make_shared<UtilsX509Adapter>(this);
}

UtilsX509Adapter::UtilsX509Adapter(Builder *builder)
        : mBuilder{builder}
          , mX509Ctx{builder->mX509Ctx}
          , mX509Req{builder->mX509Req}
          , mX509CtxSign{builder->mX509CtxSign}
          , mX509ReqSign{builder->mX509ReqSign}
          , mX509CtxPEM{}
          , mX509ReqPEM{}
{
    if (this->mX509Ctx == nullptr)
        this->mX509Ctx = new UtilsX509WithOpenSSL();
    if (this->mX509Req == nullptr)
        this->mX509Req = new UtilsX509ReqWithOpenSSL();

    if (this->mX509CtxSign == nullptr)
        this->mX509CtxSign = new UtilsX509SignWithOpenSSL(builder->mCAPriKey.c_str());
    if (this->mX509ReqSign == nullptr)
        this->mX509ReqSign = new UtilsX509SignWithOpenSSL(builder->mPriKey.c_str());

    if (!this->mX509Req->x509ReqInit())
        return;

    this->mX509Req->x509ReqSetCountry(builder->mCountry.c_str())
            .x509ReqSetProvince(builder->mProvince.c_str())
            .x509ReqSetCity(builder->mCity.c_str())
            .x509ReqSetOranization(builder->mOrganization.c_str())
            .x509ReqSetOranizationUnit(builder->mOrganizationUnit.c_str())
            .x509ReqSetCommon(builder->mCommon.c_str())
            .x509ReqSetPublicKey(builder->mPubKey.c_str())
            .x509ReqSetEmail(builder->mEmail.c_str())
            .x509ReqSetVersion(builder->mVersion)
            .x509ReqSetSignObject(this->mX509ReqSign);

    if (!this->mX509Req->x509ReqEnd(this->mX509ReqPEM))
        return;
    if (!this->mX509Ctx->x509Init())
        return;

    this->mX509Ctx->x509SetVersion(builder->mVersion)
            .x509SetRequest(this->mX509ReqPEM.c_str())
            .x509SetCA(builder->mCA.c_str())
            .x509SetExpDate(builder->mExpDate)
            .x509SetSignObject(this->mX509CtxSign);

    this->mX509Ctx->x509End(this->mX509CtxPEM);
}

UtilsX509Adapter::~UtilsX509Adapter()
{
    delete (this->mBuilder);
}

const std::string &UtilsX509Adapter::getCertREQ()
{
    return this->mX509ReqPEM;
}

const std::string &UtilsX509Adapter::getCert()
{
    return this->mX509CtxPEM;
}

}

}

#endif //CSIGNAPK_UTILSX509ADAPTER_HPP
