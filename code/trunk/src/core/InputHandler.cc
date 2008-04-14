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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
 @file
 @brief Implementation of the different input handlers.
 */

#include "Debug.h"
#include "util/Convert.h"
#include "InputEventListener.h"
#include "InputEvent.h"
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
    @brief Loads the key bindings from the ini file.
    Currently, this is just a simple test routine that fills the list with numbers.
  */
  bool InputHandlerGame::loadBindings()
  {
    for (int i = 0; i < numberOfKeys_s; i++)
    {
      // simply write the key number (i) in the string
      this->bindingsKeyPressed_[i] = getConvertedValue<int, std::string>(i);
      this->bindingsKeyReleased_[i] = getConvertedValue<int, std::string>(i);
    }
    return true;
  }

  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputHandlerGame::keyPressed(const OIS::KeyEvent &e)
  {
    if (e.key == OIS::KC_ESCAPE)
    {
      InputEvent e = {1, true, 0, 0, 0};
      InputHandlerGame::callListeners(e);
    }
    else
    {
      // find the appropriate key binding
      std::string cmdStr = bindingsKeyPressed_[int(e.key)];
      //COUT(3) << cmdStr << " pressed" << std::endl;
    }
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputHandlerGame::keyReleased(const OIS::KeyEvent &e)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsKeyReleased_[int(e.key)];
    //COUT(3) << cmdStr << " released" << std::endl;
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

  /**
    @brief Calls all the objects from classes that derive from InputEventListener.
    @param evt The input event that occured.
  */
  inline void InputHandlerGame::callListeners(orxonox::InputEvent &evt)
  {
    for (Iterator<InputEventListener> it = ObjectList<InputEventListener>::start(); it; )
    {
      if (it->bActive_)
        (it++)->eventOccured(evt);
      else
        it++;
    }
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
		//CEGUI::System::getSingleton().injectKeyDown( arg.key );
		//CEGUI::System::getSingleton().injectChar( arg.text );
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputHandlerGUI::keyReleased(const OIS::KeyEvent &e)
  {
		//CEGUI::System::getSingleton().injectKeyUp( arg.key );
    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputHandlerGUI::mouseMoved(const OIS::MouseEvent &e)
  {
		//CEGUI::System::getSingleton().injectMouseMove( arg.state.X.rel, arg.state.Y.rel );
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandlerGUI::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
		//CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));
    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandlerGUI::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
		//CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));
    return true;
  }

}
