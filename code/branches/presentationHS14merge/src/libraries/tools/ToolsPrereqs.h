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
@brief
    Shared library macros, enums, constants and forward declarations for the tools module
*/

#ifndef _ToolsPrereqs_H__
#define _ToolsPrereqs_H__

#include "OrxonoxConfig.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(TOOLS_STATIC_BUILD)
#  ifdef TOOLS_SHARED_BUILD
#    define _ToolsExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _ToolsExport
#    else
#      define _ToolsExport __declspec(dllimport)
#    endif
#  endif
#  define _ToolsPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _ToolsExport  __attribute__ ((visibility("default")))
#  define _ToolsPrivate __attribute__ ((visibility("hidden")))
#else
#  define _ToolsExport
#  define _ToolsPrivate
#endif

//-----------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------

namespace orxonox
{
    namespace LODParticle
    {
        enum Value
        {
            Off = 0,
            Low = 1,
            Normal = 2,
            High = 3
        };
    }
}

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class BillboardSet;
    class BulletDebugDrawer;
    class DebugDrawer;
    class IcoSphere;
    class Mesh;
    class ParticleInterface;
    class RealTimer;
    class ResourceCollection;
    class ResourceLocation;
    class Shader;
    class Tickable;
    class TimeFactorListener;
    class Timer;
}

namespace Ogre
{
    // OGRE Wiki adapted code
    class DynamicLines;
    class DynamicRenderable;
}

#endif /* _ToolsPrereqs_H__ */
