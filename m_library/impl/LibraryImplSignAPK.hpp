#pragma once

#ifndef CSIGNAPK_LIBRARYIMPLSIGNAPK_HPP
#define CSIGNAPK_LIBRARYIMPLSIGNAPK_HPP

namespace m
{

inline namespace apk
{

class LibraryImplSignAPK
        : public LibrarySignAPKI
{
private:
    constexpr static Jchar DEFAULT_SERIAL_NUMBER[] = "F10869747416DEC7";

    constexpr static Jllong DEFAULT_STA_DATE = 1512986512;
    constexpr static Jllong DEFAULT_EXP_DATE = 2376986512;

    APKObjects::Builder         *mBuilder;
    std::shared_ptr<APKObjects> mObject;
    UtilsPKCS7SignI             *mPKCS7Sign;
    UtilsX509SignI              *mX509Ctx;
    UtilsX509SignI              *mX509Req;

public:
    explicit LibraryImplSignAPK(LibrarySignAPKExtSignI *v);

    LibrarySignAPKI &apkCity(const Jchar *v) override;

    LibrarySignAPKI &apkCommon(const Jchar *v) override;

    LibrarySignAPKI &apkEmail(const Jchar *v) override;

    LibrarySignAPKI &apkCountry(const Jchar *v) override;

    LibrarySignAPKI &apkOrganization(const Jchar *v) override;

    LibrarySignAPKI &apkOrganizationUnit(const Jchar *v) override;

    LibrarySignAPKI &apkProvince(const Jchar *v) override;

    LibrarySignAPKI &apkPrivateKey(const Jchar *v) override;

    LibrarySignAPKI &apkPublicKey(const Jchar *v) override;

    LibrarySignAPKI &apkInputAPKPath(const Jchar *v) override;

    LibrarySignAPKI &apkOutputPath(const Jchar *v) override;

    void apkSign() override;
};

LibraryImplSignAPK::LibraryImplSignAPK(LibrarySignAPKExtSignI *v)
        : mBuilder{}
          , mObject{}
          , mPKCS7Sign{new LibraryImplPKCS7Sign(v)}
          , mX509Ctx{new LibraryImplX509Sign(v)}
          , mX509Req{new LibraryImplX509ReqSign(v)}
{
    this->mBuilder = new APKObjects::Builder();
    if (v != nullptr)
    {
        this->mBuilder->setPKCS7Sign(this->mPKCS7Sign)
                .setX509Sign(this->mX509Ctx)
                .setX509ReqSign(this->mX509Req);
    }
}

LibrarySignAPKI &LibraryImplSignAPK::apkCity(const Jchar *v)
{
    this->mBuilder->setCity(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkCommon(const Jchar *v)
{
    this->mBuilder->setCommon(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkEmail(const Jchar *v)
{
    this->mBuilder->setEmail(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkCountry(const Jchar *v)
{
    this->mBuilder->setCountry(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkOrganization(const Jchar *v)
{
    this->mBuilder->setOrganization(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkOrganizationUnit(const Jchar *v)
{
    this->mBuilder->setOrganizationUnit(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkProvince(const Jchar *v)
{
    this->mBuilder->setProvince(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkPrivateKey(const Jchar *v)
{
    this->mBuilder->setPrivateKey(v);
    this->mBuilder->setCAPrivateKey(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkPublicKey(const Jchar *v)
{
    this->mBuilder->setPublicKey(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkInputAPKPath(const Jchar *v)
{
    this->mBuilder->setAPKPath(v);
    return (*this);
}

LibrarySignAPKI &LibraryImplSignAPK::apkOutputPath(const Jchar *v)
{
    this->mBuilder->setAPKOutPath(v);
    return (*this);
}

void LibraryImplSignAPK::apkSign()
{
    this->mObject = this->mBuilder->setVersion(UtilsX509Version::V3)
            .setSerialNumber(DEFAULT_SERIAL_NUMBER)
            .setStaDate(DEFAULT_STA_DATE)
            .setExpDate(DEFAULT_EXP_DATE)
            .build();
}

}

}

#endif //CSIGNAPK_LIBRARYIMPLSIGNAPK_HPP
