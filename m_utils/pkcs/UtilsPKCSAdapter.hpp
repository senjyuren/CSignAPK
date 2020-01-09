#pragma once

#ifndef CSIGNAPK_UTILSPKCSADAPTER_HPP
#define CSIGNAPK_UTILSPKCSADAPTER_HPP

#include "UtilsPKCSRSAPadding.hpp"

#include "UtilsPKCS7SignI.hpp"
#include "UtilsPKCSType7I.hpp"

#include "UtilsPKCSType7WithOpenSSL.hpp"
#include "UtilsPKCS7SignWithOpenSSL.hpp"

namespace m
{

inline namespace apk
{

class UtilsPKCSAdapter
{
public:
    class Builder;

private:
    Builder *mBuilder;

    UtilsPKCSType7I *mType7;
    UtilsPKCS7SignI *mSign7;

public:
    class Builder
    {
    private:
        std::string mKey;
        std::string mCert;

        UtilsPKCSType7I *mType7;
        UtilsPKCS7SignI *mSign7;

    public:
        friend UtilsPKCSAdapter;

        Builder();

        Builder &setKey(const Jchar *key);

        Builder &setCert(const Jchar *cert);

        Builder &setPKCS7(UtilsPKCSType7I *v);

        Builder &setPKCS7Sign(UtilsPKCS7SignI *v);

        std::shared_ptr<UtilsPKCSAdapter> build();
    };

    explicit UtilsPKCSAdapter(Builder *builder);

    ~UtilsPKCSAdapter();

    Jbool init();

    Jbool update(const Jbyte *v, Jint vLen);

    Jbool final(std::vector<Jbyte> &v);
};

UtilsPKCSAdapter::Builder::Builder()
        : mKey{}
          , mCert{}
          , mType7{}
          , mSign7{}
{
}

UtilsPKCSAdapter::Builder &UtilsPKCSAdapter::Builder::setKey(const Jchar *key)
{
    this->mKey.append(key);
    return (*this);
}

UtilsPKCSAdapter::Builder &UtilsPKCSAdapter::Builder::setCert(const Jchar *cert)
{
    this->mCert.append(cert);
    return (*this);
}

UtilsPKCSAdapter::Builder &UtilsPKCSAdapter::Builder::setPKCS7(UtilsPKCSType7I *v)
{
    this->mType7 = v;
    return (*this);
}

UtilsPKCSAdapter::Builder &UtilsPKCSAdapter::Builder::setPKCS7Sign(UtilsPKCS7SignI *v)
{
    this->mSign7 = v;
    return (*this);
}

std::shared_ptr<UtilsPKCSAdapter> UtilsPKCSAdapter::Builder::build()
{
    return std::make_shared<UtilsPKCSAdapter>(this);
}

UtilsPKCSAdapter::UtilsPKCSAdapter(Builder *builder)
        : mBuilder{builder}
          , mType7{builder->mType7}
          , mSign7{builder->mSign7}
{
    if (this->mSign7 == nullptr)
        this->mSign7 = new UtilsPKCS7SignWithOpenSSL(builder->mKey.c_str());
    if (this->mType7 == nullptr)
        this->mType7 = new UtilsPKCSType7WithOpenSSL(builder->mKey.c_str(), builder->mCert.c_str());
}

UtilsPKCSAdapter::~UtilsPKCSAdapter()
{
    delete (this->mBuilder);
}

Jbool UtilsPKCSAdapter::init()
{
    return this->mType7->pkcs7Init(this->mSign7);
}

Jbool UtilsPKCSAdapter::update(const Jbyte *v, Jint vLen)
{
    return this->mType7->pkcs7Update(v, vLen);
}

Jbool UtilsPKCSAdapter::final(std::vector<Jbyte> &v)
{
    return this->mType7->pkcs7Final(v);
}

}

}

#endif //CSIGNAPK_UTILSPKCSADAPTER_HPP
