#pragma once

#ifndef CSIGNAPK_APKLOCALENVI_HPP
#define CSIGNAPK_APKLOCALENVI_HPP

namespace m
{

inline namespace apk
{

class APKLocalEnvI
{
public:
    virtual ~APKLocalEnvI();

    virtual APKLocalActionI <APKLocalBeanFileVar, APKLocalBeanFileCon> &getFile() = 0;

    virtual APKLocalActionI <APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &getFileSign() = 0;
};

APKLocalEnvI::~APKLocalEnvI() = default;

}

}

#endif //CSIGNAPK_APKLOCALENVI_HPP
