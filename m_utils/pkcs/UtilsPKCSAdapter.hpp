#pragma once

#ifndef CSIGNAPK_UTILSPKCSADAPTER_HPP
#define CSIGNAPK_UTILSPKCSADAPTER_HPP

#include "UtilsPKCSType7I.hpp"
#include "UtilsPKCSType7OpenSSL.hpp"

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

public:
    class Builder
    {
    private:
        std::string     mKeyPath;
        std::string     mCertPath;
        UtilsPKCSType7I *mType7;

    public:
        friend UtilsPKCSAdapter;

        Builder();

        Builder &setKeyAndCertPath(const Jchar *keyPath, const Jchar *certPath);

        Builder &setPKCS7(UtilsPKCSType7I *v);

        std::shared_ptr<UtilsPKCSAdapter> build();
    };

    explicit UtilsPKCSAdapter(Builder *builder);

    ~UtilsPKCSAdapter();

    UtilsPKCSType7I &getType7();
};

UtilsPKCSAdapter::Builder::Builder()
        : mKeyPath{}
          , mCertPath{}
          , mType7{}
{
}

UtilsPKCSAdapter::Builder &UtilsPKCSAdapter::Builder::setKeyAndCertPath(const Jchar *keyPath, const Jchar *certPath)
{
    this->mKeyPath.append(keyPath);
    this->mCertPath.append(certPath);
    return (*this);
}

UtilsPKCSAdapter::Builder &UtilsPKCSAdapter::Builder::setPKCS7(UtilsPKCSType7I *v)
{
    this->mType7 = v;
    return (*this);
}

std::shared_ptr<UtilsPKCSAdapter> UtilsPKCSAdapter::Builder::build()
{
    return std::make_shared<UtilsPKCSAdapter>(this);
}

UtilsPKCSAdapter::UtilsPKCSAdapter(Builder *builder)
        : mBuilder{builder}
          , mType7{builder->mType7}
{
    if (this->mType7 == nullptr)
        this->mType7 = new UtilsPKCSType7OpenSSL(builder->mKeyPath.c_str(), builder->mCertPath.c_str());
}

UtilsPKCSAdapter::~UtilsPKCSAdapter()
{
    delete (this->mBuilder);
}

UtilsPKCSType7I &UtilsPKCSAdapter::getType7()
{
    return (*this->mType7);
}

}

}

#endif //CSIGNAPK_UTILSPKCSADAPTER_HPP
