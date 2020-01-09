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

    virtual LibrarySignAPKI &apkCity(const char *v) = 0;

    virtual LibrarySignAPKI &apkCommon(const char *v) = 0;

    virtual LibrarySignAPKI &apkEmail(const char *v) = 0;

    virtual LibrarySignAPKI &apkCountry(const char *v) = 0;

    virtual LibrarySignAPKI &apkOrganization(const char *v) = 0;

    virtual LibrarySignAPKI &apkOrganizationUnit(const char *v) = 0;

    virtual LibrarySignAPKI &apkProvince(const char *v) = 0;

    virtual LibrarySignAPKI &apkPrivateKey(const char *v) = 0;

    virtual LibrarySignAPKI &apkPublicKey(const char *v) = 0;

    virtual LibrarySignAPKI &apkInputAPKPath(const char *v) = 0;

    virtual LibrarySignAPKI &apkOutputPath(const char *v) = 0;

    virtual void apkSign() = 0;
};

LibrarySignAPKI::~LibrarySignAPKI() = default;

}

}

#endif //CSIGNAPK_LIBRARYSIGNAPKI_H
