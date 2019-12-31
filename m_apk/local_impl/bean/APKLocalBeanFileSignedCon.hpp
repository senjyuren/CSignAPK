#pragma once

#ifndef CSIGNAPK_APKLOCALBEANFILESIGNEDCON_HPP
#define CSIGNAPK_APKLOCALBEANFILESIGNEDCON_HPP

namespace m
{

inline namespace apk
{

class APKLocalBeanFileSignedCon
{
private:
    Jint               mID;
    Jint               mFileID;
    std::string        mName;
    std::vector<Jbyte> mSHA1;
    std::vector<Jbyte> mSHA256;

public:
    APKLocalBeanFileSignedCon();

    APKLocalBeanFileSignedCon &setID(Jint id);

    APKLocalBeanFileSignedCon &setFileID(Jint id);

    APKLocalBeanFileSignedCon &setName(const Jchar *v);

    APKLocalBeanFileSignedCon &setSHA1(const Jbyte *v, Jint vLen);

    APKLocalBeanFileSignedCon &setSHA256(const Jbyte *v, Jint vLen);

    Jint getID();

    Jint getFileID();

    const std::string &getName();

    const std::vector<Jbyte> &getSHA1();

    const std::vector<Jbyte> &getSHA256();
};

APKLocalBeanFileSignedCon::APKLocalBeanFileSignedCon()
        : mID{}
          , mFileID{}
          , mName{}
          , mSHA1{}
          , mSHA256{}
{
}

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setID(Jint id)
{
    this->mID = id;
    return (*this);
}

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setFileID(Jint id)
{
    this->mFileID = id;
    return (*this);
}

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setName(const Jchar *v)
{
    this->mName.append(v);
    return (*this);
}

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setSHA1(const Jbyte *v, Jint vLen)
{
    Jint i = 0;

    for (i = 0; i < vLen; ++i)
        this->mSHA1.push_back(v[i]);
    return (*this);
}

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setSHA256(const Jbyte *v, Jint vLen)
{
    Jint i = 0;

    for (i = 0; i < vLen; ++i)
        this->mSHA256.push_back(v[i]);
    return (*this);
}

Jint APKLocalBeanFileSignedCon::getID()
{
    return this->mID;
}

Jint APKLocalBeanFileSignedCon::getFileID()
{
    return this->mFileID;
}

const std::string &APKLocalBeanFileSignedCon::getName()
{
    return this->mName;
}

const std::vector<Jbyte> &APKLocalBeanFileSignedCon::getSHA1()
{
    return this->mSHA1;
}

const std::vector<Jbyte> &APKLocalBeanFileSignedCon::getSHA256()
{
    return this->mSHA256;
}

}

}

#endif //CSIGNAPK_APKLOCALBEANFILESIGNEDCON_HPP
