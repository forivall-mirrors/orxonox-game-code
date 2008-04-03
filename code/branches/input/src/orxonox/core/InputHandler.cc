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
 @brief Implementation of the different input handlers.
 */

#include "InputHandler.h"

namespace orxonox
{
  // ###############################
  // ###    InputHandlerGame     ###
  // ###############################

  /**
    @brief standard constructor
  */
  InputHandlerGame::InputHandlerGame()
  {
  }

  /**
    @brief Destructor
  */
  InputHandlerGame::~InputHandlerGame()
  {
  }

  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputHandlerGame::keyPressed(const OIS::KeyEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputHandlerGame::keyReleased(const OIS::KeyEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputHandlerGame::mouseMoved(const OIS::MouseEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandlerGame::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandlerGame::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  // ###############################
  // ###     InputHandlerGUI     ###
  // ###############################

  /**
    @brief standard constructor
  */
  InputHandlerGUI::InputHandlerGUI()
  {
  }

  /**
    @brief Destructor
  */
  InputHandlerGUI::~InputHandlerGUI()
  {
  }

  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputHandlerGUI::keyPressed(const OIS::KeyEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputHandlerGUI::keyReleased(const OIS::KeyEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputHandlerGUI::mouseMoved(const OIS::MouseEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandlerGUI::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandlerGUI::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

}
