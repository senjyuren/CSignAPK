#pragma once

#ifndef CSIGNAPK_APKLOCALFACTORY_HPP
#define CSIGNAPK_APKLOCALFACTORY_HPP

#include "APKLocalActionI.hpp"
#include "APKLocalFile.hpp"
#include "APKLocalFileSigned.hpp"

#include "APKLocalEnvI.hpp"
#include "APKLocalEnv.hpp"

namespace m
{

inline namespace apk
{

class APKLocalFactory
        : public APKLocalEnvI
{
private:
    std::shared_ptr<APKLocalEnvI> mEnv;

    APKLocalFactory();

public:
    static APKLocalFactory &getInstance();

    APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon> &getFile() override;

    APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &getFileSign() override;

    APKLocalFactory &init(std::shared_ptr<APKLocalEnvI> v);
};

APKLocalFactory::APKLocalFactory()
        : mEnv{}
{
}

APKLocalFactory &APKLocalFactory::getInstance()
{
    static APKLocalFactory *obj = nullptr;
    if (obj == nullptr)
        obj = new APKLocalFactory();
    return (*obj);
}

APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon> &APKLocalFactory::getFile()
{
    return this->mEnv->getFile();
}

APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &APKLocalFactory::getFileSign()
{
    return this->mEnv->getFileSign();
}

APKLocalFactory &APKLocalFactory::init(std::shared_ptr<APKLocalEnvI> v)
{
    this->mEnv = std::move(v);
    return (*this);
}

}

}

#endif //CSIGNAPK_APKLOCALFACTORY_HPP
