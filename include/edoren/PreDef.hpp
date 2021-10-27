#pragma once

#include <cassert>

// clang-format off

#define EDOREN_UNUSED(var) (void)var

#ifndef EDOREN_DEBUG
    #define EDOREN_NOEXCEPTION
#endif

// Use (void) to silent unused warnings.
#define EDOREN_ASSERT(exp, msg) assert(((void)msg, exp))

#if (defined(__cpp_exceptions) || \
     defined(__EXCEPTIONS) || \
     defined(_CPPUNWIND)) &&  \
    !defined(JSON_NOEXCEPTION)
    #define EDOREN_THROW(exception) throw exception
    #define EDOREN_TRY try
    #define EDOREN_CATCH(exception) catch (exception)
#else
    #define EDOREN_THROW(exception) std::abort()
    #define EDOREN_TRY if (true)
    #define EDOREN_CATCH(exception) if (false)
#endif

// clang-format on
