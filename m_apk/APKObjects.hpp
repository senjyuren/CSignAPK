#pragma once

#ifndef CSIGNAPK_APKOBJECTS_HPP
#define CSIGNAPK_APKOBJECTS_HPP

#include "local_impl/APKLocalFactory.hpp"

namespace m
{

inline namespace apk
{

class APKObjects
        : public UtilsZipUnPackInterceptI
{
public:
    class Builder;

private:
    Builder *mBuilder;

    APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon>             &mFile;
    APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &mFileSign;

    std::vector<APKLocalBeanFileSignedCon> mSelectArray;

    std::shared_ptr<UtilsSHAAdapter> mSHAAdapter;
    std::vector<Jbyte>               mSHAValue;
    std::string                      mSHAStrValue;

public:
    class Builder
    {
    private:
        APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon>             *mFile;
        APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> *mFileSign;

        std::string mAPKPath;
        std::string mAPKOutPath;

    public:
        friend APKObjects;

        Builder();

        Builder &setAPKPath(const Jchar *v);

        Builder &setAPKOutPath(const Jchar *v);

        Builder &setDBFile(APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon> &v);

        Builder &setDBFileSign(APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &v);

        std::shared_ptr<APKObjects> build();
    };

    explicit APKObjects(Builder *builder);

    ~APKObjects() override;

    void unPackStart(const Jchar *name) override;

    void unPackEnd(const Jchar *name) override;

    void unPackStream(const Jchar *name, const Jbyte *v, Jint vLen) override;
};

APKObjects::Builder::Builder()
        : mFile{}
          , mFileSign{}
          , mAPKPath{}
          , mAPKOutPath{}
{
    auto &&dbEnv = (new APKLocalEnv::Builder())
            ->build();

    APKLocalFactory::getInstance().init(dbEnv);
    this->mFile     = &APKLocalFactory::getInstance().getFile();
    this->mFileSign = &APKLocalFactory::getInstance().getFileSign();
}

APKObjects::Builder &APKObjects::Builder::setAPKPath(const Jchar *v)
{
    this->mAPKPath.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setAPKOutPath(const Jchar *v)
{
    this->mAPKOutPath.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setDBFile(APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon> &v)
{
    this->mFile = &v;
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setDBFileSign(
        APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &v
)
{
    this->mFileSign = &v;
    return (*this);
}

std::shared_ptr<APKObjects> APKObjects::Builder::build()
{
    return std::make_shared<APKObjects>(this);
}

APKObjects::APKObjects(Builder *builder)
        : mBuilder{builder}
          , mFile{*builder->mFile}
          , mFileSign{*builder->mFileSign}
          , mSelectArray{}
          , mSHAAdapter{}
          , mSHAValue{}
          , mSHAStrValue{}
{
    this->mSHAAdapter = (new UtilsSHAAdapter::Builder())
            ->build();

    (new UtilsZip::Builder())
            ->setInputPath(builder->mAPKPath.c_str())
            .setOutputPath(builder->mAPKPath.c_str())
            .setMode(UtilsZipMode::UN_PACK)
            .addUnPackSteam(this)
            .build();
}

APKObjects::~APKObjects()
{
    delete (this->mBuilder);
}

void APKObjects::unPackStart(const Jchar *name)
{
    this->mSHAAdapter->getSHA1().sha1Ready();
    this->mSHAAdapter->getSHA256().sha256Ready();
}

void APKObjects::unPackEnd(const Jchar *name)
{
    APKLocalBeanFileSignedCon sign;

    this->mFileSign.select(
            APKLocalBeanFileSignedVar::NAME,
            APKLocalBeanFileSignedCon().setName(name),
            this->mSelectArray
    );

    sign.setName(name);
    this->mSHAAdapter->getSHA1().sha1Done(this->mSHAValue);
    UtilsHEX::vOneTwo(this->mSHAValue.data(), this->mSHAValue.size(), this->mSHAStrValue);
    sign.setSHA1(this->mSHAStrValue.c_str());

    this->mSHAAdapter->getSHA256().sha256Done(this->mSHAValue);
    UtilsHEX::vOneTwo(this->mSHAValue.data(), this->mSHAValue.size(), this->mSHAStrValue);
    sign.setSHA256(this->mSHAStrValue.c_str());

    if (this->mSelectArray.empty())
        this->mFileSign.insert(sign);
    else
        this->mFileSign.update(APKLocalBeanFileSignedVar::NAME, APKLocalBeanFileSignedCon().setName(name), sign);
}

void APKObjects::unPackStream(const Jchar *name, const Jbyte *v, Jint vLen)
{
    this->mSHAAdapter->getSHA1().sha1Process(v, vLen);
    this->mSHAAdapter->getSHA256().sha256Process(v, vLen);
}

}

}

#endif //CSIGNAPK_APKOBJECTS_HPP
