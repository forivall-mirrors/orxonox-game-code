/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      An unknown man from the Ogre development crew
 *   Co-authors:
 *      Reto Grieder (OGRE --> ORXONOX)
 *
 */

/**
 @file  OrxonoxPlatform.h
 @brief Various constants for compiler, architecture and platform. It's a mere
        copy of the file found in the Ogre source code (OgrePlatform.h).
 */

#ifndef _OrxonoxPlatform_H__
#define _OrxonoxPlatform_H__

namespace orxonox {
/* Initial platform/compiler-related stuff to set.
*/
#define ORXONOX_PLATFORM_WIN32 1
#define ORXONOX_PLATFORM_LINUX 2
#define ORXONOX_PLATFORM_APPLE 3

#define ORXONOX_COMPILER_MSVC 1
#define ORXONOX_COMPILER_GNUC 2
#define ORXONOX_COMPILER_BORL 3

#define ORXONOX_ENDIAN_LITTLE 1
#define ORXONOX_ENDIAN_BIG 2

#define ORXONOX_ARCHITECTURE_32 1
#define ORXONOX_ARCHITECTURE_64 2

/* Finds the compiler type and version.
*/
#if defined( _MSC_VER )
#  define ORXONOX_COMPILER ORXONOX_COMPILER_MSVC
#  define ORXONOX_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
#  define ORXONOX_COMPILER ORXONOX_COMPILER_GNUC
#  define ORXONOX_COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#  define ORXONOX_COMPILER ORXONOX_COMPILER_BORL
#  define ORXONOX_COMP_VER __BCPLUSPLUS__

#else
#  pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead */
#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#  if ORXONOX_COMP_VER >= 1200
#    define FORCEINLINE __forceinline
#  endif
#elif defined(__MINGW32__)
#  if !defined(FORCEINLINE)
#    define FORCEINLINE __inline
#  endif
#else
#  define FORCEINLINE __inline
#endif

/* Finds the current platform */

#if defined( __WIN32__ ) || defined( _WIN32 )
#  define ORXONOX_PLATFORM ORXONOX_PLATFORM_WIN32

#elif defined( __APPLE_CC__)
#  define ORXONOX_PLATFORM ORXONOX_PLATFORM_APPLE

#else
#  define ORXONOX_PLATFORM ORXONOX_PLATFORM_LINUX
#endif

    /* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#  define ORXONOX_ARCH_TYPE ORXONOX_ARCHITECTURE_64
#else
#  define ORXONOX_ARCH_TYPE ORXONOX_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define ORXONOX_QUOTE_INPLACE(x) # x
#define ORXONOX_QUOTE(x) ORXONOX_QUOTE_INPLACE(x)
#define ORXONOX_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32

// Win32 compilers use _DEBUG for specifying debug builds.
#  ifdef _DEBUG
#    define ORXONOX_DEBUG_MODE 1
#  else
#    define ORXONOX_DEBUG_MODE 0
#  endif

// Disable unicode support on MingW at the moment, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLKIT_UNICODE__ in _mingw.h
#  if defined( __MINGW32__ ) && !defined(_STLPORT_VERSION)
#    include<_mingw.h>
#    if defined(__MINGW32_TOOLBOX_UNICODE__)
#      define ORXONOX_UNICODE_SUPPORT 1
#    else
#      define ORXONOX_UNICODE_SUPPORT 0
#    endif
#  else
#    define ORXONOX_UNICODE_SUPPORT 1
#  endif

#endif
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Linux/Apple Settings
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_LINUX || ORXONOX_PLATFORM == ORXONOX_PLATFORM_APPLE


// A quick define to overcome different names for the same function
#  define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#  ifdef DEBUG
#    define ORXONOX_DEBUG_MODE 1
#  else
#    define ORXONOX_DEBUG_MODE 0
#  endif

/* FIXME: Check what this actually is and whether we need it or not
#  if ORXONOX_PLATFORM == ORXONOX_PLATFORM_APPLE
#    define ORXONOX_PLATFORM_LIB "OrxonoxPlatform.bundle"
#  else
// ORXONOX_PLATFORM_LINUX
#    define ORXONOX_PLATFORM_LIB "libOrxonoxPlatform.so"
#  endif
*/

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#  define ORXONOX_UNICODE_SUPPORT 1

#endif

//For apple, we always have a custom config.h file
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_APPLE
#  include "config.h"
#endif

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set ORXONOX_ENDIAN correctly
#ifdef ORXONOX_CONFIG_BIG_ENDIAN
#  define ORXONOX_ENDIAN ORXONOX_ENDIAN_BIG
#else
#  define ORXONOX_ENDIAN ORXONOX_ENDIAN_LITTLE
#endif

// Integer formats of fixed bit width
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#ifdef ORXONOX_DOUBLE_PRECISION
typedef double Real;
#else
typedef float Real;
#endif


#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
//#   pragma warning (disable : 4786)

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
//#   pragma warning (disable : 4503)

// disable: conversion from 'double' to 'float', possible loss of data
// disable: conversion from 'ogg_int64_t' to 'long', possible loss of data
// This has been dealt with in base_properties of the solution since the
// warning primarily occurs in library header files (which are mostly
// included before OrxonoxPlatform.h is)
//#   pragma warning (disable : 4244)

// disable: "conversion from 'size_t' to 'unsigned int', possible loss of data
//#   pragma warning (disable : 4267)

// disable: "truncation from 'double' to 'float'
//#   pragma warning (disable : 4305)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
// This has been dealt with in base_properties of the solution since the
// warning primarily occurs in library header files (which are mostly
// included before OrxonoxPlatform.h is)
//#   pragma warning (disable : 4251)

// disable: 'MultiTypeString' : multiple assignment operators specified
// Used in MultiType and works fine so far
#   pragma warning (disable : 4522)

// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
//#   pragma warning (disable : 4275)

// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception
// specifications in the ANSI C++ draft.
//#   pragma warning( disable : 4290 )

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
//#   pragma warning( disable: 4661)

// disable: deprecation warnings when using CRT calls in VC8
// These show up on all C runtime lib code in VC8, disable since they clutter
// the warnings with things we may not be able to do anything about (e.g.
// generated code from nvparse etc). I doubt very much that these calls
// will ever be actually removed from VC anyway, it would break too much code.
//#	pragma warning( disable: 4996)

// disable: "conditional expression constant", always occurs on 
// ORXONOX_MUTEX_CONDITIONAL when no threading enabled
//#   pragma warning (disable : 201)

#endif


// Define the english written operators like and, or, xor
#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#  include <iso646.h>
#endif

} /* namespace orxonox */

// include visual leak detector to search for memory links
//#include <vld.h>

#endif /* _OrxonoxPlatform_H__ */
