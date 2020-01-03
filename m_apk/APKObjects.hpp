#pragma once

#ifndef CSIGNAPK_APKOBJECTS_HPP
#define CSIGNAPK_APKOBJECTS_HPP

#include "local_impl/APKLocalFactory.hpp"
#include "signed_impl/APKSignedCert.hpp"
#include "signed_impl/APKSignedCertRSA.hpp"
#include "signed_impl/APKSignedManifest.hpp"

namespace m
{

inline namespace apk
{

class APKObjects
        : public UtilsZipUnPackInterceptI
          , public APKSignedManifestBlockI
          , public APKSignedCertBlockI
{
public:
    class Builder;

private:
    constexpr static Jchar DEFAULT_OUT_DIR[]  = ".";
    constexpr static Jchar BACK_CERT_SF[]     = "CERT.SF";
    constexpr static Jchar BACK_CERT_RSA[]    = "CERT.RSA";
    constexpr static Jchar BACK_MANIFEST_MF[] = "MANIFEST.MF";

    Builder *mBuilder;

    APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &mFileSign;

    std::vector<APKLocalBeanFileSignedCon> mSelectArray;

    std::shared_ptr<APKSignedCert>    mSignedCert;
    std::shared_ptr<APKSignedCertRSA> mSignedCertRSA;

    std::shared_ptr<UtilsPKCSAdapter> mPKCSAdapter;
    std::vector<Jbyte>                mPKCSValue;

    std::shared_ptr<UtilsSHAAdapter> mSHAAdapter;
    std::vector<Jbyte>               mSHAValue;
    std::string                      mSHABase64Value;

public:
    class Builder
    {
    private:
        APKLocalFactory *mLocalFactory;

        std::string mAPKPath;
        std::string mAPKOutPath;
        std::string mKeyPath;
        std::string mCertPath;

    public:
        friend APKObjects;

        Builder();

        Builder &setAPKPath(const Jchar *v);

        Builder &setAPKOutPath(const Jchar *v);

        Builder &setPrivateKeyPath(const Jchar *v);

        Builder &setCertPath(const Jchar *v);

        Builder &setLocalFactory(APKLocalFactory &factory);

        std::shared_ptr<APKObjects> build();
    };

    explicit APKObjects(Builder *builder);

    ~APKObjects() override;

    void unPackStart(const Jchar *name, const Jchar *path) override;

    void unPackEnd(const Jchar *name, const Jchar *path) override;

    void unPackStream(const Jchar *name, const Jchar *path, const Jbyte *v, Jint vLen) override;

    void manifestBlock(const Jchar *name, const Jchar *v, Jint vLen) override;

    void manifestContentStart(const Jchar *name) override;

    void manifestContentUpdate(const Jchar *name, const Jchar *v, Jint vLen) override;

    void manifestContentEnd(const Jchar *name) override;

    void certContentStart(const Jchar *name) override;

    void certContentUpdate(const Jchar *name, const Jchar *v, Jint vLen) override;

    void certContentEnd(const Jchar *name) override;
};

APKObjects::Builder::Builder()
        : mLocalFactory{&APKLocalFactory::getInstance()}
          , mAPKPath{}
          , mAPKOutPath{DEFAULT_OUT_DIR}
{
    auto &&dbEnv = (new APKLocalEnv::Builder())
            ->build();
    this->mLocalFactory = &APKLocalFactory::getInstance().init(dbEnv);
}

APKObjects::Builder &APKObjects::Builder::setAPKPath(const Jchar *v)
{
    this->mAPKPath.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setAPKOutPath(const Jchar *v)
{
    this->mAPKOutPath.clear();
    this->mAPKOutPath.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setPrivateKeyPath(const Jchar *v)
{
    this->mKeyPath.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setCertPath(const Jchar *v)
{
    this->mCertPath.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setLocalFactory(class m::apk::APKLocalFactory &factory)
{
    this->mLocalFactory = &factory;
    return (*this);
}

std::shared_ptr<APKObjects> APKObjects::Builder::build()
{
    return std::make_shared<APKObjects>(this);
}

APKObjects::APKObjects(Builder *builder)
        : mBuilder{builder}
          , mFileSign{builder->mLocalFactory->getFileSign()}
          , mSelectArray{}
          , mSignedCert{}
          , mSignedCertRSA{}
          , mPKCSAdapter{}
          , mPKCSValue{}
          , mSHAAdapter{}
          , mSHAValue{}
          , mSHABase64Value{}
{
    this->mFileSign.remove();

    this->mPKCSAdapter = (new UtilsPKCSAdapter::Builder())
            ->setKeyAndCertPath(builder->mKeyPath.c_str(), builder->mCertPath.c_str())
            .build();
    this->mSHAAdapter  = (new UtilsSHAAdapter::Builder())
            ->build();

    auto &&autoUnZip = (new UtilsZip::Builder())
            ->setInputPath(builder->mAPKPath.c_str())
            .setOutputPath(builder->mAPKOutPath.c_str())
            .addUnPackSteam(this)
            .addFileNameToBacklist(BACK_CERT_SF)
            .addFileNameToBacklist(BACK_CERT_RSA)
            .addFileNameToBacklist(BACK_MANIFEST_MF)
            .build();

    this->mSignedCertRSA = (new APKSignedCertRSA::Builder())
            ->setOutPath(builder->mAPKOutPath.c_str())
            .build();

    this->mSignedCert = (new APKSignedCert::Builder())
            ->setOutPath(builder->mAPKOutPath.c_str())
            .addIntercept(this)
            .build();

    auto &&manifest = (new APKSignedManifest::Builder())
            ->setOutPath(builder->mAPKOutPath.c_str())
            .addIntercept(this)
            .build();

    this->mFileSign.select(this->mSelectArray);
    if (this->mSelectArray.empty())
        return;

    for (APKLocalBeanFileSignedCon &v : this->mSelectArray)
    {
        UtilsBase64::Encrypt(v.getSHA256().data(), v.getSHA256().size(), this->mSHABase64Value);
        manifest->signContentStream(v.getName().c_str(), this->mSHABase64Value.c_str(), this->mSHABase64Value.length());
    }

    manifest->signStreamEnd();
    this->mSignedCert->signStreamEnd();
    this->mSignedCertRSA->signStreamEnd();

    auto &&autoZip = (new UtilsZip::Builder(autoUnZip->getAllZipPath(), autoUnZip->getAllRootPath()))
            ->setInputPath(builder->mAPKPath.c_str())
            .setOutputPath(builder->mAPKOutPath.c_str())
            .isOnlyPack()
            .addFileToPack(manifest->getPath().c_str())
            .addFileToPack(this->mSignedCert->getPath().c_str())
            .addFileToPack(this->mSignedCertRSA->getPath().c_str())
            .build();
    autoZip->pack();
}

APKObjects::~APKObjects()
{
    delete (this->mBuilder);
}

void APKObjects::unPackStart(const Jchar *, const Jchar *path)
{
    this->mSHAAdapter->getSHA1().sha1Ready();
    this->mSHAAdapter->getSHA256().sha256Ready();
}

void APKObjects::unPackEnd(const Jchar *name, const Jchar *path)
{
    APKLocalBeanFileSignedCon sign;

    this->mFileSign.select(
            APKLocalBeanFileSignedVar::NAME,
            APKLocalBeanFileSignedCon().setName(name),
            this->mSelectArray
    );

    sign.setName(name);
    this->mSHAAdapter->getSHA1().sha1Done(this->mSHAValue);
    sign.setSHA1(this->mSHAValue.data(), this->mSHAValue.size());

    this->mSHAAdapter->getSHA256().sha256Done(this->mSHAValue);
    sign.setSHA256(this->mSHAValue.data(), this->mSHAValue.size());

    if (this->mSelectArray.empty())
        this->mFileSign.insert(sign);
    else
        this->mFileSign.update(APKLocalBeanFileSignedVar::NAME, APKLocalBeanFileSignedCon().setName(name), sign);
}

void APKObjects::unPackStream(const Jchar *, const Jchar *path, const Jbyte *v, Jint vLen)
{
    this->mSHAAdapter->getSHA1().sha1Process(v, vLen);
    this->mSHAAdapter->getSHA256().sha256Process(v, vLen);
}

void APKObjects::manifestBlock(const Jchar *name, const Jchar *v, Jint vLen)
{
    this->mSHAAdapter->getSHA256().sha256(reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen, this->mSHAValue);
    UtilsBase64::Encrypt(this->mSHAValue.data(), this->mSHAValue.size(), this->mSHABase64Value);
    this->mSignedCert->signContentStream(name, this->mSHABase64Value.c_str(), this->mSHABase64Value.length());
}

void APKObjects::manifestContentStart(const Jchar *)
{
    this->mSHAAdapter->getSHA256().sha256Ready();
}

void APKObjects::manifestContentUpdate(const Jchar *, const Jchar *v, Jint vLen)
{
    this->mSHAAdapter->getSHA256().sha256Process(reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen);
}

void APKObjects::manifestContentEnd(const Jchar *name)
{
    this->mSHAAdapter->getSHA256().sha256Done(this->mSHAValue);
    UtilsBase64::Encrypt(this->mSHAValue.data(), this->mSHAValue.size(), this->mSHABase64Value);
    this->mSignedCert->signHeadStream(name, this->mSHABase64Value.c_str(), this->mSHABase64Value.length());
}

void APKObjects::certContentStart(const Jchar *)
{
    this->mPKCSAdapter->getType7().pkcs7Ready();
}

void APKObjects::certContentUpdate(const Jchar *, const Jchar *v, Jint vLen)
{
    this->mPKCSAdapter->getType7().pkcs7Process(reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen);
}

void APKObjects::certContentEnd(const Jchar *name)
{
    this->mPKCSAdapter->getType7().pkcs7Done(this->mPKCSValue);
    this->mSignedCertRSA->signContentStream(
            name,
            reinterpret_cast<Jchar *>(this->mPKCSValue.data()),
            this->mPKCSValue.size()
    );
}

}

}

#endif //CSIGNAPK_APKOBJECTS_HPP
