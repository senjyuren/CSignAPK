#pragma once

#ifndef CSIGNAPK_UTILSPKCS7SIGNWITHOPENSSL_HPP
#define CSIGNAPK_UTILSPKCS7SIGNWITHOPENSSL_HPP

#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace m
{

inline namespace apk
{

class UtilsPKCS7SignWithOpenSSL
        : public UtilsPKCS7SignI
{
private:
    constexpr static Jint EN_BLOCK_SIZE = 4096;

    std::vector<Jbyte>  mDataArea;
    std::string         mPriKey;
    UtilsPKCSRSAPadding mPadding;

    Jbyte mBlock[EN_BLOCK_SIZE];

public:
    explicit UtilsPKCS7SignWithOpenSSL(const Jchar *prikey);

    Jbool pkcs7SignInit(UtilsPKCSRSAPadding v) override;

    Jbool pkcs7SignUpdate(const Jbyte *v, Jint vLen) override;

    Jbool pkcs7SignFinal(std::vector<Jbyte> &ret) override;
};

UtilsPKCS7SignWithOpenSSL::UtilsPKCS7SignWithOpenSSL(const Jchar *prikey)
        : mDataArea{}
          , mPriKey{}
          , mPadding{}
          , mBlock{}
{
    this->mPriKey.append(prikey);
}

Jbool UtilsPKCS7SignWithOpenSSL::pkcs7SignInit(UtilsPKCSRSAPadding v)
{
    this->mDataArea.clear();
    this->mPadding = v;
    return true;
}

Jbool UtilsPKCS7SignWithOpenSSL::pkcs7SignUpdate(const Jbyte *v, Jint vLen)
{
    Jint i = 0;

    for (i = 0; i < vLen; ++i)
        this->mDataArea.push_back(v[i]);
    return true;
}

Jbool UtilsPKCS7SignWithOpenSSL::pkcs7SignFinal(std::vector<Jbyte> &ret)
{
    Jint  i      = 0;
    Jint  retLen = 0;
    Jbool state  = false;

    BIO *prikeyBIO = nullptr;
    RSA *prikeyRSA = nullptr;

    ret.clear();
    if (this->mPriKey.empty())
        return false;

    do
    {
        prikeyBIO = BIO_new_mem_buf(this->mPriKey.data(), this->mPriKey.size());
        if (prikeyBIO == nullptr)
            break;

        PEM_read_bio_RSAPrivateKey(prikeyBIO, &prikeyRSA, nullptr, nullptr);
        if (prikeyRSA == nullptr)
            break;

        if (this->mPadding == UtilsPKCSRSAPadding::NONE)
            retLen = RSA_private_encrypt(
                    this->mDataArea.size(),
                    this->mDataArea.data(),
                    this->mBlock,
                    prikeyRSA,
                    RSA_NO_PADDING
            );
        else if (this->mPadding == UtilsPKCSRSAPadding::PKCS1)
            retLen = RSA_private_encrypt(
                    this->mDataArea.size(),
                    this->mDataArea.data(),
                    this->mBlock,
                    prikeyRSA,
                    RSA_PKCS1_PADDING
            );
        else if (this->mPadding == UtilsPKCSRSAPadding::PKCS1_OAEP)
            retLen = RSA_private_encrypt(
                    this->mDataArea.size(),
                    this->mDataArea.data(),
                    this->mBlock,
                    prikeyRSA,
                    RSA_PKCS1_OAEP_PADDING
            );
        else if (this->mPadding == UtilsPKCSRSAPadding::X931)
            retLen = RSA_private_encrypt(
                    this->mDataArea.size(),
                    this->mDataArea.data(),
                    this->mBlock,
                    prikeyRSA,
                    RSA_X931_PADDING
            );
        else
            retLen = RSA_private_encrypt(
                    this->mDataArea.size(),
                    this->mDataArea.data(),
                    this->mBlock,
                    prikeyRSA,
                    RSA_PKCS1_PSS_PADDING
            );

        if (retLen < 0)
            break;

        for (i = 0; i < retLen; ++i)
            ret.push_back(this->mBlock[i]);

        state = true;
    } while (false);

    if (prikeyRSA != nullptr)
        RSA_free(prikeyRSA);
    if (prikeyBIO != nullptr)
        BIO_free(prikeyBIO);
    return state;
}

}

}

#endif //CSIGNAPK_UTILSPKCS7SIGNWITHOPENSSL_HPP
