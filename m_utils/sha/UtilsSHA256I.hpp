#pragma once

#ifndef CSIGNAPK_UTILSSHA256I_HPP
#define CSIGNAPK_UTILSSHA256I_HPP

namespace m
{

inline namespace apk
{

class UtilsSHA256I
{
public:
    virtual ~UtilsSHA256I();

    virtual Jbool sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) = 0;

    virtual Jbool sha256Ready() = 0;

    virtual Jbool sha256Process(const Jbyte *v, Jint vLen) = 0;

    virtual Jbool sha256Done(std::vector<Jbyte> &ret) = 0;
};

UtilsSHA256I::~UtilsSHA256I() = default;

}

}

#endif //CSIGNAPK_UTILSSHA256I_HPP
