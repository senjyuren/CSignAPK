#pragma once

#ifndef CSIGNAPK_UTILSSHA_HPP
#define CSIGNAPK_UTILSSHA_HPP

#include "sha/UtilsSHAAdapter.hpp"

namespace m
{

inline namespace apk
{

class UtilsSHA
        : public UtilsSHA1I
          , public UtilsSHA256I
{
private:
    std::shared_ptr<UtilsSHAAdapter> mAdapter;

    UtilsSHA();

public:
    static UtilsSHA &getInstance();

    Jbool sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) override;

    Jbool sha1Ready() override;

    Jbool sha1Process(const Jbyte *v, Jint vLen) override;

    Jbool sha1Done(std::vector<Jbyte> &ret) override;

    Jbool sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret) override;

    Jbool sha256Ready() override;

    Jbool sha256Process(const Jbyte *v, Jint vLen) override;

    Jbool sha256Done(std::vector<Jbyte> &ret) override;

    void create(std::shared_ptr<UtilsSHAAdapter> adapter);
};

UtilsSHA::UtilsSHA()
        : mAdapter{}
{
}

UtilsSHA &UtilsSHA::getInstance()
{
    static UtilsSHA *obj = nullptr;
    if (obj == nullptr)
        obj = new UtilsSHA();
    return (*obj);
}

Jbool UtilsSHA::sha1(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret)
{
    return this->mAdapter->getSHA1().sha1(v, vLen, ret);
}

Jbool UtilsSHA::sha1Ready()
{
    return this->mAdapter->getSHA1().sha1Ready();
}

Jbool UtilsSHA::sha1Process(const Jbyte *v, Jint vLen)
{
    return this->mAdapter->getSHA1().sha1Process(v, vLen);
}

Jbool UtilsSHA::sha1Done(std::vector<Jbyte> &ret)
{
    return this->mAdapter->getSHA1().sha1Done(ret);
}

Jbool UtilsSHA::sha256(const Jbyte *v, Jint vLen, std::vector<Jbyte> &ret)
{
    return this->mAdapter->getSHA256().sha256(v, vLen, ret);
}

Jbool UtilsSHA::sha256Ready()
{
    return this->mAdapter->getSHA256().sha256Ready();
}

Jbool UtilsSHA::sha256Process(const Jbyte *v, Jint vLen)
{
    return this->mAdapter->getSHA256().sha256Process(v, vLen);
}

Jbool UtilsSHA::sha256Done(std::vector<Jbyte> &ret)
{
    return this->mAdapter->getSHA256().sha256Done(ret);
}

void UtilsSHA::create(std::shared_ptr<UtilsSHAAdapter> adapter)
{
    this->mAdapter = std::move(adapter);
}

}

}

#endif //CSIGNAPK_UTILSSHA_HPP
