#pragma once

/*
    This header file defines some useful macros to detect the current
    running system, as well as which system APIs are avaliable on it.
*/

#define PLATFORM_WINDOWS 0x1
#define PLATFORM_LINUX 0x2
#define PLATFORM_MACOS 0x4
#define PLATFORM_ANDROID 0x8
#define PLATFORM_IOS 0x10

#define PLATFORM_TYPE_DESKTOP 0x1
#define PLATFORM_TYPE_MOBILE 0x2

#define PLATFORM_API_WIN32 0x1
#define PLATFORM_API_WIN64 0x2
#define PLATFORM_API_POSIX 0x4

// Find the current platform and platform type
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM PLATFORM_WINDOWS
    #define PLATFORM_TYPE PLATFORM_TYPE_DESKTOP
#elif defined(__linux__)
    #if defined(__ANDROID__) || defined(ANDROID)
        #define PLATFORM PLATFORM_ANDROID
        #define PLATFORM_TYPE PLATFORM_TYPE_MOBILE
    #else
        #define PLATFORM PLATFORM_LINUX
        #define PLATFORM_TYPE PLATFORM_TYPE_DESKTOP
    #endif
#elif defined(__APPLE__)
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
        #define PLATFORM PLATFORM_IOS
        #define PLATFORM_TYPE PLATFORM_TYPE_MOBILE
    #elif TARGET_OS_MAC == 1
        #define PLATFORM PLATFORM_MACOS
        #define PLATFORM_TYPE PLATFORM_TYPE_DESKTOP
    #endif
#else
    #define PLATFORM PLATFORM_UNKNOWN
    #define PLATFORM_TYPE PLATFORM_TYPE_UNKNOWN
#endif

// Detect the platform avaliable APIs
#if defined(_WIN32)
    #if defined(_WIN64)
        #define PLATFORM_APIS (PLATFORM_API_WIN32 & PLATFORM_API_WIN64)
    #else
        #define PLATFORM_APIS PLATFORM_API_WIN32
    #endif
#elif defined(__unix__) || defined(__APPLE__) /* UNIX-style OS. */
    #if defined(_POSIX_VERSION)               /* POSIX compliant */
        #undef PLATFORM_APIS
        #define PLATFORM_APIS PLATFORM_API_POSIX
    #endif
#endif

#define PLATFORM_IS(x) PLATFORM&(x)
#define PLATFORM_TYPE_IS(x) (PLATFORM_TYPE & (x))
#define PLATFORM_HAS_API(x) (PLATFORM_APIS & (x))
