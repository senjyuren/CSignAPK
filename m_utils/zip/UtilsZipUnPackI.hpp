#pragma once

#ifndef CSIGNAPK_UTILSZIPUNPACKI_HPP
#define CSIGNAPK_UTILSZIPUNPACKI_HPP

namespace m
{

inline namespace apk
{

class UtilsZipUnPackI
{
public:
    virtual ~UtilsZipUnPackI();

    virtual void addInBacklist(const Jchar *v) = 0;

    virtual void addIntercept(UtilsZipUnPackInterceptI *v) = 0;

    virtual void cleanIntercept() = 0;

    virtual void streamIntercept(const Jchar *in, const Jchar *out) = 0;
};

UtilsZipUnPackI::~UtilsZipUnPackI() = default;

}

}

#endif //CSIGNAPK_UTILSZIPUNPACKI_HPP
