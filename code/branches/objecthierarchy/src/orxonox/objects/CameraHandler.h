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

#include <list>

#include <OgreCamera.h>

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"

namespace orxonox
{
  class _OrxonoxExport CameraHandler
  {

    public:
      inline static CameraHandler* getInstance() { if (!CameraHandler::singletonRef) CameraHandler::singletonRef = new CameraHandler(); return CameraHandler::singletonRef; }
      inline ~CameraHandler() { CameraHandler::singletonRef = NULL; }
      inline Camera* getActiveCamera() { return this->focusList_.back(); }
      /*void registerCamera(Camera* newCam);
      void changeActiveCamera(Camera* setCam);*/
      void requestFocus(Camera* requestCam);
      void releaseFocus(Camera* cam);

    private:
      CameraHandler();
      //bool isInVector(Camera* cam);

    private:
      static CameraHandler* singletonRef;
      Ogre::Camera* cam_;
      std::list<Camera*> focusList_;
      //std::vector<Camera*> cams_;

  };
}

#endif /* _Camera_Handler_H__ */
