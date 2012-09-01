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

#ifndef _BillboardSet_H__
#define _BillboardSet_H__

#include "tools/ToolsPrereqs.h"
#include "util/OgreForwardRefs.h"

namespace orxonox
{
    class _ToolsExport BillboardSet
    {
        public:
            BillboardSet();
            ~BillboardSet();

            void setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, int count = 1);
            void setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, const ColourValue& colour, int count = 1);
            void setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, const Vector3& position, int count = 1);
            void setBillboardSet(Ogre::SceneManager* scenemanager, const std::string& file, const ColourValue& colour, const Vector3& position, int count = 1);

            inline Ogre::BillboardSet* getBillboardSet()
                { return this->billboardSet_; }
            inline Ogre::SceneManager* getSceneManager() const
                { return this->scenemanager_; }

            const std::string& getName() const;

            void setVisible(bool visible);
            bool getVisible() const;

            void setColour(const ColourValue& colour);
            const ColourValue& getColour() const;

            void setMaterial(const std::string& material);
            const std::string& getMaterial() const;

        private:
            void destroyBillboardSet();

            Ogre::BillboardSet* billboardSet_;
            Ogre::SceneManager* scenemanager_;

            static unsigned int billboardSetCounter_s;
    };
}

#endif /* _BillboardSet_H__ */
