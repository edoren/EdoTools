#pragma once

// Define DLL import/export macros (only Windows, and only dynamic
// configuration)
#if (defined(_WIN32) || defined(_WIN64)) && !defined(EDOTOOLS_STATIC)
    #define EDOTOOLS_SYMBOL_EXPORTS __declspec(dllexport)
    #define EDOTOOLS_SYMBOL_IMPORTS __declspec(dllimport)
    // Disable annoying MSVC++ warning
    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
        #pragma warning(disable : 4661)
    #endif  // _MSC_VER
#elif __GNUC__ >= 4
    // GCC 4 has special keywords for showing/hidding symbols,
    // the same keyword is used for both importing and exporting
    #define EDOTOOLS_SYMBOL_EXPORTS __attribute__((__visibility__("default")))
    #define EDOTOOLS_SYMBOL_IMPORTS __attribute__((__visibility__("default")))
#else
    // GCC < 4 has no mechanism to explicitely hide symbols, everything's
    // exported
    #define EDOTOOLS_SYMBOL_EXPORTS
    #define EDOTOOLS_SYMBOL_IMPORTS
#endif

#ifdef EDOTOOLS_EXPORTS
    #define EDOTOOLS_API EDOTOOLS_SYMBOL_EXPORTS
#else
    #define EDOTOOLS_API EDOTOOLS_SYMBOL_IMPORTS
#endif

#if defined(DEBUG) || !defined(NDEBUG) || defined(_DEBUG)
    #define EDOTOOLS_DEBUG
#endif

// Disable warning for not using CRT secure functions
#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS
    #pragma warning(disable : 4996)
#endif

#define EDOTOOLS_UNUSED(var) (void)var

// Use (void) to silent unused warnings.
#include <cassert>
#define EDOTOOLS_ASSERT(exp, msg) assert(((void)msg, exp))

#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))
    #include <cstdlib>
    #define EDOTOOLS_THROW(exception) throw exception
    #define EDOTOOLS_TRY try
    #define EDOTOOLS_CATCH(exception) catch (exception)
#else
    #include <cstdlib>
    #define EDOTOOLS_THROW(exception) std::abort()
    #define EDOTOOLS_TRY if (true)
    #define EDOTOOLS_CATCH(exception) if (false)
#endif

/**
 * @namespace edoren
 * @brief Namespace that contains all the tools
 */
