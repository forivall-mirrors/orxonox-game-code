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
    Shared library macros, enums, constants and forward declarations for the core library
*/

#ifndef _CorePrereqs_H__
#define _CorePrereqs_H__

#include "OrxonoxConfig.h"
#include <boost/version.hpp>

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined( CORE_STATIC_BUILD )
#  ifdef CORE_SHARED_BUILD
#    define _CoreExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _CoreExport
#    else
#      define _CoreExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _CoreExport  __attribute__ ((visibility("default")))
#else
#  define _CoreExport
#endif

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

namespace orxonox
{
}

//-----------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------

namespace orxonox
{
}

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class CommandLineParser;
    class CommandLineArgument;
    class Core;
    class PathConfig;
}

// Boost
namespace boost
{
#if (BOOST_VERSION < 104400)
    namespace filesystem
    {
        struct path_traits;
        template <class String, class Traits> class basic_path;
        typedef basic_path<std::string, path_traits> path;
    }
#else
    namespace filesystem2
    {
        struct path_traits;
        template <class String, class Traits> class basic_path;
        typedef basic_path<std::string, path_traits> path;
    }
    namespace filesystem
    {
        using filesystem2::basic_path;
        using filesystem2::path_traits;
        using filesystem2::path;
    }
#endif
}

#endif /* _CorePrereqs_H__ */
