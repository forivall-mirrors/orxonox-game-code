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
    class OutputHandler;
    class OutputListener;
    template <class T>
    class Singleton;
    class SubString;
}

namespace Loki
{
    class ScopeGuardImplBase;
    typedef const ScopeGuardImplBase& ScopeGuard;
}

class QDir;
class QString;

// Just so you don't have to include StringUtils.h everywhere just for this
namespace orxonox
{
    extern _UtilExport std::string BLANKSTRING;
}

//-----------------------------------------------------------------------
// Functions (implementations in StringUtils.cc)
//-----------------------------------------------------------------------

namespace orxonox
{
    _UtilExport QDir& operator/=(QDir& lhs, const QDir& rhs);
    _UtilExport QDir& operator/=(QDir& lhs, const QString& rhs);
    _UtilExport QDir operator/(const QDir& lhs, const QDir& rhs);
    _UtilExport QDir operator/(const QDir& lhs, const QString& rhs);
}

#endif /* _UtilPrereqs_H__ */
