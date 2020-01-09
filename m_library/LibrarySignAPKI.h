#pragma once

#ifndef CSIGNAPK_LIBRARYSIGNAPKI_H
#define CSIGNAPK_LIBRARYSIGNAPKI_H

namespace m
{

inline namespace apk
{

class LibrarySignAPKI
{
public:
    virtual ~LibrarySignAPKI();

    virtual LibrarySignAPKI &apkCity(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkCommon(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkEmail(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkCountry(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkOrganization(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkOrganizationUnit(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkProvince(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkPrivateKey(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkPublicKey(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkInputAPKPath(const Jchar *v) = 0;

    virtual LibrarySignAPKI &apkOutputPath(const Jchar *v) = 0;

    virtual void apkSign() = 0;
};

LibrarySignAPKI::~LibrarySignAPKI() = default;

}

}

#endif //CSIGNAPK_LIBRARYSIGNAPKI_H
