#pragma once

#ifndef CSIGNAPK_UTILSX509SIGNI_HPP
#define CSIGNAPK_UTILSX509SIGNI_HPP

namespace m
{

inline namespace apk
{

class UtilsX509SignI
{
public:
    virtual ~UtilsX509SignI();

    virtual Jbool x509SignInit(UtilsX509RSAPadding v) = 0;

    virtual Jbool x509SignUpdate(const Jbyte *v, Jint vLen) = 0;

    virtual Jbool x509SignFinal(std::vector<Jbyte> &ret) = 0;
};

UtilsX509SignI::~UtilsX509SignI() = default;

}

}

#endif //CSIGNAPK_UTILSX509SIGNI_HPP
