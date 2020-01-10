#pragma once

#ifndef CSIGNAPK_LIBRARYSIGNAPK_H
#define CSIGNAPK_LIBRARYSIGNAPK_H

#include "LibrarySignAPKDefine.h"
#include "LibrarySignAPKExtSignI.h"
#include "LibrarySignAPKI.h"

namespace m
{

inline namespace apk
{

class OBJECTS_CLS_EXPORT LibrarySignAPK
{
public:
    static LibrarySignAPKI *build(LibrarySignAPKExtSignI *v);

    static void destroy(LibrarySignAPKI *v);
};

}

}

#endif //CSIGNAPK_LIBRARYSIGNAPK_H
