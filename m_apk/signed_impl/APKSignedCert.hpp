#pragma once

#ifndef CSIGNAPK_APKSIGNEDCERT_HPP
#define CSIGNAPK_APKSIGNEDCERT_HPP

#include "APKSignedStreamI.hpp"
#include "APKSignedCertBlockI.hpp"

namespace m
{

inline namespace apk
{

class APKSignedCert
        : public APKSignedStreamI
{
public:
    class Builder;

private:
    constexpr static Jint WRITE_SIZE         = 2048;
    constexpr static Jint LIMIT_NAME_SIZE    = 64;
    constexpr static Jint LIMIT_NAME_EX_SIZE = 64;

    constexpr static Jchar DEFAULT_DIR[]  = "/META-INF";
    constexpr static Jchar DEFAULT_NAME[] = "/CERT.SF";

    constexpr static Jchar DEFAULT_TEMPLATE_HEAD[]           =
                                   "Signature-Version: 1.0\r\n"
                                   "Created-By: 1.0 (Android)\r\n"
                                   "SHA-256-Digest-Manifest: %s\r\n\r\n";
    constexpr static Jchar DEFAULT_TEMPLATE_CONTENT[]        =
                                   "Name: %s\r\n"
                                   "SHA-256-Digest: %s\r\n\r\n";
    constexpr static Jchar DEFAULT_TEMPLATE_CONTENT_TO_BIG[] =
                                   "Name: %s\r\n"
                                   " %s\r\n"
                                   "SHA-256-Digest: %s\r\n\r\n";

    Builder                          *mBuilder;
    std::list<APKSignedCertBlockI *> &mBlocks;
    std::string                      &mOutPath;

    std::string mRootDir;
    std::string mPathFile;
    std::string mZipPathFile;

    std::fstream mOutFile;
    std::string  mFileHead;
    std::string  mFileContent;
    Jchar        mWriteCache[WRITE_SIZE];

    Jchar mLimitName[LIMIT_NAME_SIZE + 1];
    Jchar mLimitNameEx[LIMIT_NAME_EX_SIZE + 1];

public:
    class Builder
    {
    private:
        std::list<APKSignedCertBlockI *> mBlocks;
        std::string                      mOutPath;

    public:
        friend APKSignedCert;

        Builder();

        ~Builder();

        Builder &setOutPath(const Jchar *v);

        Builder &addIntercept(APKSignedCertBlockI *v);

        std::shared_ptr<APKSignedCert> build();
    };

    explicit APKSignedCert(Builder *builder);

    ~APKSignedCert() override;

    void signHeadStream(const Jchar *name, const Jchar *v, Jint vLen) override;

    void signContentStream(const Jchar *name, const Jchar *v, Jint vLen) override;

    void signStreamEnd() override;

    void notifyContentStart(const Jchar *name);

    void notifyContentUpdate(const Jchar *name, const Jchar *v, Jint vLen);

    void notifyContentEnd(const Jchar *name);

    const std::string &getPath();
};

APKSignedCert::Builder::Builder()
        : mBlocks{}
          , mOutPath{}
{
}

APKSignedCert::Builder::~Builder()
{
    this->mBlocks.clear();
}

APKSignedCert::Builder &APKSignedCert::Builder::setOutPath(const Jchar *v)
{
    this->mOutPath.append(v);
    return (*this);
}

APKSignedCert::Builder &APKSignedCert::Builder::addIntercept(APKSignedCertBlockI *v)
{
    this->mBlocks.push_back(v);
    return (*this);
}

std::shared_ptr<APKSignedCert> APKSignedCert::Builder::build()
{
    return std::make_shared<APKSignedCert>(this);
}

APKSignedCert::APKSignedCert(Builder *builder)
        : mBuilder{builder}
          , mBlocks{builder->mBlocks}
          , mOutPath{builder->mOutPath}
          , mRootDir{}
          , mPathFile{}
          , mZipPathFile{}
          , mOutFile{}
          , mFileHead{}
          , mFileContent{}
          , mWriteCache{}
          , mLimitName{}
          , mLimitNameEx{}
{
    this->mRootDir.append(this->mOutPath)
            .append(DEFAULT_DIR);

    if (!std::filesystem::exists(this->mRootDir))
        std::filesystem::create_directory(this->mRootDir);

    this->mPathFile = this->mRootDir;
    this->mPathFile.append(DEFAULT_NAME);

    this->mOutFile.open(
            this->mPathFile.c_str(),
            (static_cast<Juint>(std::ios::in)
             | static_cast<Juint>(std::ios::out)
             | static_cast<Juint>(std::ios::trunc)
             | static_cast<Juint>(std::ios::binary))
    );
}

APKSignedCert::~APKSignedCert()
{
    this->mOutFile.close();
    std::filesystem::remove(this->mPathFile);
    delete (this->mBuilder);
}

void APKSignedCert::signHeadStream(const Jchar *, const Jchar *v, Jint)
{
    Jint ret = 0;

    ret = snprintf(this->mWriteCache, sizeof(this->mWriteCache), DEFAULT_TEMPLATE_HEAD, v);
    this->mFileHead.append(this->mWriteCache, ret);
}

void APKSignedCert::signContentStream(const Jchar *name, const Jchar *v, Jint)
{
    Jint ret = 0;

    auto &&nameLen = strlen(name);
    if (nameLen > LIMIT_NAME_SIZE)
    {
        memset(this->mLimitName, 0, sizeof(this->mLimitName));
        memset(this->mLimitNameEx, 0, sizeof(this->mLimitNameEx));
        memcpy(this->mLimitName, &name[0], LIMIT_NAME_SIZE);
        memcpy(this->mLimitNameEx, &name[LIMIT_NAME_SIZE], (nameLen - LIMIT_NAME_SIZE));

        ret = snprintf(
                this->mWriteCache,
                sizeof(this->mWriteCache),
                DEFAULT_TEMPLATE_CONTENT_TO_BIG,
                this->mLimitName,
                this->mLimitNameEx,
                v
        );
    } else
    {
        ret = snprintf(
                this->mWriteCache,
                sizeof(this->mWriteCache),
                DEFAULT_TEMPLATE_CONTENT,
                name,
                v
        );
    }

    this->mWriteCache[ret + 1] = 0x00;
    this->mFileContent.append(this->mWriteCache, ret);
}

void APKSignedCert::signStreamEnd()
{
    this->mOutFile.write(this->mFileHead.data(), this->mFileHead.length());
    this->mOutFile.write(this->mFileContent.data(), this->mFileContent.size());
    this->mOutFile.flush();
    this->mOutFile.clear();
    this->mOutFile.seekg(std::ios::beg);
    this->notifyContentStart(this->mPathFile.c_str());

    do
    {
        this->mOutFile.read(this->mWriteCache, sizeof(this->mWriteCache));
        this->notifyContentUpdate(this->mPathFile.c_str(), this->mWriteCache, this->mOutFile.gcount());
    } while (this->mOutFile.gcount() == sizeof(this->mWriteCache));

    this->notifyContentEnd(this->mPathFile.c_str());
}

void APKSignedCert::notifyContentStart(const Jchar *name)
{
    for (auto &block : this->mBlocks)
        block->certContentStart(name);
}

void APKSignedCert::notifyContentUpdate(const Jchar *name, const Jchar *v, Jint vLen)
{
    for (auto &block : this->mBlocks)
        block->certContentUpdate(name, v, vLen);
}

void APKSignedCert::notifyContentEnd(const Jchar *name)
{
    for (auto &block : this->mBlocks)
        block->certContentEnd(name);
}

const std::string &APKSignedCert::getPath()
{
    auto &&point = this->mPathFile.find(DEFAULT_DIR) + 1;
    return (this->mZipPathFile = this->mPathFile.substr(point));
}

}

}

#endif //CSIGNAPK_APKSIGNEDCERT_HPP
