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
@brief
    Shared library macros, enums, constants and forward declarations for the util library
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
#  define _UtilPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _UtilExport  __attribute__ ((visibility("default")))
#  define _UtilPrivate __attribute__ ((visibility("hidden")))
#else
#  define _UtilExport
#  define _UtilPrivate
#endif

//-----------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------

namespace orxonox
{
    namespace ScopeID
    {
        //!A list of available scopes for the Scope template.
        enum Value
        {
            Root,
            Graphics
        };
    }
}

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class AdditionalContextListener;
    class BaseWriter;
    class Clock;
    class ConsoleWriter;
    class Exception;
    class ExprParser;
    class LogWriter;
    class MemoryWriter;
    class MultiType;
    class OutputListener;
    class OutputManager;
    class OutputStream;
    template <ScopeID::Value>
    class Scope;
    template <class, ScopeID::Value>
    class ScopedSingleton;
    class ScopeListener;
    template <class T>
    class SharedPtr;
    class SignalHandler;
    template <class T>
    class Singleton;
    class SubcontextOutputListener;
    class SubString;
}

namespace Ogre
{
    class Radian;
    class Degree;
    class Vector2;
    class Vector3;
    class Vector4;
    class Matrix3;
    class Matrix4;
    class Quaternion;
    class ColourValue;
}
namespace orxonox
{
    using Ogre::Radian;
    using Ogre::Degree;
    using Ogre::Vector2;
    using Ogre::Vector3;
    using Ogre::Vector4;
    using Ogre::Matrix3;
    using Ogre::Matrix4;
    using Ogre::Quaternion;
    using Ogre::ColourValue;
}

namespace Loki
{
    class ScopeGuardImplBase;
    typedef const ScopeGuardImplBase& ScopeGuard;
}

// Just so you don't have to include StringUtils.h everywhere just for this
namespace orxonox
{
    extern _UtilExport std::string BLANKSTRING;
}


#endif /* _UtilPrereqs_H__ */
