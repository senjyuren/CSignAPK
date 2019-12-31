#pragma once

#ifndef CSIGNAPK_APKSIGNEDMANIFESTBLOCKI_HPP
#define CSIGNAPK_APKSIGNEDMANIFESTBLOCKI_HPP

namespace m
{

inline namespace apk
{

class APKSignedManifestBlockI
{
public:
    virtual ~APKSignedManifestBlockI();

    virtual void manifestBlock(const Jchar *name, const Jchar *v, Jint vLen) = 0;

    virtual void manifestContentStart(const Jchar *name) = 0;

    virtual void manifestContentUpdate(const Jchar *name, const Jchar *v, Jint vLen) = 0;

    virtual void manifestContentEnd(const Jchar *name) = 0;
};

APKSignedManifestBlockI::~APKSignedManifestBlockI() = default;

}

}

#endif //CSIGNAPK_APKSIGNEDMANIFESTBLOCKI_HPP
