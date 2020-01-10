#pragma once

#ifndef CSIGNAPK_LIBRARYIMPLPKCS7SIGN_HPP
#define CSIGNAPK_LIBRARYIMPLPKCS7SIGN_HPP

namespace m
{

inline namespace apk
{

class LibraryImplPKCS7Sign
        : public UtilsPKCS7SignI
{
private:
    constexpr static Jint CACHE_SIZE = 4096;

    LibrarySignAPKExtSignI *mExt;
    Jbyte                  mCache[CACHE_SIZE];

public:
    explicit LibraryImplPKCS7Sign(LibrarySignAPKExtSignI *ext);

    Jbool pkcs7SignInit(UtilsPKCSRSAPadding v) override;

    Jbool pkcs7SignUpdate(const Jbyte *v, Jint vLen) override;

    Jbool pkcs7SignFinal(std::vector<Jbyte> &ret) override;
};

LibraryImplPKCS7Sign::LibraryImplPKCS7Sign(LibrarySignAPKExtSignI *ext)
        : mExt{ext}
          , mCache{}
{
}

Jbool LibraryImplPKCS7Sign::pkcs7SignInit(UtilsPKCSRSAPadding)
{
    return (this->mExt->extSignInit() > 0);
}

Jbool LibraryImplPKCS7Sign::pkcs7SignUpdate(const Jbyte *v, Jint vLen)
{
    return (this->mExt->extSignUpdate(v, vLen) > 0);
}

Jbool LibraryImplPKCS7Sign::pkcs7SignFinal(std::vector<Jbyte> &ret)
{
    Jint i      = 0;
    Jint retLen = 0;

    retLen = this->mExt->extSignFinal(this->mCache, sizeof(this->mCache));
    if (retLen < 1)
        return false;

    ret.clear();
    for (i = 0; i < retLen; ++i)
        ret.push_back(this->mCache[i]);
    return true;
}

}

}

#endif //CSIGNAPK_LIBRARYIMPLPKCS7SIGN_HPP
