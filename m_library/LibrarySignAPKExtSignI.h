#pragma once

#ifndef CSIGNAPK_LIBRARYSIGNAPKEXTSIGNI_HPP
#define CSIGNAPK_LIBRARYSIGNAPKEXTSIGNI_HPP

namespace m
{

inline namespace apk
{

class LibrarySignAPKExtSignI
{
public:
    virtual ~LibrarySignAPKExtSignI();

    virtual int extSignInit() = 0;

    virtual int extSignUpdate(const unsigned char *v, int vLen) = 0;

    virtual int extSignFinal(unsigned char *v, int vLen) = 0;
};

LibrarySignAPKExtSignI::~LibrarySignAPKExtSignI() = default;

}

}

#endif //CSIGNAPK_LIBRARYSIGNAPKEXTSIGN_HPP
