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
 *      Benjamin Knecht
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

 /**
 @file
 @brief Handles the instances of Camera class
 @author Benjamin Knecht <beni_at_orxonox.net>
  */

#ifndef _CameraManager_H__
#define _CameraManager_H__

#include "OrxonoxPrereqs.h"

#include <cassert>
#include <list>
#include "util/OgreForwardRefs.h"
#include "util/Singleton.h"
#include "core/OrxonoxClass.h"
#include "core/SmartPtr.h"

namespace orxonox
{
    class _OrxonoxExport CameraManager : public Singleton<CameraManager>, public OrxonoxClass
    {
            friend class Singleton<CameraManager>;
        public:
            CameraManager();
            ~CameraManager();

            Camera* getActiveCamera() const;

            void requestFocus(Camera* camera);
            void releaseFocus(Camera* camera);

            void useCamera(Ogre::Camera* camera);

            static CameraManager* getInstancePtr() { return singletonPtr_s; }

        private:
            CameraManager(const CameraManager&); // don't use

            std::list<Camera*>    cameraList_;
            Ogre::Viewport*       viewport_;

            static CameraManager* singletonPtr_s;
    };
}

#endif /* _CameraManager_H__ */
