#pragma once

#ifndef CSIGNAPK_LIBRARYIMPLX509SIGN_HPP
#define CSIGNAPK_LIBRARYIMPLX509SIGN_HPP

namespace m
{

inline namespace apk
{

class LibraryImplX509Sign
        : public UtilsX509SignI
{
private:
    constexpr static Jint CACHE_SIZE = 4096;

    LibrarySignAPKExtSignI *mExt;
    Jbyte                  mCache[CACHE_SIZE];

public:
    explicit LibraryImplX509Sign(LibrarySignAPKExtSignI *ext);

    Jbool x509SignInit(UtilsX509RSAPadding v) override;

    Jbool x509SignUpdate(const Jbyte *v, Jint vLen) override;

    Jbool x509SignFinal(std::vector<Jbyte> &ret) override;
};

LibraryImplX509Sign::LibraryImplX509Sign(LibrarySignAPKExtSignI *ext)
        : mExt{ext}
          , mCache{}
{
}

Jbool LibraryImplX509Sign::x509SignInit(UtilsX509RSAPadding)
{
    return (this->mExt->extSignInit() > 0);
}

Jbool LibraryImplX509Sign::x509SignUpdate(const Jbyte *v, Jint vLen)
{
    return (this->mExt->extSignUpdate(v, vLen) > 0);
}

Jbool LibraryImplX509Sign::x509SignFinal(std::vector<Jbyte> &ret)
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

#endif //CSIGNAPK_LIBRARYIMPLX509SIGN_HPP
