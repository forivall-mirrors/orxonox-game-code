/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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
 *      Fabian 'x3n' Landau
 *
 */

/**
  @file
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _UtilPrereqs_H__
#define _UtilPrereqs_H__

#include "OrxonoxConfig.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined( UTIL_STATIC_BUILD )
#  ifdef UTIL_SHARED_BUILD
#    define _UtilExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _UtilExport
#    else
#      define _UtilExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _UtilExport  __attribute__ ((visibility("default")))
#else
#  define _UtilExport
#endif


//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------
namespace orxonox
{
    class Exception;
    class ExprParser;
    class IntVector2;
    class IntVector3;
    class MultiType;
    class OutputBuffer;
    class OutputBufferListener;
    class OutputHandler;
    class SignalHandler;
    class SubString;
}

#endif /* _UtilPrereqs_H__ */
