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
#include "OrxonoxStableHeaders.h"
#include "CameraManager.h"

#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

#include "core/Core.h"
#include "objects/worldentities/Camera.h"
#include "objects/Scene.h"
#include "util/String.h"

namespace orxonox
{
    CameraManager* CameraManager::singletonRef_s = 0;

    CameraManager::CameraManager(Ogre::Viewport* viewport)
        : viewport_(viewport)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->fallbackCamera_ = 0;
    }

    CameraManager::~CameraManager()
    {
        assert(singletonRef_s != 0);
        singletonRef_s = 0;

        if (this->fallbackCamera_)
        {
            this->fallbackCamera_->getSceneManager()->destroyCamera(this->fallbackCamera_);
COUT(0) << "remove camera-manager" << std::endl;
        }
    }

    Camera* CameraManager::getActiveCamera() const
    {
        if (this->cameraList_.size() > 0)
            return this->cameraList_.front();
        else
            return 0;
    }

    void CameraManager::requestFocus(Camera* camera)
    {
        if (!Core::showsGraphics())
            return;

        // notify old camera (if it exists)
        if (this->cameraList_.size() > 0)
            this->cameraList_.front()->removeFocus();
        else if (this->fallbackCamera_)
        {
            this->fallbackCamera_->getSceneManager()->destroyCamera(this->fallbackCamera_);
            this->fallbackCamera_ = 0;
COUT(0) << "remove fallback camera" << std::endl;
        }

        camera->setFocus(this->viewport_);

        // make sure we don't add it twice
        for (std::list<Camera*>::iterator it = this->cameraList_.begin(); it != this->cameraList_.end(); ++it)
            if ((*it) == camera)
                return;

        // add to list
        this->cameraList_.push_front(camera);
    }

    void CameraManager::releaseFocus(Camera* camera)
    {
        if (!Core::showsGraphics())
            return;

        // notify the cam of releasing the focus
        if (this->cameraList_.front() == camera)
        {
            camera->removeFocus();
            this->cameraList_.pop_front();

            // set new focus if possible
            if (this->cameraList_.size() > 0)
                this->cameraList_.front()->setFocus(this->viewport_);
            else
            {
                // there are no more cameras, create a fallback
                if (!this->fallbackCamera_)
                {
COUT(0) << "create fallback camera" << std::endl;
                    this->fallbackCamera_ = camera->getScene()->getSceneManager()->createCamera(getUniqueNumberString());
                }
                this->viewport_->setCamera(this->fallbackCamera_);
COUT(0) << "use fallback camera" << std::endl;
            }
        }
        else
        {
            this->cameraList_.remove(camera);
        }
    }
}
