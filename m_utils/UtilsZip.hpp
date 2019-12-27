#pragma once

#ifndef CSIGNAPK_UTILSZIP_HPP
#define CSIGNAPK_UTILSZIP_HPP

#include "zip/UtilsZipUnPackInterceptI.hpp"
#include "zip/UtilsZipUnPackI.hpp"

#include "zip/UtilsZipMode.hpp"
#include "zip/UtilsZipPack.hpp"
#include "zip/UtilsZipUnPack.hpp"

namespace m
{

inline namespace utils
{

class UtilsZip
{
public:
    class Builder;

private:
    Builder *mBuilder;

    UtilsZipMode &mMode;
    std::string  &mInPath;
    std::string  &mOuPath;

public:
    class Builder
    {
    private:
        std::unique_ptr<UtilsZipUnPackI> mUnPack;

        UtilsZipMode mMode;
        std::string  mInPath;
        std::string  mOuPath;

    public:
        friend UtilsZip;

        Builder();

        ~Builder();

        Builder &setInputPath(const Jchar *v);

        Builder &setOutputPath(const Jchar *v);

        Builder &setMode(UtilsZipMode mode);

        Builder &addUnPackSteam(UtilsZipUnPackInterceptI *intercept);

        std::shared_ptr<UtilsZip> build();
    };

    explicit UtilsZip(Builder *builder);

    ~UtilsZip();
};

UtilsZip::Builder::Builder()
        : mUnPack{new UtilsZipUnPack()}
          , mMode{}
          , mInPath{}
          , mOuPath{}
{
}

UtilsZip::Builder::~Builder()
{
    this->mUnPack->clean();
}

UtilsZip::Builder &UtilsZip::Builder::setInputPath(const Jchar *v)
{
    this->mInPath.append(v);
    return (*this);
}

UtilsZip::Builder &UtilsZip::Builder::setOutputPath(const Jchar *v)
{
    this->mOuPath.append(v);
    return (*this);
}

UtilsZip::Builder &UtilsZip::Builder::setMode(UtilsZipMode mode)
{
    this->mMode = mode;
    return (*this);
}

UtilsZip::Builder &UtilsZip::Builder::addUnPackSteam(UtilsZipUnPackInterceptI *intercept)
{
    this->mUnPack->add(intercept);
    return (*this);
}

std::shared_ptr<UtilsZip> UtilsZip::Builder::build()
{
    return std::make_shared<UtilsZip>(this);
}

UtilsZip::UtilsZip(Builder *builder)
        : mBuilder{builder}
          , mMode{builder->mMode}
          , mInPath{builder->mInPath}
          , mOuPath{builder->mOuPath}
{
    if (this->mMode == UtilsZipMode::UN_PACK)
        builder->mUnPack->stream(this->mInPath.c_str(), this->mOuPath.c_str());
}

UtilsZip::~UtilsZip()
{
    delete (this->mBuilder);
}

}

}

#endif //CSIGNAPK_UTILSZIP_HPP
