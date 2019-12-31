#pragma once

#ifndef CSIGNAPK_APKSIGNEDSTREAMI_HPP
#define CSIGNAPK_APKSIGNEDSTREAMI_HPP

namespace m
{

inline namespace apk
{

class APKSignedStreamI
{
public:
    virtual ~APKSignedStreamI();

    virtual void signHeadStream(const Jchar *name, const Jchar *v, Jint vLen) = 0;

    virtual void signContentStream(const Jchar *name, const Jchar *v, Jint vLen) = 0;

    virtual void signStreamEnd() = 0;
};

APKSignedStreamI::~APKSignedStreamI() = default;

}

}

#endif //CSIGNAPK_APKSIGNEDSTREAMI_HPP
