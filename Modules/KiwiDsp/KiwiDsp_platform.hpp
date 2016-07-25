/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_DSP_PLATFORM_HPP_INCLUDED
#define KIWI_DSP_PLATFORM_HPP_INCLUDED

//==============================================================================
/*  This file is based on the juce_TargetPlatform.h from the JUCE Core module ( http://juce.com ).
 
    This file figures out which platform is being built, and defines some macros
    that the rest of the code can use for OS-specific compilation.

    Macros that will be set here are:

    - One of KIWI_WINDOWS, KIWI_APPLE, KIWI_MAC, KIWI_LINUX, KIWI_IOS, KIWI_ANDROID, etc.
    - Either KIWI_32BIT or KIWI_64BIT, depending on the architecture.
    - Either KIWI_INTEL or KIWI_ARM
    - Either KIWI_GCC or KIWI_CLANG or KIWI_MSVC
*/

//==============================================================================
#if (defined (_WIN32) || defined (_WIN64))
  #define       KIWI_WIN32 1
  #define       KIWI_WINDOWS 1
#elif defined (KIWI_ANDROID)
  #undef        KIWI_ANDROID
  #define       KIWI_ANDROID 1
#elif defined (LINUX) || defined (__linux__)
  #define     KIWI_LINUX 1
#elif defined (__APPLE_CPP__) || defined(__APPLE_CC__)
  #include <CoreFoundation/CoreFoundation.h> // (needed to find out what platform we're using)

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #define     KIWI_IPHONE 1
    #define     KIWI_IOS 1
  #else
    #define     KIWI_MAC 1
  #endif
#elif defined (__FreeBSD__)
  #define       KIWI_BSD 1
#else
  #error "Unknown platform!"
#endif

//==============================================================================
#if KIWI_WINDOWS
  #ifdef _MSC_VER
    #ifdef _WIN64
      #define KIWI_64BIT 1
    #else
      #define KIWI_32BIT 1
    #endif
  #endif

  #ifdef _DEBUG
    #define KIWI_DEBUG 1
  #endif

  #ifdef __MINGW32__
    #define KIWI_MINGW 1
    #ifdef __MINGW64__
      #define KIWI_64BIT 1
    #else
      #define KIWI_32BIT 1
    #endif
  #endif

  #define KIWI_INTEL 1
#endif

//==============================================================================
#if KIWI_MAC || KIWI_IOS

  #define KIWI_APPLE 1

  #if defined (DEBUG) || defined (_DEBUG) || ! (defined (NDEBUG) || defined (_NDEBUG))
    #define KIWI_DEBUG 1
  #endif

  #if ! (defined (DEBUG) || defined (_DEBUG) || defined (NDEBUG) || defined (_NDEBUG))
    #warning "Neither NDEBUG or DEBUG has been defined - you should set one of these to make it clear whether this is a release build,"
  #endif

  #ifdef __LP64__
    #define KIWI_64BIT 1
  #else
    #define KIWI_32BIT 1
  #endif

  #if KIWI_MAC && MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
    #error "Building under OSX 10.7 is not supported!"
  #endif

#endif

//==============================================================================
#if KIWI_LINUX || KIWI_ANDROID

  #ifdef _DEBUG
    #define KIWI_DEBUG 1
  #endif

  #if defined (__LP64__) || defined (_LP64) || defined (__arm64__)
    #define KIWI_64BIT 1
  #else
    #define KIWI_32BIT 1
  #endif

  #if defined (__arm__) || defined (__arm64__)
    #define KIWI_ARM 1
  #elif __MMX__ || __SSE__ || __amd64__
    #define KIWI_INTEL 1
  #endif
#endif

//==============================================================================
// Compiler type macros.

#ifdef __clang__
  #define KIWI_CLANG 1
#elif defined (__GNUC__)
  #define KIWI_GCC 1
#elif defined (_MSC_VER)
  #define KIWI_MSVC 1

  #if _MSC_VER < 1500
    #define KIWI_VC8_OR_EARLIER 1

    #if _MSC_VER < 1400
      #error "Visual Studio 2003 and earlier are no longer supported!"
    #endif
  #endif
#else
  #error unknown compiler
#endif

#endif   // KIWI_DSP_PLATFORM_HPP_INCLUDED
