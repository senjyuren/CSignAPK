#pragma once

#ifndef CSIGNAPK_APKSIGNEDCERTRSA_HPP
#define CSIGNAPK_APKSIGNEDCERTRSA_HPP

namespace m
{

inline namespace apk
{

class APKSignedCertRSA
        : public APKSignedStreamI
{
public:
    class Builder;

private:
    constexpr static Jchar DEFAULT_DIR[]  = "/META-INF";
    constexpr static Jchar DEFAULT_NAME[] = "/CERT.RSA";

    Builder     *mBuilder;
    std::string &mOutPath;

    std::string mRootDir;
    std::string mPathFile;
    std::string mZipPathFile;

    std::fstream mOutFile;

public:
    class Builder
    {
    private:
        std::string mOutPath;

    public:
        friend APKSignedCertRSA;

        Builder();

        Builder &setOutPath(const Jchar *v);

        std::shared_ptr<APKSignedCertRSA> build();
    };

    explicit APKSignedCertRSA(Builder *builder);

    ~APKSignedCertRSA() override;

    void signHeadStream(const Jchar *name, const Jchar *v, Jint vLen) override;

    void signContentStream(const Jchar *name, const Jchar *v, Jint vLen) override;

    void signStreamEnd() override;

    const std::string &getPath();
};

APKSignedCertRSA::Builder::Builder()
        : mOutPath{}
{
}

APKSignedCertRSA::Builder &APKSignedCertRSA::Builder::setOutPath(const Jchar *v)
{
    this->mOutPath.append(v);
    return (*this);
}

std::shared_ptr<APKSignedCertRSA> APKSignedCertRSA::Builder::build()
{
    return std::make_shared<APKSignedCertRSA>(this);
}

APKSignedCertRSA::APKSignedCertRSA(Builder *builder)
        : mBuilder{builder}
          , mOutPath{builder->mOutPath}
          , mRootDir{}
          , mPathFile{}
          , mZipPathFile{}
          , mOutFile{}
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

APKSignedCertRSA::~APKSignedCertRSA()
{
    this->mOutFile.close();
    delete (this->mBuilder);
}

void APKSignedCertRSA::signHeadStream(const Jchar *name, const Jchar *v, Jint vLen)
{

}

void APKSignedCertRSA::signContentStream(const Jchar *name, const Jchar *v, Jint vLen)
{
    this->mOutFile.write(v, vLen);
}

void APKSignedCertRSA::signStreamEnd()
{
    this->mOutFile.flush();
}

const std::string &APKSignedCertRSA::getPath()
{
    auto &&point = this->mPathFile.find(DEFAULT_DIR) + 1;
    return (this->mZipPathFile = this->mPathFile.substr(point));
}

}

}

#endif //CSIGNAPK_APKSIGNEDCERTRSA_HPP
