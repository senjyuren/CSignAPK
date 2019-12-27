#pragma once

#ifndef CSIGNAPK_APKLOCALBEANFILECON_HPP
#define CSIGNAPK_APKLOCALBEANFILECON_HPP

namespace m
{

inline namespace apk
{

class APKLocalBeanFileCon
{
private:
    Jint               mID;
    std::string        mName;
    std::vector<Jbyte> mSign;

public:
    APKLocalBeanFileCon();

    APKLocalBeanFileCon &setID(Jint id);

    APKLocalBeanFileCon &setName(const Jchar *name);

    APKLocalBeanFileCon &setSign(const Jbyte *v, Jint vLen);

    Jint getID();

    const std::string &getName();

    const std::vector<Jbyte> &getSign();
};

APKLocalBeanFileCon::APKLocalBeanFileCon()
        : mID{}
          , mName{}
          , mSign{}
{
}

APKLocalBeanFileCon &APKLocalBeanFileCon::setID(Jint id)
{
    this->mID = id;
    return (*this);
}

APKLocalBeanFileCon &APKLocalBeanFileCon::setName(const Jchar *name)
{
    this->mName.clear();
    this->mName.append(name);
    return (*this);
}

APKLocalBeanFileCon &APKLocalBeanFileCon::setSign(const Jbyte *v, Jint vLen)
{
    Jint i = 0;

    this->mSign.clear();
    for (i = 0; i < vLen; ++i)
        this->mSign.push_back(v[i]);
    return (*this);
}

Jint APKLocalBeanFileCon::getID()
{
    return this->mID;
}

const std::string &APKLocalBeanFileCon::getName()
{
    return this->mName;
}

const std::vector<Jbyte> &APKLocalBeanFileCon::getSign()
{
    return this->mSign;
}

}

}

#endif //CSIGNAPK_APKLOCALBEANFILECON_HPP
