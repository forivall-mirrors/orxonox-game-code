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
 *      ...
 *
 */

/**
  @file
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _AudioPrereqs_H__
#define _AudioPrereqs_H__

#include "OrxonoxPlatform.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if (ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32) && !defined( AUDIO_STATIC_BUILD )
#  ifdef AUDIO_SHARED_BUILD
#    define _AudioExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _AudioExport
#    else
#      define _AudioExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _AudioExport  __attribute__ ((visibility("default")))
#else
#  define _AudioExport
#endif


//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------
namespace audio
{
  class AudioBuffer;
  class AudioManager;
  class AudioSource;
  class AudioStream;
}

#endif /* _AudioPrereqs_H__ */
