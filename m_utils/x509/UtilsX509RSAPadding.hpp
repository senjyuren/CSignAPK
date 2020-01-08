#pragma once

#ifndef CSIGNAPK_UTILSX509RSAPADDING_HPP
#define CSIGNAPK_UTILSX509RSAPADDING_HPP

namespace m
{

inline namespace apk
{

enum class UtilsX509RSAPadding
        : Juint
{
    NONE       = 0,
    PKCS1      = 1,
    PKCS1_OAEP = 2,
    PSS        = 3,
    X931       = 4,
};

}

}

#endif //CSIGNAPK_UTILSX509RSAPADDING_HPP
