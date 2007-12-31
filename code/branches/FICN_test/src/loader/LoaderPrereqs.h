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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  OrxonoxPrereq.cc
 @brief Contains all the necessary forward declarations for all classes, structs and enums.
 */

#ifndef _LoaderPrereqs_H__
#define _LoaderPrereqs_H__

#include "orxonox/OrxonoxPlatform.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if (ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32) && !defined( LOADER_STATIC_BUILD )
#  ifdef LOADER_SHARED_BUILD
#    define _LoaderExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _LoaderExport
#    else
#      define _LoaderExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _LoaderExport  __attribute__ ((visibility("default")))
#else
#  define _LoaderExport
#endif


//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------
namespace loader {
  class LevelLoader;
}

#endif /* _LoaderPrereqs_H__ */
