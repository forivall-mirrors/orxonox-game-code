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
 *      Benjamin Knecht
 *
 */

#ifndef _Camera_H__
#define _Camera_H__

#include <OgrePrerequisites.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>

#include "OrxonoxPrereqs.h"
#include "objects/worldentities/PositionableEntity.h"
#include "objects/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport Camera : public PositionableEntity//, public Tickable
    {
      friend class CameraHandler;

      public:
        Camera();
        virtual ~Camera();

        virtual void tick(float dt);
        void update();

        void requestFocus();
        inline bool hasFocus()
            { return this->bHasFocus_; }

        inline void setDrag(bool bDrag)
            { this->bDrag_ = bDrag; }
        inline bool getDrag() const
            { return this->bDrag_; }

      private:
        void removeFocus();
        void setFocus(Ogre::Camera* ogreCam);

        Ogre::Camera* cam_;
        Ogre::SceneNode* cameraNode_;
        Ogre::Vector3 oldPos;
        bool bHasFocus_;
        bool bDrag_;
    };
}

#endif /* _Camera_H__ */
