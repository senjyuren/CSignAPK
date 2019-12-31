#pragma once

#ifndef CSIGNAPK_APKSIGNEDCERTBLOCKI_HPP
#define CSIGNAPK_APKSIGNEDCERTBLOCKI_HPP

namespace m
{

inline namespace apk
{

class APKSignedCertBlockI
{
public:
    virtual ~APKSignedCertBlockI();

    virtual void certContentStart(const Jchar *name) = 0;

    virtual void certContentUpdate(const Jchar *name, const Jchar *v, Jint vLen) = 0;

    virtual void certContentEnd(const Jchar *name) = 0;
};

APKSignedCertBlockI::~APKSignedCertBlockI() = default;

}

}

#endif //CSIGNAPK_APKSIGNEDCERTBLOCKI_HPP
