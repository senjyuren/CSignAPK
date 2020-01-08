#pragma once

#ifndef CSIGNAPK_UTILSZIP_HPP
#define CSIGNAPK_UTILSZIP_HPP

#include <utility>

#include "zip/UtilsZipUnPackInterceptI.hpp"
#include "zip/UtilsZipPackI.hpp"
#include "zip/UtilsZipUnPackI.hpp"

#include "zip/UtilsZipPackZLIB.hpp"
#include "zip/UtilsZipUnPackZLIB.hpp"

namespace m
{

inline namespace utils
{

class UtilsZip
{
public:
    class Builder;

private:
    constexpr static Jchar DIR_OF_TEMP[] = "csign_apk_v1_0_2020_01_02_13_50";

    Builder *mBuilder;

    std::shared_ptr<UtilsZipPackI> &mPack;
    std::string                    &mInPath;
    std::string                    &mOuPath;
    std::vector<std::string>       &mZipAllFilePath;
    std::vector<std::string>       &mRootAllFilePath;

    std::string mTempPath;

public:
    class Builder
    {
    private:
        std::shared_ptr<UtilsZipPackI>   mPack;
        std::shared_ptr<UtilsZipUnPackI> mUnPack;

        Jbool                    mNotCallUnPack;
        std::string              mInPath;
        std::string              mOuPath;
        std::vector<std::string> mZipAllFilePath;
        std::vector<std::string> mRootAllFilePath;

    public:
        friend UtilsZip;

        Builder();

        Builder(std::vector<std::string> allZip, std::vector<std::string> allRoot);

        ~Builder();

        Builder &isOnlyPack();

        Builder &setInputPath(const Jchar *v);

        Builder &setOutputPath(const Jchar *v);

        Builder &addFileNameToBacklist(const Jchar *v);

        Builder &addFileToPack(const Jchar *v);

        Builder &addUnPackSteam(UtilsZipUnPackInterceptI *intercept);

        std::shared_ptr<UtilsZip> build();
    };

    explicit UtilsZip(Builder *builder);

    ~UtilsZip();

    void pack();

    const std::vector<std::string> &getAllZipPath();

    const std::vector<std::string> &getAllRootPath();
};

UtilsZip::Builder::Builder()
        : mPack{}
          , mUnPack{}
          , mNotCallUnPack{}
          , mInPath{}
          , mOuPath{}
          , mZipAllFilePath{}
          , mRootAllFilePath{}
{
    this->mPack   = std::make_shared<UtilsZipPackZLIB>(this->mZipAllFilePath, this->mRootAllFilePath);
    this->mUnPack = std::make_shared<UtilsZipUnPackZLIB>(this->mZipAllFilePath, this->mRootAllFilePath);
}

UtilsZip::Builder::Builder(std::vector<std::string> allZip, std::vector<std::string> allRoot)
        : mPack{}
          , mUnPack{}
          , mNotCallUnPack{}
          , mInPath{}
          , mOuPath{}
          , mZipAllFilePath{std::move(allZip)}
          , mRootAllFilePath{std::move(allRoot)}
{
    this->mPack   = std::make_shared<UtilsZipPackZLIB>(this->mZipAllFilePath, this->mRootAllFilePath);
    this->mUnPack = std::make_shared<UtilsZipUnPackZLIB>(this->mZipAllFilePath, this->mRootAllFilePath);
}

UtilsZip::Builder::~Builder()
{
    this->mUnPack->cleanIntercept();
}

UtilsZip::Builder &UtilsZip::Builder::isOnlyPack()
{
    this->mNotCallUnPack = true;
    return (*this);
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

UtilsZip::Builder &UtilsZip::Builder::addFileNameToBacklist(const Jchar *v)
{
    this->mUnPack->addInBacklist(v);
    return (*this);
}

UtilsZip::Builder &UtilsZip::Builder::addFileToPack(const Jchar *v)
{
    this->mPack->addFile(v);
    return (*this);
}

UtilsZip::Builder &UtilsZip::Builder::addUnPackSteam(UtilsZipUnPackInterceptI *intercept)
{
    this->mUnPack->addIntercept(intercept);
    return (*this);
}

std::shared_ptr<UtilsZip> UtilsZip::Builder::build()
{
    return std::make_shared<UtilsZip>(this);
}

UtilsZip::UtilsZip(Builder *builder)
        : mBuilder{builder}
          , mPack{builder->mPack}
          , mInPath{builder->mInPath}
          , mOuPath{builder->mOuPath}
          , mZipAllFilePath{builder->mZipAllFilePath}
          , mRootAllFilePath{builder->mRootAllFilePath}
          , mTempPath{}
{
    this->mTempPath = std::filesystem::temp_directory_path().string();
    this->mTempPath.append(DIR_OF_TEMP);

    if (!builder->mNotCallUnPack)
    {
        if (std::filesystem::exists(this->mTempPath))
            std::filesystem::remove_all(this->mTempPath);
        builder->mUnPack->streamIntercept(this->mInPath.c_str(), this->mTempPath.c_str());
    }
}

UtilsZip::~UtilsZip()
{
    delete (this->mBuilder);
}

void UtilsZip::pack()
{
    this->mPack->pack(this->mTempPath.c_str(), this->mOuPath.c_str());
}

const std::vector<std::string> &UtilsZip::getAllZipPath()
{
    return this->mZipAllFilePath;
}

const std::vector<std::string> &UtilsZip::getAllRootPath()
{
    return this->mRootAllFilePath;
}

}

}

#endif //CSIGNAPK_UTILSZIP_HPP
