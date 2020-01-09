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

    std::shared_ptr<UtilsX509Adapter> mX509Adapter;
    std::shared_ptr<UtilsPKCSAdapter> mPKCSAdapter;
    std::vector<Jbyte>                mPKCSValue;

    std::shared_ptr<UtilsSHAAdapter> mSHAAdapter;
    std::vector<Jbyte>               mSHAValue;
    std::string                      mSHABase64Value;

public:
    class Builder
    {
    private:
        std::shared_ptr<APKLocalEnv> mLocalEnv;

        std::string mAPKPath;
        std::string mAPKOutPath;
        std::string mPriKey;
        std::string mPubKey;
        std::string mCAPriKey;

        std::string mCity;
        std::string mCommon;
        std::string mEmail;
        std::string mCountry;
        std::string mOrganization;
        std::string mOrganizationUnit;
        std::string mProvince;
        std::string mSerialNumber;

        Jllong           mStaDate;
        Jllong           mExpDate;
        UtilsX509Version mVersion;

    public:
        friend APKObjects;

        Builder();

        Builder &setAPKPath(const Jchar *v);

        Builder &setAPKOutPath(const Jchar *v);

        Builder &setPrivateKey(const Jchar *v);

        Builder &setPublicKey(const Jchar *v);

        Builder &setCAPrivateKey(const Jchar *v);

        Builder &setCity(const Jchar *v);

        Builder &setCommon(const Jchar *v);

        Builder &setEmail(const Jchar *v);

        Builder &setCountry(const Jchar *v);

        Builder &setOrganization(const Jchar *v);

        Builder &setOrganizationUnit(const Jchar *v);

        Builder &setProvince(const Jchar *v);

        Builder &setSerialNumber(const Jchar *v);

        Builder &setStaDate(Jllong v);

        Builder &setExpDate(Jllong v);

        Builder &setVersion(UtilsX509Version v);

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
        : mLocalEnv{}
          , mAPKPath{}
          , mAPKOutPath{DEFAULT_OUT_DIR}
          , mPriKey{}
          , mPubKey{}
          , mCAPriKey{}
          , mCity{}
          , mCommon{}
          , mEmail{}
          , mCountry{}
          , mOrganization{}
          , mOrganizationUnit{}
          , mProvince{}
          , mSerialNumber{}
          , mStaDate{}
          , mExpDate{}
          , mVersion{}
{
    this->mLocalEnv = (new APKLocalEnv::Builder())
            ->build();
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

APKObjects::Builder &APKObjects::Builder::setPrivateKey(const Jchar *v)
{
    this->mPriKey.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setPublicKey(const Jchar *v)
{
    this->mPubKey.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setCAPrivateKey(const Jchar *v)
{
    this->mCAPriKey.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setCity(const Jchar *v)
{
    this->mCity.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setCommon(const Jchar *v)
{
    this->mCommon.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setEmail(const Jchar *v)
{
    this->mEmail.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setCountry(const Jchar *v)
{
    this->mCountry.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setOrganization(const Jchar *v)
{
    this->mOrganization.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setOrganizationUnit(const Jchar *v)
{
    this->mOrganizationUnit.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setProvince(const Jchar *v)
{
    this->mProvince.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setSerialNumber(const Jchar *v)
{
    this->mSerialNumber.append(v);
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setStaDate(Jllong v)
{
    this->mStaDate = v;
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setExpDate(Jllong v)
{
    this->mExpDate = v;
    return (*this);
}

APKObjects::Builder &APKObjects::Builder::setVersion(UtilsX509Version v)
{
    this->mVersion = v;
    return (*this);
}

std::shared_ptr<APKObjects> APKObjects::Builder::build()
{
    return std::make_shared<APKObjects>(this);
}

APKObjects::APKObjects(Builder *builder)
        : mBuilder{builder}
          , mFileSign{builder->mLocalEnv->getFileSign()}
          , mSelectArray{}
          , mSignedCert{}
          , mSignedCertRSA{}
          , mX509Adapter{}
          , mPKCSAdapter{}
          , mPKCSValue{}
          , mSHAAdapter{}
          , mSHAValue{}
          , mSHABase64Value{}
{
    this->mFileSign.remove();

    this->mX509Adapter = (new UtilsX509Adapter::Builder())
            ->setCity(builder->mCity.c_str())
            .setCommon(builder->mCommon.c_str())
            .setEmail(builder->mEmail.c_str())
            .setCountry(builder->mCountry.c_str())
            .setOrganization(builder->mOrganization.c_str())
            .setOrganizationUnit(builder->mOrganizationUnit.c_str())
            .setProvince(builder->mProvince.c_str())
            .setVersion(builder->mVersion)
            .setSerialNumber(builder->mSerialNumber.c_str())
            .setStaDate(builder->mStaDate)
            .setExpDate(builder->mExpDate)
            .setPublicKey(builder->mPubKey.c_str())
            .setPrivateKey(builder->mPriKey.c_str())
            .setCAPrivateKey(builder->mCAPriKey.c_str())
            .build();
    this->mPKCSAdapter = (new UtilsPKCSAdapter::Builder())
            ->setKey(builder->mPriKey.c_str())
            .setCert(this->mX509Adapter->getCert().c_str())
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

void APKObjects::unPackStart(const Jchar *, const Jchar *)
{
    this->mSHAAdapter->sha1Init();
    this->mSHAAdapter->sha256Init();
}

void APKObjects::unPackEnd(const Jchar *name, const Jchar *)
{
    APKLocalBeanFileSignedCon sign;

    this->mFileSign.select(
            APKLocalBeanFileSignedVar::NAME,
            APKLocalBeanFileSignedCon().setName(name),
            this->mSelectArray
    );

    sign.setName(name);
    this->mSHAAdapter->sha1Final(this->mSHAValue);
    sign.setSHA1(this->mSHAValue.data(), this->mSHAValue.size());

    this->mSHAAdapter->sha256Final(this->mSHAValue);
    sign.setSHA256(this->mSHAValue.data(), this->mSHAValue.size());

    if (this->mSelectArray.empty())
        this->mFileSign.insert(sign);
    else
        this->mFileSign.update(APKLocalBeanFileSignedVar::NAME, APKLocalBeanFileSignedCon().setName(name), sign);
}

void APKObjects::unPackStream(const Jchar *, const Jchar *, const Jbyte *v, Jint vLen)
{
    this->mSHAAdapter->sha1Update(v, vLen);
    this->mSHAAdapter->sha256Update(v, vLen);
}

void APKObjects::manifestBlock(const Jchar *name, const Jchar *v, Jint vLen)
{
    this->mSHAAdapter->sha256(reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen, this->mSHAValue);
    UtilsBase64::Encrypt(this->mSHAValue.data(), this->mSHAValue.size(), this->mSHABase64Value);
    this->mSignedCert->signContentStream(name, this->mSHABase64Value.c_str(), this->mSHABase64Value.length());
}

void APKObjects::manifestContentStart(const Jchar *)
{
    this->mSHAAdapter->sha256Init();
}

void APKObjects::manifestContentUpdate(const Jchar *, const Jchar *v, Jint vLen)
{
    this->mSHAAdapter->sha256Update(reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen);
}

void APKObjects::manifestContentEnd(const Jchar *name)
{
    this->mSHAAdapter->sha256Final(this->mSHAValue);
    UtilsBase64::Encrypt(this->mSHAValue.data(), this->mSHAValue.size(), this->mSHABase64Value);
    this->mSignedCert->signHeadStream(name, this->mSHABase64Value.c_str(), this->mSHABase64Value.length());
}

void APKObjects::certContentStart(const Jchar *)
{
    this->mPKCSAdapter->init();
}

void APKObjects::certContentUpdate(const Jchar *, const Jchar *v, Jint vLen)
{
    this->mPKCSAdapter->update(reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen);
}

void APKObjects::certContentEnd(const Jchar *name)
{
    this->mPKCSAdapter->final(this->mPKCSValue);
    this->mSignedCertRSA->signContentStream(
            name,
            reinterpret_cast<Jchar *>(this->mPKCSValue.data()),
            this->mPKCSValue.size()
    );
}

}

}

#endif //CSIGNAPK_APKOBJECTS_HPP
