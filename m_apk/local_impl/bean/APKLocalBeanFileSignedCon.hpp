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
    Jint        mID;
    Jint        mFileID;
    std::string mName;
    std::string mSHA1;
    std::string mSHA256;

public:
    APKLocalBeanFileSignedCon();

    APKLocalBeanFileSignedCon &setID(Jint id);

    APKLocalBeanFileSignedCon &setFileID(Jint id);

    APKLocalBeanFileSignedCon &setName(const Jchar *v);

    APKLocalBeanFileSignedCon &setSHA1(const Jchar *v);

    APKLocalBeanFileSignedCon &setSHA256(const Jchar *v);

    Jint getID();

    Jint getFileID();

    const std::string &getName();

    const std::string &getSHA1();

    const std::string &getSHA256();
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

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setSHA1(const Jchar *v)
{
    this->mSHA1.append(v);
    return (*this);
}

APKLocalBeanFileSignedCon &APKLocalBeanFileSignedCon::setSHA256(const Jchar *v)
{
    this->mSHA256.append(v);
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

const std::string &APKLocalBeanFileSignedCon::getSHA1()
{
    return this->mSHA1;
}

const std::string &APKLocalBeanFileSignedCon::getSHA256()
{
    return this->mSHA256;
}

}

}

#endif //CSIGNAPK_APKLOCALBEANFILESIGNEDCON_HPP
