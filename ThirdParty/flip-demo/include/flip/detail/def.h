/*****************************************************************************

      def.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <stdexcept>

#include <cassert>

#if defined _MSC_VER
   #include "intrin.h"  // for byteswap
#endif

#if defined (__APPLE__)
   #include "TargetConditionals.h"
#endif




/*\\\ LANG DIALECT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// http://sourceforge.net/p/predef/wiki/Standards/

#define flip_LANG_DIALECT_CPP11 (1)
#define flip_LANG_DIALECT_CPP14 (2)

#if defined (__cplusplus)
   #if (__cplusplus <= 201103L)
      #define flip_LANG_DIALECT (flip_LANG_DIALECT_CPP11)

   #elif (__cplusplus <= 201402L)
      #define flip_LANG_DIALECT (flip_LANG_DIALECT_CPP14)

   #else
      #define flip_LANG_DIALECT (flip_LANG_DIALECT_CPP14)   // for now
   #endif

#else
   #error Unsupported language
#endif



/*\\\ WORD SIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// http://sourceforge.net/p/predef/wiki/Architectures/

#if defined (_M_IA64) || defined (_WIN64) || defined (__64BIT__) || defined (__powerpc64__) || defined (__ppc64__) || defined (__x86_64__) || defined (__aarch64__)
   #define flip_WORD_SIZE 64
   #define flip_WORD_SIZE_BYTE 8

#elif defined (_M_IX86) || defined (_WIN32)  || defined (__i386__) || defined (__arm__)
   #define flip_WORD_SIZE 32
   #define flip_WORD_SIZE_BYTE 4

#else
   #error Unsupported architecture

#endif



/*\\\ COMPILER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// http://www.boost.org boost/config/select_compiler_config.hpp

#define flip_COMPILER_CLANG (1)
#define flip_COMPILER_GNU   (2)
#define flip_COMPILER_MSVC  (3)
#define flip_COMPILER_INTEL (4)

#if defined (__INTEL_COMPILER) || defined (__ICL) || defined (__ICC) || defined (__ECC)
   #define flip_COMPILER (flip_COMPILER_INTEL)

#elif defined (__clang__)
   #define flip_COMPILER (flip_COMPILER_CLANG)

#elif defined __GNUC__
   #define flip_COMPILER (flip_COMPILER_GNU)

#elif defined _MSC_VER
   #define flip_COMPILER (flip_COMPILER_MSVC)

#else
   #error Unsupported compiler

#endif



/*\\\ PLATFORM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// http://www.boost.org boost/config/select_platform_config.hpp

#define flip_PLATFORM_LINUX  (1)
#define flip_PLATFORM_BSD    (2)
#define flip_PLATFORM_CYGWIN (3)
#define flip_PLATFORM_WIN32  (4)
#define flip_PLATFORM_MACOS  (5)
#define flip_PLATFORM_IOS    (6)


#if (defined (linux) || defined (__linux) || defined (__linux__) || defined (__GNU__) || defined (__GLIBC__)) && ! defined (_CRAYC)
   #define flip_PLATFORM (flip_PLATFORM_LINUX)

#elif defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) || defined (__DragonFly__)
   #define flip_PLATFORM (flip_PLATFORM_BSD)

#elif defined (__CYGWIN__)
   #define flip_PLATFORM (flip_PLATFORM_CYGWIN)

#elif defined (_WIN32) || defined (__WIN32__) || defined (WIN32)
   #define flip_PLATFORM (flip_PLATFORM_WIN32)

#elif defined (macintosh) || defined (__APPLE__) || defined (__APPLE_CC__)
   #if defined (TARGET_OS_IOS) && TARGET_OS_IOS
      #define flip_PLATFORM (flip_PLATFORM_IOS)
   #else
      #define flip_PLATFORM (flip_PLATFORM_MACOS)
   #endif

#else
   #error Unsupported platform

#endif



/*\\\ ENDIAN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define flip_ENDIAN_BIG    (1)
#define flip_ENDIAN_LITTLE (2)

#if (flip_PLATFORM == flip_PLATFORM_WIN32) || defined (__i386__) || defined (__x86_64__) || defined (__INTEL__) || (defined (__APPLE__) && defined (__aarch64__)) || (defined (__APPLE__) && defined (__arm__))
   #define flip_ENDIAN (flip_ENDIAN_LITTLE)
#else
   #error Unsupported architecture
#endif


#if (flip_ENDIAN == flip_ENDIAN_LITTLE)
   #if (flip_COMPILER == flip_COMPILER_CLANG) || (flip_COMPILER == flip_COMPILER_GNU)
      #define flip_SWAP_BE16(x) \
         x = __builtin_bswap16 (x)

      #define flip_SWAP_BE32(x) \
         x = __builtin_bswap32 (x)

      #define flip_SWAP_BE64(x) \
         x = __builtin_bswap64 (x)

   #elif (flip_COMPILER == flip_COMPILER_MSVC)
      #define flip_SWAP_BE16(x) \
         x = _byteswap_ushort (x)

      #define flip_SWAP_BE32(x) \
         x = _byteswap_ulong (x)

      #define flip_SWAP_BE64(x) \
         x = _byteswap_uint64 (x)

   #else
      #error Unsupported compiler
   #endif

#else
   #define flip_SWAP_BE16(x) \
      ((void)(0))

   #define flip_SWAP_BE32(x) \
      ((void)(0))

   #define flip_SWAP_BE64(x) \
      ((void)(0))

#endif



/*\\\ EXCEPTION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// flip_THROW represents a non-permanent runtime error

#if (flip_COMPILER == flip_COMPILER_CLANG) || (flip_COMPILER == flip_COMPILER_GNU)
   #define flip_THROW \
      throw std::runtime_error (__PRETTY_FUNCTION__)

#elif (flip_COMPILER == flip_COMPILER_MSVC)
   #define flip_THROW \
      throw std::runtime_error (__FUNCDNAME__)

#else
   #error Unsupported Compiler.

#endif



// flip_FATAL represents a permanent error, either a developer error or an
// internal non-recoverable error

#if (flip_COMPILER == flip_COMPILER_CLANG)
   #if (flip_FATAL_MODE == flip_FATAL_MODE_THROW)
      #define flip_FATAL \
         throw std::runtime_error (__PRETTY_FUNCTION__)
   #elif (flip_FATAL_MODE == flip_FATAL_MODE_ABORT)
      #define flip_FATAL \
         _Pragma ("clang diagnostic push") \
         _Pragma ("clang diagnostic ignored \"-Wunreachable-code\"") \
         {assert (false); throw;} \
         _Pragma ("clang diagnostic pop")
   #elif (flip_FATAL_MODE == flip_FATAL_MODE_USER)
      #if ! defined (flip_FATAL)
         #error flip_FATAL_MODE_USER requires a user provided definition of flip_FATAL
      #endif
   #else
      #error Unsupported Configuration
   #endif

#elif (flip_COMPILER == flip_COMPILER_GNU)
   #if (flip_FATAL_MODE == flip_FATAL_MODE_THROW)
      #define flip_FATAL \
         throw std::runtime_error (__PRETTY_FUNCTION__)
   #elif (flip_FATAL_MODE == flip_FATAL_MODE_ABORT)
      #define flip_FATAL \
         {assert (false); throw;}
   #elif (flip_FATAL_MODE == flip_FATAL_MODE_USER)
      #if ! defined (flip_FATAL)
         #error flip_FATAL_MODE_USER requires a user provided definition of flip_FATAL
      #endif
   #else
      #error Unsupported Configuration
   #endif

#elif (flip_COMPILER == flip_COMPILER_MSVC)
   #if (flip_FATAL_MODE == flip_FATAL_MODE_THROW)
      #define flip_FATAL \
         throw std::runtime_error (__FUNCDNAME__)
   #elif (flip_FATAL_MODE == flip_FATAL_MODE_ABORT)
      #define flip_FATAL \
         {assert (false); throw;}
   #elif (flip_FATAL_MODE == flip_FATAL_MODE_USER)
      #if ! defined (flip_FATAL)
         #error flip_FATAL_MODE_USER requires a user provided definition of flip_FATAL
      #endif
   #else
      #error Unsupported Configuration
   #endif

#else
   #error Unsupported Compiler.

#endif



// flip_NOTHING_CHECK_*

#if (flip_NOTHING == flip_NOTHING_ZERO)
   #define flip_NOTHING_CHECK_VALUE ((void)(0))
   #define flip_NOTHING_CHECK_BEFORE ((void)(0))

#elif (flip_NOTHING == flip_NOTHING_FATAL)
   #define flip_NOTHING_CHECK_VALUE if (removed ()) flip_FATAL
   #define flip_NOTHING_CHECK_BEFORE if (added ()) flip_FATAL

#endif



/*\\\ UTILITY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define flip_CC4(c0, c1, c2, c3) (((c0) << 24) | ((c1) << 16) | ((c2) << 8) | (c3))
#define flip_STATIC_LOCAL(type, name, arguments) static type & name = * new type arguments

#define flip_UNUSED(expr) ((void) (expr))

#if (flip_COMPILER == flip_COMPILER_CLANG) || (flip_COMPILER == flip_COMPILER_GNU)
   #define flip_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#else
   #define flip_WARN_UNUSED_RESULT

#endif



#if (flip_COMPILER == flip_COMPILER_CLANG) || (flip_COMPILER == flip_COMPILER_GNU)
   #define flip_NORETURN [[noreturn]]

#elif (flip_COMPILER == flip_COMPILER_MSVC)
   #define flip_NORETURN __declspec(noreturn)

#else
   #error Unsupported Compiler.

#endif



/*\\\ WARNINGS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if defined (__clang__)
   #define flip_DISABLE_WARNINGS_CAST \
      _Pragma ("clang diagnostic push") \
      _Pragma ("clang diagnostic ignored \"-Wundefined-reinterpret-cast\"")

   #define flip_DISABLE_WARNINGS_BINARY_LITERALS \
      _Pragma ("clang diagnostic push") \
      _Pragma ("clang diagnostic ignored \"-Wc++1y-extensions\"")

   #define flip_DISABLE_WARNINGS_GLOBAL_DESTRUCTORS \
      _Pragma ("clang diagnostic push") \
      _Pragma ("clang diagnostic ignored \"-Wglobal-constructors\"") \
      _Pragma ("clang diagnostic ignored \"-Wexit-time-destructors\"")

   #define flip_DISABLE_WARNINGS_FOUR_CHAR_CONSTANTS \
      _Pragma ("clang diagnostic push") \
      _Pragma ("clang diagnostic ignored \"-Wfour-char-constants\"")

   #define flip_DISABLE_WARNINGS_CONTROL_PATHS \
      _Pragma ("clang diagnostic push")

   #define flip_DISABLE_WARNINGS_DOMINANCE \
      _Pragma ("clang diagnostic push")

   #define flip_DISABLE_WARNINGS_TRUSTED_MACROS \
      _Pragma ("clang diagnostic push") \
      _Pragma ("clang diagnostic ignored \"-Wold-style-cast\"") \
      _Pragma ("clang diagnostic ignored \"-Wunreachable-code\"")

   #define flip_DISABLE_WARNINGS_SYSTEM_HEADERS \
      _Pragma ("clang diagnostic push")

   #define flip_DISABLE_WARNINGS_THROW_FROM_DTOR \
      _Pragma ("clang diagnostic push")

   #define flip_RESTORE_WARNINGS \
      _Pragma ("clang diagnostic pop")

   #define flip_DISABLE_WARNINGS_TEST \
      _Pragma ("clang diagnostic ignored \"-Wfour-char-constants\"")



#elif defined (__GNUC__)
   #define flip_DISABLE_WARNINGS_CAST \
      _Pragma ("GCC diagnostic push") \
      _Pragma ("GCC diagnostic ignored \"-Wstrict-aliasing\"")

   #define flip_DISABLE_WARNINGS_BINARY_LITERALS \
      _Pragma ("GCC diagnostic push") \

   #define flip_DISABLE_WARNINGS_GLOBAL_DESTRUCTORS \
      _Pragma ("GCC diagnostic push") \

   #define flip_DISABLE_WARNINGS_FOUR_CHAR_CONSTANTS \
      _Pragma ("GCC diagnostic push") \

   #define flip_DISABLE_WARNINGS_CONTROL_PATHS \
      _Pragma ("GCC diagnostic push") \
      _Pragma ("GCC diagnostic ignored \"-Wreturn-type\"")

   #define flip_DISABLE_WARNINGS_DOMINANCE \
      _Pragma ("GCC diagnostic push")

   #define flip_DISABLE_WARNINGS_TRUSTED_MACROS \
      _Pragma ("GCC diagnostic push") \

   #define flip_DISABLE_WARNINGS_SYSTEM_HEADERS \
      _Pragma ("GCC diagnostic push")

   #define flip_DISABLE_WARNINGS_THROW_FROM_DTOR \
      _Pragma ("GCC diagnostic push")

   #define flip_RESTORE_WARNINGS \
      _Pragma ("GCC diagnostic pop")

   #define flip_DISABLE_WARNINGS_TEST



#elif defined (_MSC_VER)
   #define flip_DISABLE_WARNINGS_CAST \
      __pragma (warning (push))

   #define flip_DISABLE_WARNINGS_BINARY_LITERALS \
      __pragma (warning (push))

   #define flip_DISABLE_WARNINGS_GLOBAL_DESTRUCTORS \
      __pragma (warning (push))

   #define flip_DISABLE_WARNINGS_FOUR_CHAR_CONSTANTS \
      __pragma (warning (push))

   #define flip_DISABLE_WARNINGS_CONTROL_PATHS \
      __pragma (warning (push)) \
      __pragma (warning (disable: 4715))

   #define flip_DISABLE_WARNINGS_DOMINANCE \
      __pragma (warning (push)) \
      __pragma (warning (disable: 4250))

   #define flip_DISABLE_WARNINGS_TRUSTED_MACROS \
      __pragma (warning (push)) \
      __pragma (warning (disable: 4548))

   #define flip_DISABLE_WARNINGS_SYSTEM_HEADERS \
      __pragma (warning (push)) \
      __pragma (warning (disable: 4668)) \
      __pragma (warning (disable: 4625)) \
      __pragma (warning (disable: 4626))

   #define flip_DISABLE_WARNINGS_THROW_FROM_DTOR \
      __pragma (warning (push)) \
      __pragma (warning (disable: 4297))

   #define flip_RESTORE_WARNINGS \
      __pragma (warning (pop))

   #define flip_DISABLE_WARNINGS_TEST \
      __pragma (warning (disable: 4265)) \
      __pragma (warning (disable: 4242)) \
      __pragma (warning (disable: 4255)) \
      __pragma (warning (disable: 4625)) \
      __pragma (warning (disable: 4626))

#else
   #define flip_DISABLE_WARNINGS_CAST
   #define flip_DISABLE_WARNINGS_BINARY_LITERALS
   #define flip_DISABLE_WARNINGS_GLOBAL_DESTRUCTORS
   #define flip_DISABLE_WARNINGS_FOUR_CHAR_CONSTANTS
   #define flip_DISABLE_WARNINGS_CONTROL_PATHS
   #define flip_DISABLE_WARNINGS_DOMINANCE
   #define flip_DISABLE_WARNINGS_TRUSTED_MACROS
   #define flip_DISABLE_WARNINGS_SYSTEM_HEADERS
   #define flip_DISABLE_WARNINGS_THROW_FROM_DTOR

   #define flip_RESTORE_WARNINGS

   #define flip_DISABLE_WARNINGS_TEST

#endif

#if defined (__clang__)

   #if __has_warning ("-Wtautological-undefined-compare")
      #define flip_ASSERT_NON_NULLPTR(x)                                            \
         _Pragma ("clang diagnostic push")                                          \
         _Pragma ("clang diagnostic ignored \"-Wtautological-undefined-compare\"")  \
         assert ((x) != nullptr);                                                   \
         _Pragma ("clang diagnostic pop")
   #else
      #define flip_ASSERT_NON_NULLPTR(x)                                            \
         assert ((x) != nullptr);
   #endif


#else
   #define flip_ASSERT_NON_NULLPTR(x)  assert ((x) != nullptr)

#endif



namespace flip
{



}  // namespace flip



//#include  "flip/def.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

