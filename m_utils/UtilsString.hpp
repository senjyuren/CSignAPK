#pragma once

#ifndef CSIGNAPK_UTILSSTRING_HPP
#define CSIGNAPK_UTILSSTRING_HPP

namespace m
{

inline namespace apk
{

class UtilsString
{
private:
    UtilsString();

public:
    static Jbool Equals(const Jchar *v, const Jchar *v1);
};

UtilsString::UtilsString() = default;

Jbool UtilsString::Equals(const Jchar *v, const Jchar *v1)
{
    Jint i     = 0;
    Jint vLen  = 0;
    Jint v1Len = 0;

    if ((v == nullptr) || (v1 == nullptr))
        return false;

    vLen  = strlen(v);
    v1Len = strlen(v1);
    if (vLen != v1Len)
        return false;

    for (i = 0; i < vLen; ++i)
    {
        if (v[i] != v1[i])
            break;
    }

    return (i == vLen);
}

}

}

#endif //CSIGNAPK_UTILSSTRING_HPP
