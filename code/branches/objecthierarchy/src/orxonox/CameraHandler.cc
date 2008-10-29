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
#include "OrxonoxStableHeaders.h"
#include "CameraHandler.h"

#include <OgreViewport.h>

#include "core/Core.h"
#include "objects/worldentities/Camera.h"


namespace orxonox
{
    CameraHandler* CameraHandler::singletonRef_s = 0;

    CameraHandler::CameraHandler(Ogre::Viewport* viewport)
        : viewport_(viewport)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;
    }

    CameraHandler::~CameraHandler()
    {
        assert(singletonRef_s != 0);
        singletonRef_s = 0;
    }

    Camera* CameraHandler::getActiveCamera() const
    {
        if (this->cameraList_.size() > 0)
            return this->cameraList_.front();
        else
            return 0;
    }

    void CameraHandler::requestFocus(Camera* camera)
    {
        if (!Core::showsGraphics())
            return;

        // notify old camera (if it exists)
        if (this->cameraList_.size() > 0)
            this->cameraList_.front()->removeFocus();

        camera->setFocus(this->viewport_);

        // add to list
        std::list<Camera*>::iterator it;
        for (it = this->cameraList_.begin(); it != this->cameraList_.end(); ++it)
        {
            if ((*it) == camera)
                return; // make sure we don't add it twice
        }
        this->cameraList_.push_front(camera);
    }

    void CameraHandler::releaseFocus(Camera* camera)
    {
        if (!Core::showsGraphics())
            return;

        // notify the cam of releasing the focus
        if (this->cameraList_.front() == camera)
        {
            camera->removeFocus();
            this->cameraList_.pop_front();

            // set new focus if necessary
            if (cameraList_.size() > 0)
                cameraList_.front()->setFocus(this->viewport_);
        }
        else
        {
            this->cameraList_.remove(camera);
        }
    }
}
