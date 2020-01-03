#pragma once

#ifndef CSIGNAPK_APKLOCALFILE_HPP
#define CSIGNAPK_APKLOCALFILE_HPP

#include "bean/APKLocalBeanFileCon.hpp"
#include "bean/APKLocalBeanFileVar.hpp"

namespace m
{

inline namespace apk
{

class APKLocalFile
        : public APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon>
{
private:
    constexpr static Jint INDEX_START = 1;

    Jint                           mIDAutoIndex;
    std::list<APKLocalBeanFileCon> mSotres;

public:
    APKLocalFile();

    void start(const Jchar *path) override;

    Jbool select(std::vector<APKLocalBeanFileCon> &array) override;

    Jbool select(
            APKLocalBeanFileVar cond,
            APKLocalBeanFileCon &value,
            std::vector<APKLocalBeanFileCon> &array
    ) override;

    Jbool insert(APKLocalBeanFileCon &value) override;

    Jbool update(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value, APKLocalBeanFileCon &write) override;

    Jbool remove() override;

    Jbool remove(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value) override;
};

APKLocalFile::APKLocalFile()
        : mIDAutoIndex{INDEX_START}
          , mSotres{}
{
}

void APKLocalFile::start(const Jchar *path)
{
}

Jbool APKLocalFile::select(std::vector<APKLocalBeanFileCon> &array)
{
    for (auto &con : this->mSotres)
    {
        if (con.getID() == 0)
            continue;

        array.push_back(con);
    }
    return true;
}

Jbool APKLocalFile::select(
        APKLocalBeanFileVar cond,
        APKLocalBeanFileCon &value,
        std::vector<APKLocalBeanFileCon> &array
)
{
    for (auto &con : this->mSotres)
    {
        if (con.getID() == 0)
            continue;

        if (cond == APKLocalBeanFileVar::ID)
        {
            if (value.getID() != con.getID())
                continue;
        } else if (cond == APKLocalBeanFileVar::FILE_NAME)
        {
            if (value.getName() != con.getName())
                continue;
        } else if (cond == APKLocalBeanFileVar::FILE_SIGN)
        {
            if (value.getSign() != con.getSign())
                continue;
        } else
        {
            continue;
        }

        array.push_back(con);
    }
    return true;
}

Jbool APKLocalFile::insert(APKLocalBeanFileCon &value)
{
    this->mSotres.push_back(value.setID(this->mIDAutoIndex));
    ++this->mIDAutoIndex;
    return true;
}

Jbool APKLocalFile::update(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value, APKLocalBeanFileCon &write)
{
    for (auto &con : this->mSotres)
    {
        if (con.getID() == 0)
            continue;

        if (cond == APKLocalBeanFileVar::ID)
        {
            if (value.getID() != con.getID())
                continue;
        } else if (cond == APKLocalBeanFileVar::FILE_NAME)
        {
            if (value.getName() != con.getName())
                continue;
        } else if (cond == APKLocalBeanFileVar::FILE_SIGN)
        {
            if (value.getSign() != con.getSign())
                continue;
        } else
        {
            continue;
        }

        con.setName(write.getName().c_str())
                .setSign(write.getSign().data(), write.getSign().size());
    }
    return true;
}

Jbool APKLocalFile::remove()
{
    this->mIDAutoIndex = INDEX_START;
    this->mSotres.clear();
    return true;
}

Jbool APKLocalFile::remove(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value)
{
    for (APKLocalBeanFileCon &con : this->mSotres)
    {
        if (cond == APKLocalBeanFileVar::ID)
        {
            if (value.getID() != con.getID())
                continue;
        } else if (cond == APKLocalBeanFileVar::FILE_NAME)
        {
            if (value.getName() != con.getName())
                continue;
        } else if (cond == APKLocalBeanFileVar::FILE_SIGN)
        {
            if (value.getSign() != con.getSign())
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

#endif //CSIGNAPK_APKLOCALFILE_HPP
