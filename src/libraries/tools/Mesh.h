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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Mesh_H__
#define _Mesh_H__

#include "tools/ToolsPrereqs.h"
#include "util/OgreForwardRefs.h"

namespace orxonox
{
    class _ToolsExport Mesh
    {
        public:
            Mesh();
            ~Mesh();

            void setMeshSource(Ogre::SceneManager* scenemanager, const std::string& file);

            inline Ogre::Entity* getEntity()
                { return this->entity_; }

            const std::string& getName() const;

            void setMaterial(const std::string& name);
            const std::string& getMaterial() const;

            void setVisible(bool bVisible);
            bool isVisible() const;

            void setCastShadows(bool bCastShadows);
            inline bool getCastShadows() const
                { return this->bCastShadows_; }

        private:
            Ogre::Entity* entity_;
            bool bCastShadows_;
            Ogre::SceneManager* scenemanager_;

            static unsigned int meshCounter_s;
    };
}

#endif /* _Mesh_H__ */
