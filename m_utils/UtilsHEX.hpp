#pragma once

#ifndef CSIGNAPK_UTILSHEX_HPP
#define CSIGNAPK_UTILSHEX_HPP

namespace m
{

inline namespace apk
{

class UtilsHEX
{
private:
    constexpr static Jchar DICTIONARY[] = "0123456789ABCDEF";

    UtilsHEX();

public:
    static Jbool vTowOne(const Jchar *v, std::vector<Jbyte> &ret);

    static Jbool vOneTwo(const Jbyte *v, Jint vLen, std::string &ret);
};

UtilsHEX::UtilsHEX() = default;

Jbool UtilsHEX::vTowOne(const Jchar *v, std::vector<Jbyte> &ret)
{
    Jint  i    = 0;
    Jint  vLen = 0;
    Jbyte tmp  = 0;

    vLen = strlen(v);
    ret.clear();
    ret.resize(vLen / 2);
    for (i = 0; i < vLen; i += 2)
    {
        tmp     = v[i];
        if (tmp > 0x3F)
            tmp = toupper(tmp) - ('A' - 0x0A);
        else
            tmp &= 0x0Fu;
        tmp <<= 4u;
        ret[i / 2] = tmp;

        tmp     = v[i + 1];
        if (tmp > 0x3F)
            tmp = toupper(tmp) - ('A' - 0x0A);
        else
            tmp &= 0x0Fu;
        ret[i / 2] += tmp;
    }

    return (!ret.empty());
}

Jbool UtilsHEX::vOneTwo(const Jbyte *v, Jint vLen, std::string &ret)
{
    Jint nCounter = 0;

    ret.clear();
    ret.resize(vLen * 2);
    for (nCounter = 0; nCounter < vLen; ++nCounter)
    {
        ret[2 * nCounter]     = DICTIONARY[(v[nCounter] >> 4u)];
        ret[2 * nCounter + 1] = DICTIONARY[(v[nCounter] & 0x0Fu)];
    }

    return (!ret.empty());
}

}

}

#endif //CSIGNAPK_UTILSHEX_HPP
