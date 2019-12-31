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

    ~UtilsSHAAdapter();

    UtilsSHA1I &getSHA1();

    UtilsSHA256I &getSHA256();
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

UtilsSHA1I &UtilsSHAAdapter::getSHA1()
{
    return (*this->mSHA1);
}

UtilsSHA256I &UtilsSHAAdapter::getSHA256()
{
    return (*this->mSHA256);
}

}

}

#endif //CSIGNAPK_UTILSSHAADAPTER_HPP
