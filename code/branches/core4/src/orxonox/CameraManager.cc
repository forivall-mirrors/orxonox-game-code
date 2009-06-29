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

#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCompositorManager.h>

#include "util/StringUtils.h"
#include "core/GameMode.h"
#include "core/ObjectList.h"
#include "tools/Shader.h"
#include "objects/worldentities/Camera.h"
#include "objects/Scene.h"
#include "gui/GUIManager.h"

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
            this->fallbackCamera_->getSceneManager()->destroyCamera(this->fallbackCamera_);
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
        if (!GameMode::showsGraphics())
            return;

        // notify old camera (if it exists)
        if (this->cameraList_.size() > 0)
            this->cameraList_.front()->removeFocus();
        else if (this->fallbackCamera_)
        {
            this->fallbackCamera_->getSceneManager()->destroyCamera(this->fallbackCamera_);
            this->fallbackCamera_ = 0;
        }

        camera->setFocus();

        // make sure we don't add it twice
        for (std::list<Camera*>::iterator it = this->cameraList_.begin(); it != this->cameraList_.end(); ++it)
            if ((*it) == camera)
                return;

        // add to list
        this->cameraList_.push_front(camera);
    }

    void CameraManager::releaseFocus(Camera* camera)
    {
        if (!GameMode::showsGraphics())
            return;

        // notify the cam of releasing the focus
        if (this->cameraList_.front() == camera)
        {
            camera->removeFocus();
            this->cameraList_.pop_front();

            // set new focus if possible
            if (this->cameraList_.size() > 0)
                this->cameraList_.front()->setFocus();
            else
            {
                // there are no more cameras, create a fallback
                if (!this->fallbackCamera_)
                    this->fallbackCamera_ = camera->getScene()->getSceneManager()->createCamera(getUniqueNumberString());
                this->useCamera(this->fallbackCamera_);
            }
        }
        else
        {
            this->cameraList_.remove(camera);
        }
    }

    void CameraManager::useCamera(Ogre::Camera* camera)
    {
        // This workaround is needed to avoid weird behaviour with active compositors while
        // switching the camera (like freezing the image)
        //
        // Last known Ogre version needing this workaround:
        // 1.4.8

        // deactivate all compositors
        {
            Ogre::ResourceManager::ResourceMapIterator iterator = Ogre::CompositorManager::getSingleton().getResourceIterator();
            while (iterator.hasMoreElements())
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(this->viewport_, iterator.getNext()->getName(), false);
        }

        this->viewport_->setCamera(camera);
        GUIManager::getInstance().setCamera(camera);

        // reactivate all visible compositors
        {
            for (ObjectList<Shader>::iterator it = ObjectList<Shader>::begin(); it != ObjectList<Shader>::end(); ++it)
                it->updateVisibility();
        }
    }
}
