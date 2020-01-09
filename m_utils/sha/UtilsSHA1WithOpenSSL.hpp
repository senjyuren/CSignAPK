#pragma once

#ifndef CSIGNAPK_UTILSSHA1WITHOPENSSL_HPP
#define CSIGNAPK_UTILSSHA1WITHOPENSSL_HPP

#include <openssl/sha.h>

namespace m
{

inline namespace apk
{

class UtilsSHA1WithOpenSSL
        : public UtilsSHA1I
{
private:
    SHA_CTX mCtx;

    Jbyte mBuffer[SHA_DIGEST_LENGTH];

public:
    UtilsSHA1WithOpenSSL();

    Jbool sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) override;

    Jbool sha1Init() override;

    Jbool sha1Update(const Jbyte *v, Jint vLen) override;

    Jbool sha1Final(std::vector<Jbyte> &ret) override;
};

UtilsSHA1WithOpenSSL::UtilsSHA1WithOpenSSL()
        : mCtx{}
          , mBuffer{}
{
}

Jbool UtilsSHA1WithOpenSSL::sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret)
{
    Jint i = 0;

    SHA1_Init(&this->mCtx);
    SHA1_Update(&this->mCtx, v, vLen);
    SHA1_Final(this->mBuffer, &this->mCtx);

    ret.clear();
    for (i = 0; i < static_cast<Jint>(sizeof(this->mBuffer)); ++i)
        ret.push_back(this->mBuffer[i]);
    return true;
}

Jbool UtilsSHA1WithOpenSSL::sha1Init()
{
    SHA1_Init(&this->mCtx);
    return true;
}

Jbool UtilsSHA1WithOpenSSL::sha1Update(const Jbyte *v, Jint vLen)
{
    SHA1_Update(&this->mCtx, v, vLen);
    return true;
}

Jbool UtilsSHA1WithOpenSSL::sha1Final(std::vector<Jbyte> &ret)
{
    Jint i = 0;

    SHA1_Final(this->mBuffer, &this->mCtx);

    ret.clear();
    for (i = 0; i < static_cast<Jint>(sizeof(this->mBuffer)); ++i)
        ret.push_back(this->mBuffer[i]);
    return true;
}

}

}

#endif //CSIGNAPK_UTILSSHA1WITHOPENSSL_HPP
