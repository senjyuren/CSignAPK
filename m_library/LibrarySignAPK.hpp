#pragma once

#ifndef CSIGNAPK_LIBRARYSIGNAPK_HPP
#define CSIGNAPK_LIBRARYSIGNAPK_HPP

#include "LibrarySignAPK.h"
#include "impl/LibraryImplSignAPK.hpp"

namespace m
{

inline namespace apk
{

LibrarySignAPKI *LibrarySignAPK::build()
{
    return new LibraryImplSignAPK();
}

void LibrarySignAPK::destroy(LibrarySignAPKI *v)
{
    delete (v);
}

}

}

#endif //CSIGNAPK_LIBRARYSIGNAPK_HPP
