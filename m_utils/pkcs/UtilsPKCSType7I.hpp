#pragma once

#ifndef CSIGNAPK_UTILSPKCSTYPE7I_HPP
#define CSIGNAPK_UTILSPKCSTYPE7I_HPP

namespace m
{

inline namespace apk
{

class UtilsPKCSType7I
{
public:
    virtual ~UtilsPKCSType7I();

    virtual Jbool pkcs7Ready() = 0;

    virtual Jbool pkcs7Process(const Jbyte *v, Jint vLen) = 0;

    virtual Jbool pkcs7Done(std::vector<Jbyte> &ret) = 0;
};

UtilsPKCSType7I::~UtilsPKCSType7I() = default;

}

}

#endif //CSIGNAPK_UTILSPKCSTYPE7I_HPP
