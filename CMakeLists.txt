CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(CSignAPK)

SET(CMAKE_CXX_STANDARD 17)

ADD_DEFINITIONS("/DZLIB_WINAPI")
ADD_DEFINITIONS("/D_WIN32")

INCLUDE_DIRECTORIES(D:/work/Libary/openssl_install_static/include)
INCLUDE_DIRECTORIES(D:/work/Libary/zlib-1.2.11/build_windows/include)
INCLUDE_DIRECTORIES(D:/work/Libary/libdeflate-1.5-windows-i686-bin)

ADD_EXECUTABLE(CSignAPK main.cpp)
TARGET_LINK_LIBRARIES(CSignAPK
        D:/work/Libary/openssl_install_static/lib/libssl.lib
        D:/work/Libary/openssl_install_static/lib/libcrypto.lib
        D:/work/Libary/zlib-1.2.11/build_windows/lib/zlibstat.lib
        D:/work/Libary/libdeflate-1.5-windows-i686-bin/libdeflatestatic.lib)