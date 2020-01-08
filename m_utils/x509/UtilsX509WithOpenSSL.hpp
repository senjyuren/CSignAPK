#pragma once

#ifndef CSIGNAPK_UTILSX509WITHOPENSSL_HPP
#define CSIGNAPK_UTILSX509WITHOPENSSL_HPP

#if defined (_WIN32)
#undef X509_NAME
#undef X509_EXTENSIONS
#undef PKCS7_ISSUER_AND_SERIAL
#undef PKCS7_SIGNER_INFO
#undef OCSP_REQUEST
#undef OCSP_RESPONSE
#endif

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

namespace m
{

inline namespace apk
{

static UtilsX509SignI *sX509ContextSignObject = nullptr;

class UtilsX509WithOpenSSL
        : public UtilsX509I
{
private:
    constexpr static Jint BN_SIZE    = 159;
    constexpr static Jint READ_CACHE = 128;

    X509       *mX509Ctx;
    EVP_PKEY   *mEvpPKey;
    RSA_METHOD *mRSAMeth;

    Jchar mReadCache[READ_CACHE];

public:
    UtilsX509WithOpenSSL();

    Jbool x509Init() override;

    UtilsX509I &x509SetVersion(UtilsX509Version v) override;

    UtilsX509I &x509SetCA(const Jchar *v) override;

    UtilsX509I &x509SetRequest(const Jchar *v) override;

    UtilsX509I &x509SetExpDate(Jlong v) override;

    UtilsX509I &x509SetSignObject(UtilsX509SignI *v) override;

    Jbool x509End(std::string &ret) override;
};

UtilsX509WithOpenSSL::UtilsX509WithOpenSSL()
        : mX509Ctx{}
          , mEvpPKey{}
          , mRSAMeth{}
          , mReadCache{}
{
}

Jbool UtilsX509WithOpenSSL::x509Init()
{
    this->mEvpPKey = EVP_PKEY_new();
    if (this->mEvpPKey == nullptr)
        return false;

    this->mX509Ctx = X509_new();
    return (this->mX509Ctx != nullptr);
}

UtilsX509I &UtilsX509WithOpenSSL::x509SetVersion(UtilsX509Version v)
{
    if (v == UtilsX509Version::V1)
        X509_set_version(this->mX509Ctx, 0);
    else if (v == UtilsX509Version::V2)
        X509_set_version(this->mX509Ctx, 1);
    else if (v == UtilsX509Version::V3)
        X509_set_version(this->mX509Ctx, 2);
    return (*this);
}

UtilsX509I &UtilsX509WithOpenSSL::x509SetCA(const Jchar *v)
{
    Jint vLen = 0;

    X509 *x509CA    = nullptr;
    BIO  *x509CABIO = nullptr;

    X509_NAME      *x509Name      = nullptr;
    X509_EXTENSION *x509Extension = nullptr;
    X509V3_CTX     x509V3Ctx;

    vLen = strlen(v);
    if (vLen != 0)
    {
        do
        {
            x509CABIO = BIO_new_mem_buf(v, vLen);
            if (x509CABIO == nullptr)
                break;

            x509CA = PEM_read_bio_X509(x509CABIO, &x509CA, nullptr, nullptr);
            if (x509CA == nullptr)
                break;

            x509Name = X509_get_subject_name(x509CA);
            X509_set_issuer_name(this->mX509Ctx, x509Name);
        } while (false);

        if (x509CA != nullptr)
            X509_free(x509CA);
        if (x509CABIO != nullptr)
            BIO_free(x509CABIO);
    } else
    {
        x509Name = X509_get_subject_name(this->mX509Ctx);
        X509_set_issuer_name(this->mX509Ctx, x509Name);

        x509V3Ctx.db = nullptr;
        X509V3_set_ctx(&x509V3Ctx, this->mX509Ctx, this->mX509Ctx, nullptr, nullptr, 0);
        x509Extension = X509V3_EXT_conf_nid(nullptr, &x509V3Ctx, NID_subject_key_identifier, "hash");
        X509_add_ext(this->mX509Ctx, x509Extension, -1);
        X509_EXTENSION_free(x509Extension);

        x509V3Ctx.db = nullptr;
        X509V3_set_ctx(&x509V3Ctx, this->mX509Ctx, this->mX509Ctx, nullptr, nullptr, 0);
        x509Extension = X509V3_EXT_conf_nid(nullptr, &x509V3Ctx, NID_authority_key_identifier, "keyid");
        X509_add_ext(this->mX509Ctx, x509Extension, -1);
        X509_EXTENSION_free(x509Extension);

        x509V3Ctx.db = nullptr;
        X509V3_set_ctx(&x509V3Ctx, this->mX509Ctx, this->mX509Ctx, nullptr, nullptr, 0);
        x509Extension = X509V3_EXT_conf_nid(nullptr, &x509V3Ctx, NID_basic_constraints, "CA:TRUE");
        X509_add_ext(this->mX509Ctx, x509Extension, -1);
        X509_EXTENSION_free(x509Extension);
    }
    return (*this);
}

UtilsX509I &UtilsX509WithOpenSSL::x509SetRequest(const Jchar *v)
{
    BIO       *x509BIO  = nullptr;
    X509_REQ  *x509REQ  = nullptr;
    X509_NAME *x509Name = nullptr;

    RSA              *rsa     = nullptr;
    const RSA_METHOD *rsaMeth = nullptr;

    do
    {
        x509BIO = BIO_new_mem_buf(v, strlen(v));
        if (x509BIO == nullptr)
            break;

        PEM_read_bio_X509_REQ(x509BIO, &x509REQ, nullptr, nullptr);
        if (x509REQ == nullptr)
            break;

        x509Name = X509_REQ_get_subject_name(x509REQ);
        if (x509Name == nullptr)
            break;

        this->mEvpPKey = X509_REQ_get_pubkey(x509REQ);
        if (this->mEvpPKey == nullptr)
            break;

        rsa = EVP_PKEY_get0_RSA(this->mEvpPKey);
        if (rsa == nullptr)
            break;

        rsaMeth = RSA_get_method(rsa);
        this->mRSAMeth = RSA_meth_dup(rsaMeth);
        RSA_meth_set_priv_enc(
                this->mRSAMeth,
                [](Jint flen, const Jbyte *from, Jbyte *ret, RSA *, Jint padding) -> Jint {
                    Jbool              state = false;
                    std::vector<Jbyte> result;

                    if (padding == RSA_PKCS1_PADDING)
                        sX509ContextSignObject->x509SignInit(UtilsX509RSAPadding::PKCS1);
                    else if (padding == RSA_NO_PADDING)
                        sX509ContextSignObject->x509SignInit(UtilsX509RSAPadding::NONE);
                    else if (padding == RSA_PKCS1_OAEP_PADDING)
                        sX509ContextSignObject->x509SignInit(UtilsX509RSAPadding::PKCS1_OAEP);
                    else if (padding == RSA_X931_PADDING)
                        sX509ContextSignObject->x509SignInit(UtilsX509RSAPadding::X931);
                    else if (padding == RSA_PKCS1_PSS_PADDING)
                        sX509ContextSignObject->x509SignInit(UtilsX509RSAPadding::PSS);

                    sX509ContextSignObject->x509SignUpdate(from, flen);
                    state = sX509ContextSignObject->x509SignFinal(result);
                    if (!state)
                        return -1;

                    memcpy(ret, result.data(), result.size());
                    return result.size();
                }
        );

        RSA_set_method(rsa, this->mRSAMeth);
        X509_set_pubkey(this->mX509Ctx, this->mEvpPKey);
        X509_set_subject_name(this->mX509Ctx, X509_NAME_dup(x509Name));
    } while (false);

    if (x509REQ != nullptr)
        X509_REQ_free(x509REQ);
    if (x509BIO != nullptr)
        BIO_free(x509BIO);
    return (*this);
}

UtilsX509I &UtilsX509WithOpenSSL::x509SetExpDate(Jlong v)
{
    time_t nowTime;

    nowTime = time(nullptr);
    X509_gmtime_adj(X509_getm_notBefore(this->mX509Ctx), 0);
    X509_gmtime_adj(X509_getm_notAfter(this->mX509Ctx), (static_cast<Jlong>(nowTime) + v));
    return (*this);
}

UtilsX509I &UtilsX509WithOpenSSL::x509SetSignObject(UtilsX509SignI *v)
{
    sX509ContextSignObject = v;
    return (*this);
}

Jbool UtilsX509WithOpenSSL::x509End(std::string &ret)
{
    Jint  readLine = 0;
    Jbool state    = false;

    BIO    *certBIO = nullptr;
    BIGNUM *bignum  = nullptr;

    do
    {
        ret.clear();
        bignum = BN_new();
        if (bignum == nullptr)
            break;

        BN_rand(bignum, BN_SIZE, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
        BN_add_word(bignum, 1);
        BN_to_ASN1_INTEGER(bignum, X509_get_serialNumber(this->mX509Ctx));

        X509_sign(this->mX509Ctx, this->mEvpPKey, EVP_sha1());
        certBIO = BIO_new(BIO_s_mem());
        if (certBIO == nullptr)
            break;

        PEM_write_bio_X509(certBIO, this->mX509Ctx);

        do
        {
            readLine = BIO_read(certBIO, this->mReadCache, sizeof(this->mReadCache));
            ret.append(this->mReadCache, readLine);
        } while (readLine == sizeof(this->mReadCache));

        state = true;
    } while (false);

    if (this->mRSAMeth != nullptr)
        RSA_meth_free(this->mRSAMeth);
    if (this->mX509Ctx != nullptr)
        X509_free(this->mX509Ctx);
    if (this->mEvpPKey != nullptr)
        EVP_PKEY_free(this->mEvpPKey);
    if (bignum != nullptr)
        BN_free(bignum);
    if (certBIO != nullptr)
        BIO_free(certBIO);
    return state;
}

}

}

#endif //CSIGNAPK_UTILSX509WITHOPENSSL_HPP
