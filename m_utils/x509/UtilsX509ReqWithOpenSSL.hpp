#pragma once

#ifndef CSIGNAPK_UTILSX509REQWITHOPENSSL_HPP
#define CSIGNAPK_UTILSX509REQWITHOPENSSL_HPP

namespace m
{

inline namespace apk
{

static UtilsX509SignI *sX509RequestSignObject = nullptr;

class UtilsX509ReqWithOpenSSL
        : public UtilsX509ReqI
{
private:
    constexpr static Jchar X_COUNTRY[]          = "countryName";
    constexpr static Jchar X_PROVINCE[]         = "stateOrProvinceName";
    constexpr static Jchar X_LOCAL[]            = "localityName";
    constexpr static Jchar X_ORANIZATION[]      = "organizationName";
    constexpr static Jchar X_ORANIZATION_UNIT[] = "organizationalUnitName";
    constexpr static Jchar X_COMMON[]           = "commonName";
    constexpr static Jchar X_EMAIL[]            = "emailAddress";

    constexpr static Jint READ_CACHE = 128;

    X509_REQ *mX509Req;
    X509_NAME  *
               mX509Name;
    EVP_PKEY   *mEvpPKey;
    RSA        *mRSA;
    RSA_METHOD *mRSAMeth;

    Jchar mReadCache[READ_CACHE];

public:
    UtilsX509ReqWithOpenSSL();

    Jbool x509ReqInit() override;

    UtilsX509ReqI &x509ReqSetPublicKey(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetVersion(UtilsX509Version v) override;

    UtilsX509ReqI &x509ReqSetCountry(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetProvince(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetCity(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetOranization(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetOranizationUnit(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetCommon(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetEmail(const Jchar *v) override;

    UtilsX509ReqI &x509ReqSetSignObject(UtilsX509SignI *v) override;

    Jbool x509ReqEnd(std::string &ret) override;
};

UtilsX509ReqWithOpenSSL::UtilsX509ReqWithOpenSSL()
        : mX509Req{}
          , mX509Name{}
          , mEvpPKey{}
          , mRSA{}
          , mRSAMeth{}
          , mReadCache{}
{
}

Jbool UtilsX509ReqWithOpenSSL::x509ReqInit()
{
    this->mX509Req = X509_REQ_new();
    if (this->mX509Req == nullptr)
        return false;

    this->mX509Name = X509_REQ_get_subject_name(this->mX509Req);
    if (this->mX509Name == nullptr)
        return false;

    this->mEvpPKey = EVP_PKEY_new();
    return (this->mEvpPKey != nullptr);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetPublicKey(const Jchar *v)
{
    BIO              *pubkeyBIO = nullptr;
    const RSA_METHOD *rsaMethod = nullptr;

    do
    {
        pubkeyBIO = BIO_new_mem_buf(v, strlen(v));
        if (pubkeyBIO == nullptr)
            break;

        PEM_read_bio_RSAPublicKey(pubkeyBIO, &this->mRSA, nullptr, nullptr);
        if (this->mRSA == nullptr)
            break;

        rsaMethod = RSA_get_method(this->mRSA);
        this->mRSAMeth = RSA_meth_dup(rsaMethod);
        RSA_meth_set_priv_enc(
                this->mRSAMeth,
                [](Jint flen, const Jbyte *from, Jbyte *ret, RSA *, Jint padding) -> Jint {
                    Jbool              state = false;
                    std::vector<Jbyte> result;

                    if (padding == RSA_PKCS1_PADDING)
                        sX509RequestSignObject->x509SignInit(UtilsX509RSAPadding::PKCS1);
                    else if (padding == RSA_NO_PADDING)
                        sX509RequestSignObject->x509SignInit(UtilsX509RSAPadding::NONE);
                    else if (padding == RSA_PKCS1_OAEP_PADDING)
                        sX509RequestSignObject->x509SignInit(UtilsX509RSAPadding::PKCS1_OAEP);
                    else if (padding == RSA_X931_PADDING)
                        sX509RequestSignObject->x509SignInit(UtilsX509RSAPadding::X931);
                    else if (padding == RSA_PKCS1_PSS_PADDING)
                        sX509RequestSignObject->x509SignInit(UtilsX509RSAPadding::PSS);

                    sX509RequestSignObject->x509SignUpdate(from, flen);
                    state = sX509RequestSignObject->x509SignFinal(result);
                    if (!state)
                        return -1;

                    memcpy(ret, result.data(), result.size());
                    return result.size();
                }
        );

        RSA_set_method(this->mRSA, this->mRSAMeth);
        EVP_PKEY_assign(this->mEvpPKey, EVP_PKEY_RSA, reinterpret_cast<Jchar *>(this->mRSA));
        X509_REQ_set_pubkey(this->mX509Req, this->mEvpPKey);
    } while (false);

    if (pubkeyBIO != nullptr)
        BIO_free(pubkeyBIO);
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetVersion(UtilsX509Version v)
{
    if (v == UtilsX509Version::V1)
        X509_REQ_set_version(this->mX509Req, 0);
    else if (v == UtilsX509Version::V2)
        X509_REQ_set_version(this->mX509Req, 1);
    else if (v == UtilsX509Version::V3)
        X509_REQ_set_version(this->mX509Req, 2);
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetCountry(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_COUNTRY,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetProvince(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_PROVINCE,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetCity(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_LOCAL,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetOranization(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_ORANIZATION,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetOranizationUnit(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_ORANIZATION_UNIT,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetCommon(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_COMMON,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetEmail(const Jchar *v)
{
    X509_NAME_add_entry_by_txt(
            this->mX509Name,
            X_EMAIL,
            (static_cast<Juint>(MBSTRING_FLAG) | 1u),
            reinterpret_cast<Jbyte *>(const_cast<Jchar *>(v)),
            -1,
            -1,
            0
    );
    return (*this);
}

UtilsX509ReqI &UtilsX509ReqWithOpenSSL::x509ReqSetSignObject(UtilsX509SignI *v)
{
    sX509RequestSignObject = v;
    return (*this);
}

Jbool UtilsX509ReqWithOpenSSL::x509ReqEnd(std::string &ret)
{
    Jint  readLine = 0;
    Jbool state    = false;

    BIO *reqBIO = nullptr;

    do
    {
        ret.clear();
        X509_REQ_sign(this->mX509Req, this->mEvpPKey, EVP_sha1());
        reqBIO = BIO_new(BIO_s_mem());
        if (reqBIO == nullptr)
            break;

        PEM_write_bio_X509_REQ(reqBIO, this->mX509Req);

        do
        {
            readLine = BIO_read(reqBIO, this->mReadCache, sizeof(this->mReadCache));
            ret.append(this->mReadCache, readLine);
        } while (readLine == sizeof(this->mReadCache));

        state = true;
    } while (false);

    if (this->mRSAMeth != nullptr)
        RSA_meth_free(this->mRSAMeth);
    if (this->mEvpPKey != nullptr)
        EVP_PKEY_free(this->mEvpPKey);
    if (reqBIO != nullptr)
        BIO_free(reqBIO);
    if (this->mX509Req != nullptr)
        X509_REQ_free(this->mX509Req);
    return state;
}

}

}

#endif //CSIGNAPK_UTILSX509REQWITHOPENSSL_HPP
