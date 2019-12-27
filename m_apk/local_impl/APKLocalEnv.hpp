#pragma once

#ifndef CSIGNAPK_APKLOCALENV_HPP
#define CSIGNAPK_APKLOCALENV_HPP

namespace m
{

inline namespace apk
{

class APKLocalEnv
        : public APKLocalEnvI
{
public:
    class Builder;

private:
    constexpr static Jchar DEF_PATH[] = ".";

    Builder *mBuilder;

    APKLocalActionI <APKLocalBeanFileVar, APKLocalBeanFileCon>             &mFile;
    APKLocalActionI <APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &mFileSign;

public:
    class Builder
    {
    private:
        APKLocalActionI <APKLocalBeanFileVar, APKLocalBeanFileCon>             *mFile;
        APKLocalActionI <APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> *mFileSign;

        std::string mPath;

    public:
        friend APKLocalEnv;

        Builder();

        ~Builder();

        Builder &setPath(const Jchar *v);

        std::shared_ptr<APKLocalEnv> build();
    };

    explicit APKLocalEnv(Builder *builder);

    ~APKLocalEnv() override;

    APKLocalActionI <APKLocalBeanFileVar, APKLocalBeanFileCon> &getFile() override;

    APKLocalActionI <APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &getFileSign() override;
};

APKLocalEnv::Builder::Builder()
        : mFile{new APKLocalFile()}
          , mFileSign{new APKLocalFileSigned()}
          , mPath{DEF_PATH}
{
}

APKLocalEnv::Builder::~Builder()
{
    delete (this->mFileSign);
    delete (this->mFile);
}

APKLocalEnv::Builder &APKLocalEnv::Builder::setPath(const Jchar *v)
{
    this->mPath.clear();
    this->mPath.append(v);
    return (*this);
}

std::shared_ptr<APKLocalEnv> APKLocalEnv::Builder::build()
{
    return std::make_shared<APKLocalEnv>(this);
}

APKLocalEnv::APKLocalEnv(Builder *builder)
        : mBuilder{builder}
          , mFile{*builder->mFile}
          , mFileSign{*builder->mFileSign}
{
    this->mFile.start(builder->mPath.c_str());
    this->mFileSign.start(builder->mPath.c_str());
}

APKLocalEnv::~APKLocalEnv()
{
    delete (this->mBuilder);
}

APKLocalActionI <APKLocalBeanFileVar, APKLocalBeanFileCon> &APKLocalEnv::getFile()
{
    return this->mFile;
}

APKLocalActionI <APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon> &APKLocalEnv::getFileSign()
{
    return this->mFileSign;
}

}

}

#endif //CSIGNAPK_APKLOCALENV_HPP
