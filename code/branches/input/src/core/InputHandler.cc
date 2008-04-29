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

#include "InputHandler.h"
#include "Debug.h"
#include "util/Convert.h"
#include "core/CommandExecutor.h"

namespace orxonox
{
  // ###############################
  // ######     KeyBinder     ######
  // ###############################

  /**
    @brief Constructor that does as little as necessary.
  */
  KeyBinder::KeyBinder()
  {
    clearBindings();
  }

  /**
    @brief Destructor
  */
  KeyBinder::~KeyBinder()
  {
  }

  /**
    @brief Overwrites all bindings with ""
  */
  void KeyBinder::clearBindings()
  {
    for (int i = 0; i < numberOfKeys_s; i++)
    {
      bindingsKeyPress_  [i] = "";
      bindingsKeyRelease_[i] = "";
      bindingsKeyHold_   [i] = "";
    }
    for (int i = 0; i < numberOfMouseButtons_s; i++)
    {
      bindingsMouseButtonPress_  [i] = "";
      bindingsMouseButtonRelease_[i] = "";
      bindingsMouseButtonHold_   [i] = "";
    }
    for (int i = 0; i < numberOfJoyStickButtons_s; i++)
    {
      bindingsJoyStickButtonPress_  [i] = "";
      bindingsJoyStickButtonRelease_[i] = "";
      bindingsJoyStickButtonHold_   [i] = "";
    }
  }

  /**
    @brief Loads the key and button bindings.
    @return True if loading succeeded.
  */
  bool KeyBinder::loadBindings()
  {
    COUT(ORX_DEBUG) << "KeyBinder: Loading key bindings..." << std::endl;

    // clear all the bindings at first.
    clearBindings();

    // TODO: Insert the code to load the bindings from file.
    bindingsKeyPress_[OIS::KC_NUMPADENTER] = "activateConsole";
    bindingsKeyPress_[OIS::KC_ESCAPE] = "exit";
    bindingsKeyHold_[OIS::KC_U] = "exec disco.txt";

    COUT(ORX_DEBUG) << "KeyBinder: Loading key bindings done." << std::endl;
    return true;
  }


  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool KeyBinder::keyPressed(const OIS::KeyEvent &e)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsKeyPress_[int(e.key)];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }
    
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool KeyBinder::keyReleased(const OIS::KeyEvent &e)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsKeyRelease_[int(e.key)];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  /**
    @brief Event handler for the keyHeld Event.
    @param e Event information
  */
  bool KeyBinder::keyHeld(const OIS::KeyEvent &e)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsKeyHold_[int(e.key)];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool KeyBinder::mouseMoved(const OIS::MouseEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool KeyBinder::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsMouseButtonPress_[int(id)];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool KeyBinder::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsMouseButtonRelease_[int(id)];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  /**
    @brief Event handler for the mouseHeld Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool KeyBinder::mouseHeld(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsMouseButtonHold_[int(id)];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  bool KeyBinder::buttonPressed(const OIS::JoyStickEvent &arg, int button)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsJoyStickButtonPress_[button];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  bool KeyBinder::buttonReleased(const OIS::JoyStickEvent &arg, int button)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsJoyStickButtonRelease_[button];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  bool KeyBinder::buttonHeld(const OIS::JoyStickEvent &arg, int button)
  {
    // find the appropriate key binding
    std::string cmdStr = bindingsJoyStickButtonHold_[button];
    if (cmdStr != "")
    {
      CommandExecutor::execute(cmdStr);
      COUT(3) << "Executing command: " << cmdStr << std::endl;
    }

    return true;
  }

  bool KeyBinder::axisMoved(const OIS::JoyStickEvent &arg, int axis)
  {
    return true;
  }

  bool KeyBinder::sliderMoved(const OIS::JoyStickEvent &arg, int id)
  {
    return true;
  }

  bool KeyBinder::povMoved(const OIS::JoyStickEvent &arg, int id)
  {
    return true;
  }



  // ###############################
  // ###     GUIInputHandler     ###
  // ###############################

  ///**
  //  @brief standard constructor
  //*/
  //GUIInputHandler::GUIInputHandler()
  //{
  //}

  ///**
  //  @brief Destructor
  //*/
  //GUIInputHandler::~GUIInputHandler()
  //{
  //}

  ///**
  //  @brief Event handler for the keyPressed Event.
  //  @param e Event information
  //*/
  //bool GUIInputHandler::keyPressed(const OIS::KeyEvent &e)
  //{
		////CEGUI::System::getSingleton().injectKeyDown( arg.key );
		////CEGUI::System::getSingleton().injectChar( arg.text );
  //  return true;
  //}

  ///**
  //  @brief Event handler for the keyReleased Event.
  //  @param e Event information
  //*/
  //bool GUIInputHandler::keyReleased(const OIS::KeyEvent &e)
  //{
		////CEGUI::System::getSingleton().injectKeyUp( arg.key );
  //  return true;
  //}

  ///**
  //  @brief Event handler for the mouseMoved Event.
  //  @param e Event information
  //*/
  //bool GUIInputHandler::mouseMoved(const OIS::MouseEvent &e)
  //{
		////CEGUI::System::getSingleton().injectMouseMove( arg.state.X.rel, arg.state.Y.rel );
  //  return true;
  //}

  ///**
  //  @brief Event handler for the mousePressed Event.
  //  @param e Event information
  //  @param id The ID of the mouse button
  //*/
  //bool GUIInputHandler::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  //{
		////CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));
  //  return true;
  //}

  ///**
  //  @brief Event handler for the mouseReleased Event.
  //  @param e Event information
  //  @param id The ID of the mouse button
  //*/
  //bool GUIInputHandler::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  //{
		////CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));
  //  return true;
  //}

}
