#pragma once

#ifndef CSIGNAPK_UTILSSHA256WITHOPENSSL_HPP
#define CSIGNAPK_UTILSSHA256WITHOPENSSL_HPP

#include <openssl/sha.h>

namespace m
{

inline namespace apk
{

class UtilsSHA256WithOpenSSL
        : public UtilsSHA256I
{
private:
    SHA256_CTX mCtx;

    Jbyte mBuffer[SHA256_DIGEST_LENGTH];

public:
    UtilsSHA256WithOpenSSL();

    Jbool sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) override;

    Jbool sha256Init() override;

    Jbool sha256Update(const Jbyte *v, Jint vLen) override;

    Jbool sha256Final(std::vector<Jbyte> &ret) override;
};

UtilsSHA256WithOpenSSL::UtilsSHA256WithOpenSSL()
        : mCtx{}
          , mBuffer{}
{
}

Jbool UtilsSHA256WithOpenSSL::sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret)
{
    Jint i = 0;

    SHA256_Init(&this->mCtx);
    SHA256_Update(&this->mCtx, v, vLen);
    SHA256_Final(this->mBuffer, &this->mCtx);

    ret.clear();
    for (i = 0; i < static_cast<Jint>(sizeof(this->mBuffer)); ++i)
        ret.push_back(this->mBuffer[i]);
    return true;
}

Jbool UtilsSHA256WithOpenSSL::sha256Init()
{
    SHA256_Init(&this->mCtx);
    return true;
}

Jbool UtilsSHA256WithOpenSSL::sha256Update(const Jbyte *v, Jint vLen)
{
    SHA256_Update(&this->mCtx, v, vLen);
    return true;
}

Jbool UtilsSHA256WithOpenSSL::sha256Final(std::vector<Jbyte> &ret)
{
    Jint i = 0;

    SHA256_Final(this->mBuffer, &this->mCtx);

    ret.clear();
    for (i = 0; i < static_cast<Jint>(sizeof(this->mBuffer)); ++i)
        ret.push_back(this->mBuffer[i]);
    return true;
}

}

}

#endif //CSIGNAPK_UTILSSHA256WITHOPENSSL_HPP
