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

#include "CameraManager.h"

#include <cassert>

#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCompositorManager.h>

#include "core/GameMode.h"
#include "core/GraphicsManager.h"
#include "core/object/ObjectList.h"
#include "core/singleton/ScopedSingletonWrapper.h"
#include "tools/Shader.h"
#include "graphics/Camera.h"

namespace orxonox
{
    ManageScopedSingleton(CameraManager, ScopeID::Graphics, false);

    CameraManager::CameraManager()
    {
        assert(GameMode::showsGraphics());
    }

    CameraManager::~CameraManager()
    {
    }

    Camera* CameraManager::getActiveCamera() const
    {
        if (!this->cameraList_.empty())
            return this->cameraList_.front();
        else
            return 0;
    }

    void CameraManager::requestFocus(Camera* camera)
    {
        // notify old camera (if it exists)
        if (!this->cameraList_.empty())
            this->cameraList_.front()->removeFocus();

        camera->setFocus();

        // make sure we don't add it twice
        for (std::list<Camera*>::iterator it = this->cameraList_.begin(); it != this->cameraList_.end();)
            if ((*it) == camera)
                this->cameraList_.erase(it++);
            else
                ++it;
        // add to list
        this->cameraList_.push_front(camera);
    }

    void CameraManager::releaseFocus(Camera* camera)
    {
        // notify the cam of releasing the focus
        if (!this->cameraList_.empty() && this->cameraList_.front() == camera)
        {
            camera->removeFocus();
            this->cameraList_.pop_front();

            // set new focus if possible
            if (!this->cameraList_.empty())
                this->cameraList_.front()->setFocus();
            else
                this->useCamera(NULL);
        }
        else
            this->cameraList_.remove(camera);
    }

    void CameraManager::useCamera(Ogre::Camera* camera)
    {
        GraphicsManager::getInstance().setCamera(camera);
    }
}
