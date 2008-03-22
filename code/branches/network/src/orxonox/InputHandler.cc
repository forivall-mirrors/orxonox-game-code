/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
 @file
 @brief Implementation of a little Input handler that distributes everything
        coming from OIS.
 */

#include "OrxonoxStableHeaders.h"

#include <OgreRenderWindow.h>

#include "InputHandler.h"

namespace orxonox
{
  //using namespace OIS;

  InputHandler::InputHandler()
  {
    RegisterObject(InputHandler);
  }

  void InputHandler::initialise(Ogre::RenderWindow *renderWindow)
  {
    if (!inputSystem_)
    {
      // Setup basic variables
      OIS::ParamList paramList;
      size_t windowHnd = 0;
      std::ostringstream windowHndStr;

      // Get window handle
      renderWindow->getCustomAttribute("WINDOW", &windowHnd);

      // Fill parameter list
      windowHndStr << (unsigned int)windowHnd;
      paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

      // Create inputsystem
      inputSystem_ = OIS::InputManager::createInputSystem(paramList);

      // If possible create a buffered keyboard
      // (note: if below line doesn't compile, try:  if (inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0) {
      if (inputSystem_->numKeyboards() > 0)
      {
        //if (inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0)
        //{
        keyboard_ = static_cast<OIS::Keyboard*>(inputSystem_->createInputObject(OIS::OISKeyboard, true));
        keyboard_->setEventCallback(this);
      }

      // If possible create a buffered mouse
      // (note: if below line doesn't compile, try:  if (inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0) {
      if (inputSystem_->numMice() > 0 )
      {
        //if (inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0)
        //{
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        mouse_->setEventCallback(this);

        // Get window size
        unsigned int width, height, depth;
        int left, top;
        renderWindow->getMetrics(width, height, depth, left, top);

        // Set mouse region
        this->setWindowExtents(width, height);
      }
    }
  }

  void InputHandler::tick(float dt)
  {
    // capture all the input. That calls the event handlers.
    if (mouse_)
    {
      mouse_->capture();
    }

    if (keyboard_)
    {
      keyboard_->capture();
    }
  }

  void InputHandler::setWindowExtents(int width, int height)
  {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mouse_->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
  }

  bool InputHandler::keyPressed(const OIS::KeyEvent &e)
  {
    return true;
  }

  bool InputHandler::keyReleased(const OIS::KeyEvent &e)
  {
    return true;
  }

  bool InputHandler::mouseMoved(const OIS::MouseEvent &e)
  {
    return true;
  }

  bool InputHandler::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  bool InputHandler::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

}
