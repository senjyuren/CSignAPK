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

    virtual void add(UtilsZipUnPackInterceptI *v) = 0;

    virtual void clean() = 0;

    virtual void stream(const Jchar *in, const Jchar *out) = 0;
};

UtilsZipUnPackI::~UtilsZipUnPackI() = default;

}

}

#endif //CSIGNAPK_UTILSZIPUNPACKI_HPP
