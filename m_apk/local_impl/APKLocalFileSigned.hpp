#pragma once

#ifndef CSIGNAPK_APKLOCALFILESIGNED_HPP
#define CSIGNAPK_APKLOCALFILESIGNED_HPP

#include "bean/APKLocalBeanFileSignedVar.hpp"
#include "bean/APKLocalBeanFileSignedCon.hpp"

namespace m
{

inline namespace apk
{

class APKLocalFileSigned
        : public APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon>
{
private:
    constexpr static Jint INDEX_START = 1;

    Jint                                 mIDAutoIndex;
    std::list<APKLocalBeanFileSignedCon> mStores;

public:
    APKLocalFileSigned();

    void start(const Jchar *path) override;

    Jbool select(std::vector<APKLocalBeanFileSignedCon> &array) override;

    Jbool select(
            APKLocalBeanFileSignedVar cond,
            APKLocalBeanFileSignedCon &value,
            std::vector<APKLocalBeanFileSignedCon> &array
    ) override;

    Jbool insert(APKLocalBeanFileSignedCon &value) override;

    Jbool update(
            APKLocalBeanFileSignedVar cond,
            APKLocalBeanFileSignedCon &value,
            APKLocalBeanFileSignedCon &write
    ) override;

    Jbool remove() override;

    Jbool remove(APKLocalBeanFileSignedVar cond, APKLocalBeanFileSignedCon &value) override;
};

APKLocalFileSigned::APKLocalFileSigned()
        : mIDAutoIndex{INDEX_START}
          , mStores{}
{
}

void APKLocalFileSigned::start(const Jchar *path)
{
}

Jbool APKLocalFileSigned::select(std::vector<APKLocalBeanFileSignedCon> &array)
{
    for (auto &con : this->mStores)
    {
        if (con.getID() == 0)
            continue;

        array.push_back(con);
    }
    return true;
}

Jbool APKLocalFileSigned::select(
        APKLocalBeanFileSignedVar cond,
        APKLocalBeanFileSignedCon &value,
        std::vector<APKLocalBeanFileSignedCon> &array
)
{
    for (auto &con : this->mStores)
    {
        if (con.getID() == 0)
            continue;

        if (cond == APKLocalBeanFileSignedVar::ID)
        {
            if (con.getID() != value.getID())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
        {
            if (con.getFileID() != value.getFileID())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::NAME)
        {
            if (con.getName() != value.getName())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_1)
        {
            if (con.getSHA1() != value.getSHA1())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_256)
        {
            if (con.getSHA256() != value.getSHA256())
                continue;
        } else
        {
            continue;
        }

        array.push_back(con);
    }

    return true;
}

Jbool APKLocalFileSigned::insert(APKLocalBeanFileSignedCon &value)
{
    this->mStores.push_back(value.setID(this->mIDAutoIndex));
    ++this->mIDAutoIndex;
    return true;
}

Jbool APKLocalFileSigned::update(
        APKLocalBeanFileSignedVar cond,
        APKLocalBeanFileSignedCon &value,
        APKLocalBeanFileSignedCon &write
)
{
    for (auto &con : this->mStores)
    {
        if (con.getID() == 0)
            continue;

        if (cond == APKLocalBeanFileSignedVar::ID)
        {
            if (con.getID() != value.getID())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
        {
            if (con.getFileID() != value.getFileID())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::NAME)
        {
            if (con.getName() != value.getName())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_1)
        {
            if (con.getSHA1() != value.getSHA1())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_256)
        {
            if (con.getSHA256() != value.getSHA256())
                continue;
        } else
        {
            continue;
        }

        con.setFileID(write.getFileID())
                .setName(write.getName().c_str())
                .setSHA1(write.getSHA1().data(), write.getSHA1().size())
                .setSHA256(write.getSHA256().data(), write.getSHA256().size());
    }

    return true;
}

Jbool APKLocalFileSigned::remove()
{
    this->mIDAutoIndex = INDEX_START;
    this->mStores.clear();
    return true;
}

Jbool APKLocalFileSigned::remove(APKLocalBeanFileSignedVar cond, APKLocalBeanFileSignedCon &value)
{
    for (auto &con : this->mStores)
    {
        if (con.getID() == 0)
            continue;

        if (cond == APKLocalBeanFileSignedVar::ID)
        {
            if (con.getID() != value.getID())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
        {
            if (con.getFileID() != value.getFileID())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::NAME)
        {
            if (con.getName() != value.getName())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_1)
        {
            if (con.getSHA1() != value.getSHA1())
                continue;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_256)
        {
            if (con.getSHA256() != value.getSHA256())
                continue;
        } else
        {
            continue;
        }

        con.setID(0);
    }
    return true;
}

}

}

#endif //CSIGNAPK_APKLOCALFILESIGNED_HPP
