#pragma once

#ifndef CSIGNAPK_UTILSSHAADAPTER_HPP
#define CSIGNAPK_UTILSSHAADAPTER_HPP

#include "UtilsSHA1I.hpp"
#include "UtilsSHA256I.hpp"

#include "UtilsSHA1WithOpenSSL.hpp"
#include "UtilsSHA256WithOpenSSL.hpp"

namespace m
{

inline namespace apk
{

class UtilsSHAAdapter
        : public UtilsSHA1I
          , public UtilsSHA256I
{
public:
    class Builder;

private:
    Builder *mBuilder;

    UtilsSHA1I   *mSHA1;
    UtilsSHA256I *mSHA256;

public:
    class Builder
    {
    private:
        UtilsSHA1I   *mSHA1;
        UtilsSHA256I *mSHA256;

    public:
        friend UtilsSHAAdapter;

        Builder();

        Builder &setSHA1Impl(UtilsSHA1I *v);

        Builder &setSHA256Impl(UtilsSHA256I *v);

        std::shared_ptr<UtilsSHAAdapter> build();
    };

    explicit UtilsSHAAdapter(Builder *builder);

    ~UtilsSHAAdapter() override;

    Jbool sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) override;

    Jbool sha1Init() override;

    Jbool sha1Update(const Jbyte *v, Jint vLen) override;

    Jbool sha1Final(std::vector<Jbyte> &ret) override;

    Jbool sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) override;

    Jbool sha256Init() override;

    Jbool sha256Update(const Jbyte *v, Jint vLen) override;

    Jbool sha256Final(std::vector<Jbyte> &ret) override;
};

UtilsSHAAdapter::Builder::Builder()
        : mSHA1{}
          , mSHA256{}
{
}

UtilsSHAAdapter::Builder &UtilsSHAAdapter::Builder::setSHA1Impl(UtilsSHA1I *v)
{
    this->mSHA1 = v;
    return (*this);
}

UtilsSHAAdapter::Builder &UtilsSHAAdapter::Builder::setSHA256Impl(UtilsSHA256I *v)
{
    this->mSHA256 = v;
    return (*this);
}

std::shared_ptr<UtilsSHAAdapter> UtilsSHAAdapter::Builder::build()
{
    return std::make_shared<UtilsSHAAdapter>(this);
}

UtilsSHAAdapter::UtilsSHAAdapter(UtilsSHAAdapter::Builder *builder)
        : mBuilder{builder}
          , mSHA1{builder->mSHA1}
          , mSHA256{builder->mSHA256}
{
    if (this->mSHA1 == nullptr)
        this->mSHA1   = new UtilsSHA1WithOpenSSL();
    if (this->mSHA256 == nullptr)
        this->mSHA256 = new UtilsSHA256WithOpenSSL();
}

UtilsSHAAdapter::~UtilsSHAAdapter()
{
    delete (this->mBuilder);
}

Jbool UtilsSHAAdapter::sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret)
{
    return this->mSHA1->sha1(v, vLen, ret);
}

Jbool UtilsSHAAdapter::sha1Init()
{
    return this->mSHA1->sha1Init();
}

Jbool UtilsSHAAdapter::sha1Update(const Jbyte *v, Jint vLen)
{
    return this->mSHA1->sha1Update(v, vLen);
}

Jbool UtilsSHAAdapter::sha1Final(std::vector<Jbyte> &ret)
{
    return this->mSHA1->sha1Final(ret);
}

Jbool UtilsSHAAdapter::sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret)
{
    return this->mSHA256->sha256(v, vLen, ret);
}

Jbool UtilsSHAAdapter::sha256Init()
{
    return this->mSHA256->sha256Init();
}

Jbool UtilsSHAAdapter::sha256Update(const Jbyte *v, Jint vLen)
{
    return this->mSHA256->sha256Update(v, vLen);
}

Jbool UtilsSHAAdapter::sha256Final(std::vector<Jbyte> &ret)
{
    return this->mSHA256->sha256Final(ret);
}

}

}

#endif //CSIGNAPK_UTILSSHAADAPTER_HPP
