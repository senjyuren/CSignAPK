#pragma once

#ifndef CSIGNAPK_UTILSZIPUNPACKZLIB_HPP
#define CSIGNAPK_UTILSZIPUNPACKZLIB_HPP

#include <unzip.h>

namespace m
{

inline namespace utils
{

class UtilsZipUnPackZLIB
        : public UtilsZipUnPackI
{
private:
    constexpr static Jint FILE_NAME_SIZE  = 256;
    constexpr static Jint READ_CACHE_SIZE = 2048;

    constexpr static Jchar PATH_SYMBOL[] = "/";

    std::vector<std::string> &mZipAllFilePath;
    std::vector<std::string> &mRootAllFilePath;

    Jchar mFileName[FILE_NAME_SIZE];
    Jchar mReadCache[READ_CACHE_SIZE];

    std::list<UtilsZipUnPackInterceptI *> mIntercepts;
    std::list<std::string>                mFileNameBacklist;

public:
    UtilsZipUnPackZLIB(std::vector<std::string> &zipAllPath, std::vector<std::string> &rootAllPath);

    void addInBacklist(const Jchar *v) override;

    void addIntercept(UtilsZipUnPackInterceptI *v) override;

    void cleanIntercept() override;

    void streamIntercept(const Jchar *in, const Jchar *out) override;

    void notifyStart(const Jchar *name, const Jchar *path);

    void notifyEnd(const Jchar *name, const Jchar *path);

    void notifyStream(const Jchar *name, const Jchar *path, const Jbyte *v, Jint vLen);
};

UtilsZipUnPackZLIB::UtilsZipUnPackZLIB(std::vector<std::string> &zipAllPath, std::vector<std::string> &rootAllPath)
        : mZipAllFilePath{zipAllPath}
          , mRootAllFilePath{rootAllPath}
          , mFileName{}
          , mReadCache{}
          , mIntercepts{}
          , mFileNameBacklist{}
{
}

void UtilsZipUnPackZLIB::addInBacklist(const Jchar *v)
{
    this->mFileNameBacklist.emplace_back(v);
}

void UtilsZipUnPackZLIB::addIntercept(UtilsZipUnPackInterceptI *v)
{
    this->mIntercepts.push_back(v);
}

void UtilsZipUnPackZLIB::cleanIntercept()
{
    this->mIntercepts.clear();
}

void UtilsZipUnPackZLIB::streamIntercept(const Jchar *in, const Jchar *out)
{
    Jint  readLen      = 0;
    Jbool backlistMark = false;

    unzFile unzFile = nullptr;

    unz_file_info         unzFileInfo;
    std::string           outPath;
    std::filesystem::path outDirPath;
    std::ofstream         outFile;

    unzFile = unzOpen(in);
    if (unzFile == nullptr)
        return;

    unzGoToFirstFile(unzFile);

    do
    {
        if (unzGetCurrentFileInfo(
                unzFile,
                &unzFileInfo,
                this->mFileName,
                sizeof(this->mFileName),
                nullptr,
                0,
                nullptr,
                0
        ) != UNZ_OK)
            continue;

        backlistMark = false;
        for (auto &name : this->mFileNameBacklist)
        {
            if (strstr(this->mFileName, name.c_str()) == nullptr)
                continue;

            backlistMark = true;
            break;
        }

        if (!backlistMark)
        {
            if (unzOpenCurrentFile(unzFile) != UNZ_OK)
                continue;

            outPath.clear();
            outDirPath.clear();

            outPath.append(out)
                    .append(PATH_SYMBOL)
                    .append(this->mFileName);

            outDirPath.append(outPath).remove_filename();
            if (!std::filesystem::exists(outDirPath))
                std::filesystem::create_directories(outDirPath);

            for (auto &v : outPath)
            {
                if (v == '\\')
                    v = '/';
            }

            this->mZipAllFilePath.emplace_back(this->mFileName);
            this->mRootAllFilePath.push_back(outPath);

            outFile.open(
                    outPath,
                    (static_cast<Juint>(std::ios::out)
                     | static_cast<Juint>(std::ios::binary)
                     | static_cast<Juint>(std::ios::trunc))
            );
            this->notifyStart(this->mFileName, outPath.c_str());

            do
            {
                readLen = unzReadCurrentFile(unzFile, this->mReadCache, sizeof(this->mReadCache));
                if (readLen <= 0)
                    break;

                this->notifyStream(
                        this->mFileName,
                        outPath.c_str(),
                        reinterpret_cast<Jbyte *>(this->mReadCache),
                        readLen
                );

                outFile.write(this->mReadCache, readLen);
            } while (readLen == sizeof(this->mReadCache));

            this->notifyEnd(this->mFileName, outPath.c_str());
            outFile.close();
            unzCloseCurrentFile(unzFile);
        }
    } while (unzGoToNextFile(unzFile) == UNZ_OK);

    unzClose(unzFile);
}

void UtilsZipUnPackZLIB::notifyStart(const Jchar *name, const Jchar *path)
{
    for (const auto &v :this->mIntercepts)
        v->unPackStart(name, path);
}

void UtilsZipUnPackZLIB::notifyEnd(const Jchar *name, const Jchar *path)
{
    for (const auto &v : this->mIntercepts)
        v->unPackEnd(name, path);
}

void UtilsZipUnPackZLIB::notifyStream(const Jchar *name, const Jchar *path, const Jbyte *v, Jint vLen)
{
    for (const auto &intercept : this->mIntercepts)
        intercept->unPackStream(name, path, v, vLen);
}

}

}

#endif //CSIGNAPK_UTILSZIPUNPACKZLIB_HPP
