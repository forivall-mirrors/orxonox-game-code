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
 *      ...
 *
 */

 /**
 @file  CameraHandler.h
 @brief Handles the instances of Camera class
 @author Benjamin Knecht <beni_at_orxonox.net>
  */

#ifndef _Camera_Handler_H__
#define _Camera_Handler_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <OgreCamera.h>

#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport CameraHandler
    {
        public:
            static CameraHandler& getInstance();

            Camera* getActiveCamera() const;

            void requestFocus(Camera* camera);
            void releaseFocus(Camera* camera);

        private:
            CameraHandler();
            ~CameraHandler() {}

            std::list<Camera*> cameraList_;
    };
}

#endif /* _Camera_Handler_H__ */
