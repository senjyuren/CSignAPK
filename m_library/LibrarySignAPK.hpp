#pragma once

#ifndef CSIGNAPK_LIBRARYSIGNAPK_HPP
#define CSIGNAPK_LIBRARYSIGNAPK_HPP

#include "LibrarySignAPK.h"

#include "impl/LibraryImplPKCS7Sign.hpp"
#include "impl/LibraryImplX509Sign.hpp"
#include "impl/LibraryImplX509ReqSign.hpp"
#include "impl/LibraryImplSignAPK.hpp"

namespace m
{

inline namespace apk
{

LibrarySignAPKI *LibrarySignAPK::build(LibrarySignAPKExtSignI *v)
{
    return new LibraryImplSignAPK(v);
}

void LibrarySignAPK::destroy(LibrarySignAPKI *v)
{
    delete (v);
}

}

}

#endif //CSIGNAPK_LIBRARYSIGNAPK_HPP
