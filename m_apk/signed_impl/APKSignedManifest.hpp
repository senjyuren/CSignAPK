#pragma once

#ifndef CSIGNAPK_APKSIGNEDMANIFEST_HPP
#define CSIGNAPK_APKSIGNEDMANIFEST_HPP

#include "APKSignedStreamI.hpp"
#include "APKSignedManifestBlockI.hpp"

namespace m
{

inline namespace apk
{

class APKSignedManifest
        : public APKSignedStreamI
{
public:
    class Builder;

private:
    constexpr static Jint WRITE_SIZE         = 2048;
    constexpr static Jint LIMIT_NAME_SIZE    = 64;
    constexpr static Jint LIMIT_NAME_EX_SIZE = 64;

    constexpr static Jchar DEFAULT_DIR[]  = "/META-INF";
    constexpr static Jchar DEFAULT_NAME[] = "/MANIFEST.MF";

    constexpr static Jchar DEFAULT_TEMPLATE_HEAD[]           =
                                   "Manifest-Version: 1.0\r\n"
                                   "Built-By: Generated-by-ADT\r\n"
                                   "Created-By: Android Gradle 3.1.0\r\n\r\n";
    constexpr static Jchar DEFAULT_TEMPLATE_CONTENT[]        =
                                   "Name: %s\r\n"
                                   "SHA-256-Digest: %s\r\n\r\n";
    constexpr static Jchar DEFAULT_TEMPLATE_CONTENT_TO_BIG[] =
                                   "Name: %s\r\n"
                                   " %s\r\n"
                                   "SHA-256-Digest: %s\r\n\r\n";

    Builder                              *mBuilder;
    std::list<APKSignedManifestBlockI *> &mBlocks;
    std::string                          &mOutPath;

    std::string mRootDir;
    std::string mPathFile;
    std::string mZipPathFile;

    std::fstream mOutFile;
    Jchar        mWirteCache[WRITE_SIZE];

    Jchar mLimitName[LIMIT_NAME_SIZE + 1];
    Jchar mLimitNameEx[LIMIT_NAME_EX_SIZE + 1];

public:
    class Builder
    {
    private:
        std::list<APKSignedManifestBlockI *> mBlocks;
        std::string                          mOutPath;

    public:
        friend APKSignedManifest;

        Builder();

        ~Builder();

        Builder &setOutPath(const Jchar *v);

        Builder &addIntercept(APKSignedManifestBlockI *v);

        std::shared_ptr<APKSignedManifest> build();
    };

    explicit APKSignedManifest(Builder *builder);

    ~APKSignedManifest() override;

    void signHeadStream(const Jchar *name, const Jchar *v, Jint vLen) override;

    void signContentStream(const Jchar *name, const Jchar *v, Jint vLen) override;

    void signStreamEnd() override;

    void notifyBlock(const Jchar *name, const Jchar *v, Jint vLen);

    void notifyContentStart(const Jchar *name);

    void notifyContentUpdate(const Jchar *name, const Jchar *v, Jint vLen);

    void notifyContentEnd(const Jchar *name);

    const std::string &getPath();
};

APKSignedManifest::Builder::Builder()
        : mBlocks{}
          , mOutPath{}
{
}

APKSignedManifest::Builder::~Builder()
{
    this->mBlocks.clear();
}

APKSignedManifest::Builder &APKSignedManifest::Builder::setOutPath(const Jchar *v)
{
    this->mOutPath.append(v);
    return (*this);
}

APKSignedManifest::Builder &APKSignedManifest::Builder::addIntercept(APKSignedManifestBlockI *v)
{
    this->mBlocks.push_back(v);
    return (*this);
}

std::shared_ptr<APKSignedManifest> APKSignedManifest::Builder::build()
{
    return std::make_shared<APKSignedManifest>(this);
}

APKSignedManifest::APKSignedManifest(APKSignedManifest::Builder *builder)
        : mBuilder{builder}
          , mBlocks{builder->mBlocks}
          , mOutPath{builder->mOutPath}
          , mRootDir{}
          , mPathFile{}
          , mZipPathFile{}
          , mOutFile{}
          , mWirteCache{}
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

    this->mOutFile.write(DEFAULT_TEMPLATE_HEAD, strlen(DEFAULT_TEMPLATE_HEAD));
}

APKSignedManifest::~APKSignedManifest()
{
    this->mOutFile.close();
    delete (this->mBuilder);
}

void APKSignedManifest::signHeadStream(const Jchar *name, const Jchar *v, Jint vLen)
{

}

void APKSignedManifest::signContentStream(const Jchar *name, const Jchar *v, Jint vLen)
{
    Jint ret = 0;

    auto &&nameLen = strlen(name);
    memset(this->mWirteCache, 0, sizeof(this->mWirteCache));
    if (nameLen > LIMIT_NAME_SIZE)
    {
        memset(this->mLimitName, 0, sizeof(this->mLimitName));
        memset(this->mLimitNameEx, 0, sizeof(this->mLimitNameEx));
        memcpy(this->mLimitName, &name[0], LIMIT_NAME_SIZE);
        memcpy(this->mLimitNameEx, &name[LIMIT_NAME_SIZE], (nameLen - LIMIT_NAME_SIZE));

        ret = snprintf(
                this->mWirteCache,
                sizeof(this->mWirteCache),
                DEFAULT_TEMPLATE_CONTENT_TO_BIG,
                this->mLimitName,
                this->mLimitNameEx,
                v
        );
    } else
    {
        ret = snprintf(
                this->mWirteCache,
                sizeof(this->mWirteCache),
                DEFAULT_TEMPLATE_CONTENT,
                name,
                v
        );
    }

    this->notifyBlock(name, this->mWirteCache, ret);
    this->mOutFile.write(this->mWirteCache, ret);
}

void APKSignedManifest::signStreamEnd()
{
    this->mOutFile.flush();
    this->mOutFile.clear();
    this->mOutFile.seekg(std::ios::beg);
    this->notifyContentStart(this->mPathFile.c_str());

    do
    {
        this->mOutFile.read(this->mWirteCache, sizeof(this->mWirteCache));
        this->notifyContentUpdate(this->mPathFile.c_str(), this->mWirteCache, this->mOutFile.gcount());
    } while (this->mOutFile.gcount() == sizeof(this->mWirteCache));

    this->notifyContentEnd(this->mPathFile.c_str());
}

void APKSignedManifest::notifyBlock(const Jchar *name, const Jchar *v, Jint vLen)
{
    for (auto &block : this->mBlocks)
        block->manifestBlock(name, v, vLen);
}

void APKSignedManifest::notifyContentStart(const Jchar *name)
{
    for (auto &block : this->mBlocks)
        block->manifestContentStart(name);
}

void APKSignedManifest::notifyContentUpdate(const Jchar *name, const Jchar *v, Jint vLen)
{
    for (auto &block : this->mBlocks)
        block->manifestContentUpdate(name, v, vLen);
}

void APKSignedManifest::notifyContentEnd(const Jchar *name)
{
    for (auto &block : this->mBlocks)
        block->manifestContentEnd(name);
}

const std::string &APKSignedManifest::getPath()
{
    auto &&point = this->mPathFile.find(DEFAULT_DIR) + 1;
    return (this->mZipPathFile = this->mPathFile.substr(point));
}

}

}

#endif //CSIGNAPK_APKSIGNEDMANIFEST_HPP
