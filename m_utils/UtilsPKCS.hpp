#pragma once

#ifndef CSIGNAPK_UTILSPKCS_HPP
#define CSIGNAPK_UTILSPKCS_HPP

#include "pkcs/UtilsPKCSAdapter.hpp"

namespace m
{

inline namespace apk
{

class UtilsPKCS
        : public UtilsPKCSType7I
{
private:
    std::shared_ptr<UtilsPKCSAdapter> mAdapter;

    UtilsPKCS();

public:
    static UtilsPKCS &getInstance();

    Jbool pkcs7Ready() override;

    Jbool pkcs7Process(const Jbyte *v, Jint vLen) override;

    Jbool pkcs7Done(std::vector<Jbyte> &ret) override;

    void create(std::shared_ptr<UtilsPKCSAdapter> adapter);
};

UtilsPKCS::UtilsPKCS()
        : mAdapter{}
{
}

UtilsPKCS &UtilsPKCS::getInstance()
{
    static UtilsPKCS *obj = nullptr;
    if (obj == nullptr)
        obj = new UtilsPKCS();
    return (*obj);
}

Jbool UtilsPKCS::pkcs7Ready()
{
    return this->mAdapter->getType7().pkcs7Ready();
}

Jbool UtilsPKCS::pkcs7Process(const Jbyte *v, Jint vLen)
{
    return this->mAdapter->getType7().pkcs7Process(v, vLen);
}

Jbool UtilsPKCS::pkcs7Done(std::vector<Jbyte> &ret)
{
    return this->mAdapter->getType7().pkcs7Done(ret);
}

void UtilsPKCS::create(std::shared_ptr<UtilsPKCSAdapter> adapter)
{
    this->mAdapter = std::move(adapter);
}

}

}

#endif //CSIGNAPK_UTILSPKCS_HPP
