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

#include "OrxonoxPrereqs.h"

#include "util/OgreForwardRefs.h"
#include "core/WindowEventListener.h"
#include "tools/interfaces/Tickable.h"
#include "tools/interfaces/TimeFactorListener.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    class _OrxonoxExport Camera : public StaticEntity, public Tickable,  public TimeFactorListener, public WindowEventListener
    {
        friend class CameraManager;

        public:
            Camera(Context* context);
            virtual ~Camera();

            void setConfigValues();
            virtual void tick(float dt);

            void requestFocus();
            void releaseFocus();

            inline Ogre::Camera* getOgreCamera()
               { return this->camera_; }

            inline bool hasFocus()
                { return this->bHasFocus_; }

            void setDrag(bool bDrag);
            inline bool getDrag() const
                { return this->bDrag_; }

        private:
            void removeFocus();
            void setFocus();

            void configvaluecallback_changedFovAndAspectRatio();
            void configvaluecallback_changedNearClipDistance();

            void windowResized(unsigned int newWidth, unsigned int newHeight);

            Ogre::Camera*    camera_;
            Ogre::SceneNode* cameraNode_;
            float            nearClipDistance_;
            bool             bHasFocus_;
            bool             bDrag_;
            bool             lastDtLagged_;
            float            fov_;
            float            aspectRatio_;
    };
}

#endif /* _Camera_H__ */
