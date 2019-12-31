#pragma once

#ifndef CSIGNAPK_UTILSPKCSTYPE7OPENSSL_HPP
#define CSIGNAPK_UTILSPKCSTYPE7OPENSSL_HPP

# ifdef _WIN32
#  undef X509_NAME
#  undef X509_EXTENSIONS
#  undef PKCS7_ISSUER_AND_SERIAL
#  undef PKCS7_SIGNER_INFO
#  undef OCSP_REQUEST
#  undef OCSP_RESPONSE
# endif

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
    constexpr static Jint OID_SIZE       = 128;
    constexpr static Jint READ_CACHE_MAX = 512;

    constexpr static Jchar SHA1_WITH_RSA_ENCRYPTION[]   = "sha1WithRSAEncryption";
    constexpr static Jchar SHA224_WITH_RSA_ENCRYPTION[] = "sha224WithRSAEncryption";
    constexpr static Jchar SHA256_WITH_RSA_ENCRYPTION[] = "sha256WithRSAEncryption";
    constexpr static Jchar SHA384_WITH_RSA_ENCRYPTION[] = "sha384WithRSAEncryption";
    constexpr static Jchar SHA512_WITH_RSA_ENCRYPTION[] = "sha512WithRSAEncryption";

    std::vector<Jbyte> mKeyPEM;
    std::vector<Jbyte> mCertPEM;

    BIO                 *mKeyBIO;
    BIO                 *mCertBIO;
    BIO                 *mPKCS7BIO;
    X509                *mCertCon;
    PKCS8_PRIV_KEY_INFO *mPkeyInfo;
    EVP_PKEY            *mPkeyCon;
    X509_ALGOR          *mAlgo;
    PKCS7               *mPKCS7;

    Jchar mOID[OID_SIZE];
    Jchar mReadCache[READ_CACHE_MAX];

public:
    UtilsPKCSType7OpenSSL(const Jchar *keyPath, const Jchar *certPath);

    Jbool pkcs7Ready() override;

    Jbool pkcs7Process(const Jbyte *v, Jint vLen) override;

    Jbool pkcs7Done(std::vector<Jbyte> &ret) override;
};

UtilsPKCSType7OpenSSL::UtilsPKCSType7OpenSSL(const Jchar *keyPath, const Jchar *certPath)
        : mKeyPEM{}
          , mCertPEM{}
          , mKeyBIO{}
          , mCertBIO{}
          , mPKCS7BIO{}
          , mCertCon{}
          , mPkeyInfo{}
          , mPkeyCon{}
          , mAlgo{}
          , mPKCS7{}
          , mOID{}
          , mReadCache{}
{
    Jint i = 0;

    std::ifstream file;
    std::string   keyP(keyPath);
    std::string   certP(certPath);

    if ((!std::filesystem::exists(keyP)) || (!std::filesystem::exists(certP)))
        return;

    file.open(keyP.c_str(), (std::ios::in | std::ios::binary));

    do
    {
        file.read(this->mReadCache, sizeof(this->mReadCache));
        for (i = 0; i < file.gcount(); ++i)
            this->mKeyPEM.push_back(this->mReadCache[i]);
    } while (file.gcount() == sizeof(this->mReadCache));

    file.close();
    file.open(certP);

    do
    {
        file.read(this->mReadCache, sizeof(this->mReadCache));
        for (i = 0; i < file.gcount(); ++i)
            this->mCertPEM.push_back(this->mReadCache[i]);
    } while (file.gcount() == sizeof(this->mReadCache));

    file.close();
}

Jbool UtilsPKCSType7OpenSSL::pkcs7Ready()
{
    this->mKeyBIO  = BIO_new_mem_buf(this->mKeyPEM.data(), this->mKeyPEM.size());
    this->mCertBIO = BIO_new_mem_buf(this->mCertPEM.data(), this->mCertPEM.size());

    this->mCertCon  = PEM_read_bio_X509(this->mCertBIO, nullptr, nullptr, nullptr);
    this->mPkeyInfo = d2i_PKCS8_PRIV_KEY_INFO_bio(this->mKeyBIO, nullptr);
    this->mPkeyCon  = EVP_PKCS82PKEY(this->mPkeyInfo);

    memset(this->mOID, 0, sizeof(this->mOID));
    this->mAlgo = const_cast<X509_ALGOR *>(X509_get0_tbs_sigalg(this->mCertCon));
    OBJ_obj2txt(this->mOID, sizeof(this->mOID), this->mAlgo->algorithm, 0);

    this->mPKCS7 = PKCS7_new();
    PKCS7_set_type(this->mPKCS7, NID_pkcs7_signed);
    PKCS7_add_certificate(this->mPKCS7, this->mCertCon);
    PKCS7_content_new(this->mPKCS7, NID_pkcs7_data);
    PKCS7_set_detached(this->mPKCS7, 1);

    if (UtilsString::Equals(this->mOID, SHA1_WITH_RSA_ENCRYPTION))
        PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha1());
    else if (UtilsString::Equals(this->mOID, SHA224_WITH_RSA_ENCRYPTION))
        PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha224());
    else if (UtilsString::Equals(this->mOID, SHA256_WITH_RSA_ENCRYPTION))
        PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha256());
    else if (UtilsString::Equals(this->mOID, SHA384_WITH_RSA_ENCRYPTION))
        PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha384());
    else if (UtilsString::Equals(this->mOID, SHA512_WITH_RSA_ENCRYPTION))
        PKCS7_add_signature(this->mPKCS7, this->mCertCon, this->mPkeyCon, EVP_sha512());

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
    for (i = 0; i < retLen; ++i)
        ret.push_back(retPtr[i]);

    if (retPtr != nullptr)
        free(retPtr);
    if (this->mPKCS7BIO != nullptr)
        BIO_free(this->mPKCS7BIO);
    if (this->mPkeyInfo != nullptr)
        PKCS8_PRIV_KEY_INFO_free(this->mPkeyInfo);
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

    this->mPKCS7BIO = nullptr;
    this->mPkeyInfo = nullptr;
    this->mAlgo     = nullptr;
    this->mPKCS7    = nullptr;
    this->mPkeyCon  = nullptr;
    this->mCertCon  = nullptr;
    this->mCertBIO  = nullptr;
    this->mKeyBIO   = nullptr;
    return true;
}

}

}

#endif //CSIGNAPK_UTILSPKCSTYPE7OPENSSL_HPP
