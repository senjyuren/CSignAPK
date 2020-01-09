#pragma once

#ifndef CSIGNAPK_UTILSPKCSTYPE7WITHOPENSSL_HPP
#define CSIGNAPK_UTILSPKCSTYPE7WITHOPENSSL_HPP

#include <openssl/pem.h>
#include <openssl/pkcs7.h>

namespace m
{

inline namespace apk
{

static UtilsPKCS7SignI *sPKCS7SignObject = nullptr;

class UtilsPKCSType7WithOpenSSL
        : public UtilsPKCSType7I
{
private:
    std::string        mKeyPEM;
    std::string        mCertPEM;
    std::vector<Jbyte> mSignContent;

    BIO        *mKeyBIO;
    BIO        *mCertBIO;
    BIO        *mPKCS7BIO;
    X509       *mCertCon;
    EVP_PKEY   *mPkeyCon;
    PKCS7      *mPKCS7;
    RSA_METHOD *mRSAMeth;

public:
    UtilsPKCSType7WithOpenSSL(const Jchar *key, const Jchar *cert);

    Jbool pkcs7Init(UtilsPKCS7SignI *sign) override;

    Jbool pkcs7Update(const Jbyte *v, Jint vLen) override;

    Jbool pkcs7Final(std::vector<Jbyte> &ret) override;
};

UtilsPKCSType7WithOpenSSL::UtilsPKCSType7WithOpenSSL(const Jchar *key, const Jchar *cert)
        : mKeyPEM{}
          , mCertPEM{}
          , mSignContent{}
          , mKeyBIO{}
          , mCertBIO{}
          , mPKCS7BIO{}
          , mCertCon{}
          , mPkeyCon{}
          , mPKCS7{}
          , mRSAMeth{}
{
    this->mKeyPEM.append(key);
    this->mCertPEM.append(cert);
}

Jbool UtilsPKCSType7WithOpenSSL::pkcs7Init(UtilsPKCS7SignI *sign)
{
    RSA              *rsa     = nullptr;
    const RSA_METHOD *rsaMeth = nullptr;

    sPKCS7SignObject = sign;

    this->mKeyBIO  = BIO_new_mem_buf(this->mKeyPEM.data(), this->mKeyPEM.size());
    this->mCertBIO = BIO_new_mem_buf(this->mCertPEM.data(), this->mCertPEM.size());

    PEM_read_bio_X509(this->mCertBIO, &this->mCertCon, nullptr, nullptr);
    if (this->mCertCon == nullptr)
        return false;

    PEM_read_bio_PrivateKey(this->mKeyBIO, &this->mPkeyCon, nullptr, nullptr);
    if (this->mPkeyCon == nullptr)
        return false;

    rsa = EVP_PKEY_get0_RSA(this->mPkeyCon);
    if (rsa == nullptr)
        return false;

    rsaMeth = RSA_get_method(rsa);
    this->mRSAMeth = RSA_meth_dup(rsaMeth);
    RSA_meth_set_priv_enc(
            this->mRSAMeth,
            [](Jint flen, const Jbyte *from, Jbyte *ret, RSA *, Jint padding) -> Jint {
                Jbool              state = false;
                std::vector<Jbyte> result;

                if (padding == RSA_PKCS1_PADDING)
                    sPKCS7SignObject->pkcs7SignInit(UtilsPKCSRSAPadding::PKCS1);
                else if (padding == RSA_NO_PADDING)
                    sPKCS7SignObject->pkcs7SignInit(UtilsPKCSRSAPadding::NONE);
                else if (padding == RSA_PKCS1_OAEP_PADDING)
                    sPKCS7SignObject->pkcs7SignInit(UtilsPKCSRSAPadding::PKCS1_OAEP);
                else if (padding == RSA_X931_PADDING)
                    sPKCS7SignObject->pkcs7SignInit(UtilsPKCSRSAPadding::X931);
                else if (padding == RSA_PKCS1_PSS_PADDING)
                    sPKCS7SignObject->pkcs7SignInit(UtilsPKCSRSAPadding::PSS);

                sPKCS7SignObject->pkcs7SignUpdate(from, flen);
                state = sPKCS7SignObject->pkcs7SignFinal(result);
                if (!state)
                    return -1;

                memcpy(ret, result.data(), result.size());
                return result.size();
            }
    );

    RSA_set_method(rsa, this->mRSAMeth);
    this->mPKCS7 = PKCS7_new();
    if (this->mPKCS7 == nullptr)
        return false;

    PKCS7_set_type(this->mPKCS7, NID_pkcs7_signed);
    PKCS7_add_certificate(this->mPKCS7, this->mCertCon);
    PKCS7_content_new(this->mPKCS7, NID_pkcs7_data);
    PKCS7_ctrl(this->mPKCS7, PKCS7_OP_SET_DETACHED_SIGNATURE, 1, nullptr);
    PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha256());

    this->mPKCS7BIO = PKCS7_dataInit(this->mPKCS7, nullptr);
    return (this->mPKCS7BIO != nullptr);
}

Jbool UtilsPKCSType7WithOpenSSL::pkcs7Update(const Jbyte *v, Jint vLen)
{
    return (BIO_write(this->mPKCS7BIO, v, vLen) > 0);
}

Jbool UtilsPKCSType7WithOpenSSL::pkcs7Final(std::vector<Jbyte> &ret)
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
    if (this->mRSAMeth != nullptr)
        RSA_meth_free(this->mRSAMeth);
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

#endif //CSIGNAPK_UTILSPKCSTYPE7WITHOPENSSL_HPP
