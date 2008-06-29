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

namespace orxonox
{
    class _OrxonoxExport Camera
    {
      friend class CameraHandler;
      public:
        Camera(Ogre::SceneNode* node = NULL);
        virtual ~Camera();

        void setPositionNode(Ogre::SceneNode* node);
        Ogre::SceneNode* getCameraNode() { return this->positionNode_; }
        // maybe also BaseObject
        void setTargetNode(Ogre::SceneNode* obj);

        Ogre::Camera* cam_;

        void tick(float dt);
        void update();
        bool hasFocus() { return this->bHasFocus_; }

      private:
        void removeFocus();
        void setFocus(Ogre::Camera* ogreCam);

      private:
        Ogre::SceneNode* targetNode_;
        Ogre::SceneNode* positionNode_;
        Ogre::SceneNode* cameraNode_;
        Ogre::Vector3 oldPos;
        bool bHasFocus_;
    };
}

#endif /* _Camera_H__ */
