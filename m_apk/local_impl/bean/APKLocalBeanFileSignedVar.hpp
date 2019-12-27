#pragma once

#ifndef CSIGNAPK_APKLOCALBEANFILESIGNEDVAR_HPP
#define CSIGNAPK_APKLOCALBEANFILESIGNEDVAR_HPP

namespace m
{

inline namespace apk
{

enum class APKLocalBeanFileSignedVar
        : Juint
{
    ID      = 0,
    FILE_ID = 1,
    NAME    = 2,
    SHA_1   = 3,
    SHA_256 = 4,
};

}

}

#endif //CSIGNAPK_APKLOCALBEANFILESIGNEDVAR_HPP
