#pragma once

#ifndef CSIGNAPK_UTILSBASE64_HPP
#define CSIGNAPK_UTILSBASE64_HPP

#include <openssl/evp.h>

namespace m
{

inline namespace apk
{

class UtilsBase64
{
private:
    UtilsBase64();

public:
    static Jbool Encrypt(const Jbyte *v, Jint vLen, std::string &ret);

    static Jbool Decrypt(const Jchar *v, std::vector<Jbyte> &ret);
};

Jbool UtilsBase64::Encrypt(const Jbyte *v, Jint vLen, std::string &ret)
{
    if ((v == nullptr) || (vLen < 1))
        return false;

    ret.clear();
    ret.resize(vLen * 2);
    return (EVP_EncodeBlock(reinterpret_cast<Jbyte *>(&ret[0]), v, vLen) > 0);
}

Jbool UtilsBase64::Decrypt(const Jchar *v, std::vector<Jbyte> &ret)
{
    Jint vLen = 0;

    if (v == nullptr)
        return false;

    vLen = strlen(v);
    ret.clear();
    ret.resize(vLen);
    return (EVP_DecodeBlock(&ret[0], reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)), vLen));
}

UtilsBase64::UtilsBase64() = default;

}

}

#endif //CSIGNAPK_UTILSBASE64_HPP
