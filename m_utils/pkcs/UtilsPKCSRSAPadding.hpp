#pragma once

#ifndef CSIGNAPK_UTILSPKCSRSAPADDING_HPP
#define CSIGNAPK_UTILSPKCSRSAPADDING_HPP

namespace m
{

inline namespace apk
{

enum class UtilsPKCSRSAPadding
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

#endif //CSIGNAPK_UTILSPKCSRSAPADDING_HPP
