#pragma once

#ifndef CSIGNAPK_KERNEL_HPP
#define CSIGNAPK_KERNEL_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <vector>
#include <string>
#include <list>

#include <jemalloc/jemalloc.h>

using Jbyte  = unsigned char;
using Juint  = unsigned int;
using Julong = unsigned long;
using Jchar  = char;
using Jint   = int;
using Jlong  = long;
using Jllong = long long;
using Jbool  = bool;
using Jsize  = size_t;

void *operator new(Jsize vLen)
{
    auto &&obj = je_malloc(vLen);
    if (obj != nullptr)
        memset(obj, 0, vLen);
    return obj;
}

void operator delete(void *v) noexcept
{
    je_free(v);
}

void *operator new[](Jsize vLen)
{
    auto &&obj = je_malloc(vLen);
    if (obj != nullptr)
        memset(obj, 0, vLen);
    return obj;
}

void operator delete[](void *v) noexcept
{
    je_free(v);
}

#include "m_utils/UtilsHEX.hpp"
#include "m_utils/UtilsString.hpp"
#include "m_utils/UtilsBase64.hpp"

#include "m_utils/UtilsZip.hpp"
#include "m_utils/UtilsSHA.hpp"
#include "m_utils/UtilsX509.hpp"
#include "m_utils/UtilsPKCS.hpp"

#include "m_apk/APKObjectsQueue.hpp"
#include "m_library/LibrarySignAPK.hpp"

#endif //CSIGNAPK_KERNEL_HPP
