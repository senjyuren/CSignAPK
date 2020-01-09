#pragma once

#ifndef CSIGNAPK_UTILSSHA1I_HPP
#define CSIGNAPK_UTILSSHA1I_HPP

namespace m
{

inline namespace apk
{

class UtilsSHA1I
{
public:
    virtual ~UtilsSHA1I();

    virtual Jbool sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) = 0;

    virtual Jbool sha1Init() = 0;

    virtual Jbool sha1Update(const Jbyte *v, Jint vLen) = 0;

    virtual Jbool sha1Final(std::vector<Jbyte> &ret) = 0;
};

UtilsSHA1I::~UtilsSHA1I() = default;

}

}

#endif //CSIGNAPK_UTILSSHA1I_HPP
