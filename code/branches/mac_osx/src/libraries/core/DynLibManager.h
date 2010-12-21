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
    @brief Declaration of DynLibManager, used to load modules at runtime.
*/

#ifndef _Core_DynLibManager_H__
#define _Core_DynLibManager_H__

#include "CorePrereqs.h"

#include <map>
#include <string>
#include "util/Singleton.h"

namespace orxonox
{
    /** Manager for Dynamic-loading Libraries.
        @remarks
            This manager keeps a track of all the open dynamic-loading
            libraries, opens them and returns references to already-open
            libraries.
    */
    class _CoreExport DynLibManager: public Singleton<DynLibManager>
    {
        friend class Singleton<DynLibManager>;

        protected:
            typedef std::map<std::string, DynLib*> DynLibList;
            DynLibList mLibList;

        public:
            /**
            @brief
                Default constructor.
            @note
                Should never be called as the singleton is automatically
                created during the creation of the Root object.
            @see
                Root::Root
            */
            DynLibManager();

            /**
            @brief
                Default destructor.
            @see
                Root::~Root
            */
            virtual ~DynLibManager();

            /**
            @brief
                Loads the passed library.
            @param filename
                The name of the library. The extension can be omitted
            */
            DynLib* load(const std::string& filename);

            /**
            @brief
                Unloads the passed library.
            @param lib
                A pointer to the library object
            */
            void unload(DynLib* lib);

        private:
            static DynLibManager* singletonPtr_s;
    };
}

#endif /* _Core_DynLibManager_H__ */
