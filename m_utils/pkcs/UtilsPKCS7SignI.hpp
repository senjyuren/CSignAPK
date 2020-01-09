#pragma once

#ifndef CSIGNAPK_UTILSPKCS7SIGNI_HPP
#define CSIGNAPK_UTILSPKCS7SIGNI_HPP

namespace m
{

inline namespace apk
{

class UtilsPKCS7SignI
{
public:
    virtual ~UtilsPKCS7SignI();

    virtual Jbool pkcs7SignInit(UtilsPKCSRSAPadding v) = 0;

    virtual Jbool pkcs7SignUpdate(const Jbyte *v, Jint vLen) = 0;

    virtual Jbool pkcs7SignFinal(std::vector<Jbyte> &ret) = 0;
};

UtilsPKCS7SignI::~UtilsPKCS7SignI() = default;

}

}

#endif //CSIGNAPK_UTILSPKCS7SIGNI_HPP
