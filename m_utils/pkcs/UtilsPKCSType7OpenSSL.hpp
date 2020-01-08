#pragma once

#ifndef CSIGNAPK_UTILSPKCSTYPE7OPENSSL_HPP
#define CSIGNAPK_UTILSPKCSTYPE7OPENSSL_HPP

#include <openssl/pem.h>
#include <openssl/pkcs7.h>

namespace m
{

inline namespace apk
{

class UtilsPKCSType7OpenSSL
        : public UtilsPKCSType7I
{
private:
    std::string        mKeyPEM;
    std::string        mCertPEM;
    std::vector<Jbyte> mSignContent;

    BIO      *mKeyBIO;
    BIO      *mCertBIO;
    BIO      *mPKCS7BIO;
    X509     *mCertCon;
    EVP_PKEY *mPkeyCon;
    PKCS7    *mPKCS7;

public:
    UtilsPKCSType7OpenSSL(const Jchar *key, const Jchar *cert);

    Jbool pkcs7Ready() override;

    Jbool pkcs7Process(const Jbyte *v, Jint vLen) override;

    Jbool pkcs7Done(std::vector<Jbyte> &ret) override;
};

UtilsPKCSType7OpenSSL::UtilsPKCSType7OpenSSL(const Jchar *key, const Jchar *cert)
        : mKeyPEM{}
          , mCertPEM{}
          , mSignContent{}
          , mKeyBIO{}
          , mCertBIO{}
          , mPKCS7BIO{}
          , mCertCon{}
          , mPkeyCon{}
          , mPKCS7{}
{
    this->mKeyPEM.append(key);
    this->mCertPEM.append(cert);
}

Jbool UtilsPKCSType7OpenSSL::pkcs7Ready()
{
    this->mKeyBIO  = BIO_new_mem_buf(this->mKeyPEM.data(), this->mKeyPEM.size());
    this->mCertBIO = BIO_new_mem_buf(this->mCertPEM.data(), this->mCertPEM.size());

    this->mCertCon = PEM_read_bio_X509(this->mCertBIO, nullptr, nullptr, nullptr);
    PEM_read_bio_PrivateKey(this->mKeyBIO, &this->mPkeyCon, nullptr, nullptr);

    this->mPKCS7 = PKCS7_new();
    PKCS7_set_type(this->mPKCS7, NID_pkcs7_signed);
    PKCS7_add_certificate(this->mPKCS7, this->mCertCon);
    PKCS7_content_new(this->mPKCS7, NID_pkcs7_data);
    PKCS7_ctrl(this->mPKCS7,PKCS7_OP_SET_DETACHED_SIGNATURE,1, nullptr);
    PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha256());

    this->mPKCS7BIO = PKCS7_dataInit(this->mPKCS7, nullptr);
    return true;
}

Jbool UtilsPKCSType7OpenSSL::pkcs7Process(const Jbyte *v, Jint vLen)
{
    return (BIO_write(this->mPKCS7BIO, v, vLen) > 0);
}

Jbool UtilsPKCSType7OpenSSL::pkcs7Done(std::vector<Jbyte> &ret)
{
    Jint  i       = 0;
    Jint  retLen  = 0;
    Jbyte *retPtr = nullptr;

    PKCS7_dataFinal(this->mPKCS7, this->mPKCS7BIO);
    retLen = i2d_PKCS7(this->mPKCS7, &retPtr);
    ret.clear();
    for (i = 0; i < retLen; ++i)
        ret.push_back(retPtr[i]);

    if (retPtr != nullptr)
        free(retPtr);
    if (this->mPKCS7BIO != nullptr)
        BIO_free(this->mPKCS7BIO);
    if (this->mPKCS7 != nullptr)
        PKCS7_free(this->mPKCS7);
    if (this->mPkeyCon != nullptr)
        EVP_PKEY_free(this->mPkeyCon);
    if (this->mCertCon != nullptr)
        X509_free(this->mCertCon);
    if (this->mCertBIO != nullptr)
        BIO_free(this->mCertBIO);
    if (this->mKeyBIO != nullptr)
        BIO_free(this->mKeyBIO);
    return true;
}

}

}

#endif //CSIGNAPK_UTILSPKCSTYPE7OPENSSL_HPP
