#pragma once

#ifndef CSIGNAPK_UTILSZIPPACKZLIB_HPP
#define CSIGNAPK_UTILSZIPPACKZLIB_HPP

#include <zip.h>

namespace m
{

inline namespace utils
{

class UtilsZipPackZLIB
        : public UtilsZipPackI
{
private:
    constexpr static Jint  READ_CACHE_SIZE         = 2048;
    constexpr static Jchar DEFAULT_OUT_FILE_NAME[] = "/signed.apk";

    std::vector<std::string> &mZipAllFilePath;
    std::vector<std::string> &mRootAllFilePath;

    std::vector<std::string> mExtraFiles;
    Jchar                    mReadCache[READ_CACHE_SIZE];

public:
    explicit UtilsZipPackZLIB(std::vector<std::string> &zipAll, std::vector<std::string> &rootAll);

    void addFile(const Jchar *v) override;

    void pack(const Jchar *in, const Jchar *out) override;
};

UtilsZipPackZLIB::UtilsZipPackZLIB(std::vector<std::string> &zipAll, std::vector<std::string> &rootAll)
        : mZipAllFilePath{zipAll}
          , mRootAllFilePath{rootAll}
          , mExtraFiles{}
          , mReadCache{}
{
}

void UtilsZipPackZLIB::addFile(const Jchar *v)
{
    this->mExtraFiles.emplace_back(v);
}

void UtilsZipPackZLIB::pack(const Jchar *in, const Jchar *out)
{
    Jint i = 0;

    std::string   inPath(in);
    std::string   ouPath(out);
    std::ifstream inFile;

    zipFile      zipFile = nullptr;
    zip_fileinfo zipFileInfo;

    ouPath.append(DEFAULT_OUT_FILE_NAME);
    zipFile = zipOpen(ouPath.c_str(), APPEND_STATUS_CREATE);
    if (zipFile == nullptr)
        return;

    for (auto &v : this->mExtraFiles)
    {
        this->mZipAllFilePath.push_back(v);
        this->mRootAllFilePath.push_back(v);
    }

    memset(&zipFileInfo, 0, sizeof(zipFileInfo));
    for (i = 0; i < static_cast<Jint>(this->mRootAllFilePath.size()); ++i)
    {
        if (zipOpenNewFileInZip(
                zipFile,
                this->mZipAllFilePath[i].c_str(),
                &zipFileInfo,
                nullptr,
                0,
                nullptr,
                0,
                nullptr,
                Z_DEFLATED,
                Z_DEFAULT_COMPRESSION
        ) != ZIP_OK)
            continue;

        inFile.open(this->mRootAllFilePath[i], (std::ios::in | std::ios::binary));

        do
        {
            inFile.read(this->mReadCache, sizeof(this->mReadCache));
            zipWriteInFileInZip(zipFile, this->mReadCache, inFile.gcount());
        } while (inFile.gcount() == sizeof(this->mReadCache));

        inFile.close();
        zipCloseFileInZip(zipFile);
    }

    zipClose(zipFile, nullptr);
}

}

}

#endif //CSIGNAPK_UTILSZIPPACKZLIB_HPP
