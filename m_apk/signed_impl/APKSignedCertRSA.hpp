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
    constexpr static Jchar DEFAULT_NAME[] = "/CERT.RSA";

    Builder     *mBuilder;
    std::string &mOutPath;

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
          , mOutFile{}
{
    this->mOutPath.append(DEFAULT_NAME);
    this->mOutFile.open(
            this->mOutPath.c_str(),
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

void APKSignedCertRSA::signHeadStream(const Jchar *, const Jchar *, Jint)
{

}

void APKSignedCertRSA::signContentStream(const Jchar *, const Jchar *v, Jint vLen)
{
    this->mOutFile.write(v, vLen);
}

void APKSignedCertRSA::signStreamEnd()
{
    this->mOutFile.flush();
}

const std::string &APKSignedCertRSA::getPath()
{
    return this->mOutPath;
}

}

}

#endif //CSIGNAPK_APKSIGNEDCERTRSA_HPP
