#pragma once

#include "Platform.hpp"

// Define DLL import/export macros (only Windows, and only dynamic
// configuration)
#if PLATFORM_IS(PLATFORM_WINDOWS) && !defined(EDOREN_STATIC)
    #define EDOREN_SYMBOL_EXPORTS __declspec(dllexport)
    #define EDOREN_SYMBOL_IMPORTS __declspec(dllimport)
    // Disable annoying MSVC++ warning
    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
        #pragma warning(disable : 4661)
    #endif  // _MSC_VER
#elif __GNUC__ >= 4
    // GCC 4 has special keywords for showing/hidding symbols,
    // the same keyword is used for both importing and exporting
    #define EDOREN_SYMBOL_EXPORTS __attribute__((__visibility__("default")))
    #define EDOREN_SYMBOL_IMPORTS __attribute__((__visibility__("default")))
#else
    // GCC < 4 has no mechanism to explicitely hide symbols, everything's
    // exported
    #define EDOREN_SYMBOL_EXPORTS
    #define EDOREN_SYMBOL_IMPORTS
#endif

#ifdef EDOREN_EXPORTS
    #define EDOREN_API EDOREN_SYMBOL_EXPORTS
#else
    #define EDOREN_API EDOREN_SYMBOL_IMPORTS
#endif

#if defined(DEBUG) || !defined(NDEBUG) || defined(_DEBUG)
    #define EDOREN_DEBUG
#endif

// Disable warning for not using CRT secure functions
#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS
    #pragma warning(disable : 4996)
#endif

/**
 * @namespace edoren
 * @brief Namespace that contains all the tools
 */
