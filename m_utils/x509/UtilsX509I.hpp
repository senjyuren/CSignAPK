#pragma once

#ifndef CSIGNAPK_UTILSX509I_HPP
#define CSIGNAPK_UTILSX509I_HPP

namespace m
{

inline namespace apk
{

class UtilsX509I
{
public:
    virtual ~UtilsX509I();

    virtual Jbool x509Init() = 0;

    virtual UtilsX509I &x509SetVersion(UtilsX509Version v) = 0;

    virtual UtilsX509I &x509SetCA(const Jchar *v) = 0;

    virtual UtilsX509I &x509SetRequest(const Jchar *v) = 0;

    virtual UtilsX509I &x509SetExpDate(Jlong v) = 0;

    virtual UtilsX509I &x509SetSignObject(UtilsX509SignI *v) = 0;

    virtual Jbool x509End(std::string &ret) = 0;
};

UtilsX509I::~UtilsX509I() = default;

}

}

#endif //CSIGNAPK_UTILSX509I_HPP
