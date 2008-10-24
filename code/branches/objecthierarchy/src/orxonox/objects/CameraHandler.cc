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

#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

#include "core/ObjectList.h"
#include "Camera.h"
#include "GraphicsEngine.h"

#include <OgreCamera.h>

namespace orxonox {

  CameraHandler* CameraHandler::singletonRef = NULL;

  CameraHandler::CameraHandler()
  {
    this->cam_ = GraphicsEngine::getInstance().getLevelSceneManager()->createCamera("Cam");
    GraphicsEngine::getInstance().getViewport()->setCamera(this->cam_);
    this->cam_->setNearClipDistance(1);
    //GraphicsEngine::getInstance().getRenderWindow()->addViewport(this->cam_, 2, 0.4, 0.4, 0.2, 0.2);
    /*this->activeCamera_ = *ObjectList<Camera>::begin();
    this->activeCamera_->cam_ = this->cam_;*/
  }

  void CameraHandler::requestFocus(Camera* requestCam)
  {
    // notify old camera (if it exists)
    if(focusList_.size() > 0)
      focusList_.back()->removeFocus();
    // add to list
    focusList_.push_back(requestCam);
    // set focus to new camera and update (if necessary)
    if(!requestCam->hasFocus()) {
      requestCam->setFocus(this->cam_);
      requestCam->update();
    }
    // delete dublicates
    focusList_.unique();
  }

  void CameraHandler::releaseFocus(Camera* cam)
  {
    // notify the cam of releasing the focus
    if(cam->hasFocus())
      cam->removeFocus();
    // delete camera from list
    focusList_.remove(cam);
    // set new focus if necessary
    if(focusList_.size() > 0 && !(focusList_.back()->hasFocus()))
      focusList_.back()->setFocus(this->cam_);
  }
/*
  void CameraHandler::changeActiveCamera(Camera* setCam)
  {
    cam_->getParentSceneNode()->detachObject(cam_);
    //setCam->attachCamera(cam_);
    activeCamera_ = setCam;
  }
*/
/*  bool isInVector(Camera* cam)
  {
    for(std::vector<Camera*>::iterator it = cams_.begin(); it != cams_.end(); it++)
    {
      if (*it == cam) return true;
    }
    return false;
  }*/
}
