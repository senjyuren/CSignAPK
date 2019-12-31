#pragma once

#ifndef CSIGNAPK_UTILSZIPUNPACK_HPP
#define CSIGNAPK_UTILSZIPUNPACK_HPP

#include <unzip.h>

namespace m
{

inline namespace utils
{

class UtilsZipUnPack
        : public UtilsZipUnPackI
{
private:
    constexpr static Jint FILE_NAME_SIZE  = 256;
    constexpr static Jint READ_CACHE_SIZE = 2048;

    unzFile       mUnzFile;
    unz_file_info mUnzFileInfo;

    Jchar mFileName[FILE_NAME_SIZE];
    Jchar mReadCache[READ_CACHE_SIZE];

    std::list<UtilsZipUnPackInterceptI *> mIntercepts;

public:
    UtilsZipUnPack();

    void add(UtilsZipUnPackInterceptI *v) override;

    void clean() override;

    void stream(const Jchar *in, const Jchar *out) override;

    void notifyStart(const Jchar *name);

    void notifyEnd(const Jchar *name);

    void notifyStream(const Jchar *name, const Jbyte *v, Jint vLen);
};

UtilsZipUnPack::UtilsZipUnPack()
        : mUnzFile{}
          , mUnzFileInfo{}
          , mFileName{}
          , mReadCache{}
{
}

void UtilsZipUnPack::add(UtilsZipUnPackInterceptI *v)
{
    this->mIntercepts.push_back(v);
}

void UtilsZipUnPack::clean()
{
    this->mIntercepts.clear();
}

void UtilsZipUnPack::stream(const Jchar *in, const Jchar *out)
{
    Jint readLen = 0;

    printf("%s\r\n", out);
    this->mUnzFile = unzOpen(in);
    if (this->mUnzFile == nullptr)
        return;

    unzGoToFirstFile(this->mUnzFile);
    while (unzGoToNextFile(this->mUnzFile) == UNZ_OK)
    {
        if (unzGetCurrentFileInfo(
                this->mUnzFile,
                &this->mUnzFileInfo,
                this->mFileName,
                sizeof(this->mFileName),
                nullptr,
                0,
                nullptr,
                0
        ) != UNZ_OK)
            continue;

        if (unzOpenCurrentFile(this->mUnzFile) != UNZ_OK)
            continue;

        this->notifyStart(this->mFileName);

        do
        {
            readLen = unzReadCurrentFile(this->mUnzFile, this->mReadCache, sizeof(this->mReadCache));
            if (readLen <= 0)
                break;

            this->notifyStream(this->mFileName, reinterpret_cast<Jbyte *>(this->mReadCache), readLen);
        } while (readLen == sizeof(this->mReadCache));

        this->notifyEnd(this->mFileName);
        unzCloseCurrentFile(this->mUnzFile);
    }

    unzClose(this->mUnzFile);
}

void UtilsZipUnPack::notifyStart(const Jchar *name)
{
    for (const auto &v :this->mIntercepts)
        v->unPackStart(name);
}

void UtilsZipUnPack::notifyEnd(const Jchar *name)
{
    for (const auto &v : this->mIntercepts)
        v->unPackEnd(name);
}

void UtilsZipUnPack::notifyStream(const Jchar *name, const Jbyte *v, Jint vLen)
{
    for (const auto &intercept : this->mIntercepts)
        intercept->unPackStream(name, v, vLen);
}

}

}

#endif //CSIGNAPK_UTILSZIPUNPACK_HPP
