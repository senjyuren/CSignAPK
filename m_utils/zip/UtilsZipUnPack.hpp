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
    constexpr static Jint READ_CACHE_SIZE = 1024;

    constexpr static Jchar PATH_SYMBOL[] = "/";

    unzFile       mUnzFile;
    unz_file_info mUnzFileInfo;

    std::string mOutPath;
    Jchar       mFileName[FILE_NAME_SIZE];
    Jchar       mReadCache[READ_CACHE_SIZE];

    std::list<UtilsZipUnPackInterceptI *> mIntercepts;

public:
    UtilsZipUnPack();

    void add(UtilsZipUnPackInterceptI *v) override;

    void clean() override;

    void stream(const Jchar *in, const Jchar *out) override;

    void notifyStart(const Jchar *name, const Jchar *path);

    void notifyEnd(const Jchar *name, const Jchar *path);

    void notifyStream(const Jchar *name, const Jchar *path, const Jbyte *v, Jint vLen);
};

UtilsZipUnPack::UtilsZipUnPack()
        : mUnzFile{}
          , mUnzFileInfo{}
          , mOutPath{}
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

    this->mUnzFile = unzOpen(in);
    if (this->mUnzFile == nullptr)
        return;

    unzGoToFirstFile(this->mUnzFile);
    do
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

        this->mOutPath.clear();
        this->mOutPath.append(out)
                .append(PATH_SYMBOL)
                .append(this->mFileName);
        this->notifyStart(this->mFileName, this->mOutPath.c_str());

        do
        {
            readLen = unzReadCurrentFile(this->mUnzFile, this->mReadCache, sizeof(this->mReadCache));
            if (readLen <= 0)
                break;

            this->notifyStream(
                    this->mFileName,
                    this->mOutPath.c_str(),
                    reinterpret_cast<Jbyte *>(this->mReadCache),
                    readLen
            );
        } while (readLen == sizeof(this->mReadCache));

        this->notifyEnd(this->mFileName, this->mOutPath.c_str());
        unzCloseCurrentFile(this->mUnzFile);
    } while (unzGoToNextFile(this->mUnzFile) == UNZ_OK);

    unzClose(this->mUnzFile);
}

void UtilsZipUnPack::notifyStart(const Jchar *name, const Jchar *path)
{
    for (const auto &v :this->mIntercepts)
        v->unPackStart(name, path);
}

void UtilsZipUnPack::notifyEnd(const Jchar *name, const Jchar *path)
{
    for (const auto &v : this->mIntercepts)
        v->unPackEnd(name, path);
}

void UtilsZipUnPack::notifyStream(const Jchar *name, const Jchar *path, const Jbyte *v, Jint vLen)
{
    for (const auto &intercept : this->mIntercepts)
        intercept->unPackStream(name, path, v, vLen);
}

}

}

#endif //CSIGNAPK_UTILSZIPUNPACK_HPP
