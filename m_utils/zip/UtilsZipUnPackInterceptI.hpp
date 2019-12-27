#pragma once

#ifndef CSIGNAPK_UTILSZIPINTERCEPTI_HPP
#define CSIGNAPK_UTILSZIPINTERCEPTI_HPP

namespace m
{

inline namespace apk
{

class UtilsZipUnPackInterceptI
{
public:
    virtual ~UtilsZipUnPackInterceptI();

    virtual void unPackStart(const Jchar *name) = 0;

    virtual void unPackStream(const Jchar *name, const Jbyte *v, Jint vLen) = 0;

    virtual void unPackEnd(const Jchar *name) = 0;
};

UtilsZipUnPackInterceptI::~UtilsZipUnPackInterceptI() = default;

}

}

#endif //CSIGNAPK_UTILSZIPINTERCEPTI_HPP
