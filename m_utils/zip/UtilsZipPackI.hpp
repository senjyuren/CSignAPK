#pragma once

#ifndef CSIGNAPK_UTILSZIPPACKI_HPP
#define CSIGNAPK_UTILSZIPPACKI_HPP

namespace m
{

inline namespace apk
{

class UtilsZipPackI
{
public:
    virtual ~UtilsZipPackI();

    virtual void addFile(const Jchar *v) = 0;

    virtual void pack(const Jchar *in, const Jchar *out) = 0;
};

UtilsZipPackI::~UtilsZipPackI() = default;

}

}

#endif //CSIGNAPK_UTILSZIPPACKI_HPP
