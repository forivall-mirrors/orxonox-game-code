/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/

// 08/11/2009: Small adjustments for Orxonox by Fabian 'x3n' Landau

#include "DynLib.h"

#include "util/Exception.h"

#ifdef ORXONOX_PLATFORM_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  ifndef NOMINMAX
#    define NOMINMAX // required to stop windows.h messing up std::min
#  endif
#  include <windows.h>
#endif

#ifdef ORXONOX_PLATFORM_UNIX
#  include <dlfcn.h>
#endif

#ifdef ORXONOX_PLATFORM_APPLE
#   include <OSX/macUtils.h> // OGRE include
#endif

namespace orxonox
{
    //-----------------------------------------------------------------------
    DynLib::DynLib( const std::string& name )
    {
        mName = name;
        m_hInst = NULL;
    }

    //-----------------------------------------------------------------------
    DynLib::~DynLib()
    {
    }

    //-----------------------------------------------------------------------
    void DynLib::load()
    {
        // Log library load
        COUT(2) << "Loading module " << mName << std::endl;

        std::string name = mName;
#ifdef ORXONOX_PLATFORM_LINUX
        // dlopen() does not add .so to the filename, like windows does for .dll
        if (name.substr(name.length() - 3, 3) != ".so")
           name += ".so";
#elif defined(ORXONOX_PLATFORM_APPLE)
        // dlopen() does not add .dylib to the filename, like windows does for .dll
        if (name.substr(name.length() - 6, 6) != ".dylib")
            name += ".dylib";
#elif defined(ORXONOX_PLATFORM_WINDOWS)
        // Although LoadLibraryEx will add .dll itself when you only specify the library name,
        // if you include a relative path then it does not. So, add it to be sure.
        if (name.substr(name.length() - 4, 4) != ".dll")
            name += ".dll";
#endif

        m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );

        if (!m_hInst)
            ThrowException(
                General,
                "Could not load dynamic library " + mName +
                ".  System Error: " + dynlibError());
    }

    //-----------------------------------------------------------------------
    void DynLib::unload()
    {
        // Log library unload
        COUT(4) << "Unloading module " << mName << std::endl;

        if (DYNLIB_UNLOAD( m_hInst ))
        {
            ThrowException(
                General,
                "Could not unload dynamic library " + mName +
                ".  System Error: " + dynlibError());
        }

    }

    //-----------------------------------------------------------------------
    void* DynLib::getSymbol( const std::string& strName ) const throw()
    {
        return (void*)DYNLIB_GETSYM( m_hInst, strName.c_str() );
    }
    //-----------------------------------------------------------------------
    std::string DynLib::dynlibError( void )
    {
#if defined(ORXONOX_PLATFORM_WINDOWS)
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0,
            NULL
            );
        std::string ret = (char*)lpMsgBuf;
        // Free the buffer.
        LocalFree( lpMsgBuf );
        return ret;
#elif defined(ORXONOX_PLATFORM_UNIX)
        return std::string(dlerror());
#else
        return "";
#endif
    }
}
