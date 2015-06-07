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

/**
    @file
    @ingroup Management CoreGame
    @brief Declaration of DynLib which represents a dynamically loaded module.
*/

#ifndef _Core_DynLib_H__
#define _Core_DynLib_H__

#include "core/CorePrereqs.h"

#include <string>

#if defined(ORXONOX_PLATFORM_WINDOWS)
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif defined(ORXONOX_PLATFORM_UNIX)
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

namespace orxonox
{
    /** %Resource holding data about a dynamic library.
        @remarks
            This class holds the data required to get symbols from
            libraries loaded at run-time (i.e. from DLL's for so's)
        @author
            Adrian Cearnãu (cearny@cearny.ro)
        @since
            27 January 2002
    */
    class _CoreExport DynLib
    {
    protected:
        std::string mName;
        /// Gets the last loading error
        std::string dynlibError(void);
    public:
        /** Default constructor - used by DynLibManager.
            @warning
                Do not call directly
        */
        DynLib( const std::string& name );

        /** Default destructor.
        */
        ~DynLib();

        /** Load the library
        */
        void load();
        /** Unload the library
        */
        void unload();
        /// Get the name of the library
        const std::string& getName(void) const { return mName; }

        /**
            Returns the address of the given symbol from the loaded library.
            @param
                strName The name of the symbol to search for
            @returns
                If the function succeeds, the returned value is a handle to
                the symbol.
            @par
                If the function fails, the returned value is <b>NULL</b>.

        */
        void* getSymbol( const std::string& strName ) const throw();

    protected:

        /// Handle to the loaded library.
        DYNLIB_HANDLE m_hInst;
    };

}

#endif /* _Core_DynLib_H__ */
