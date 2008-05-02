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
#include "ConfigValueIncludes.h"
#include "CoreIncludes.h"
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
    RegisterObject(KeyBinder);
    clearBindings();

    std::string keyNames[] = {
    "UNASSIGNED",
		"ESCAPE",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"0",
		"MINUS",
		"EQUALS",
		"BACK",
		"TAB",
		"Q",
		"W",
		"E",
		"R",
		"T",
		"Y",
		"U",
		"I",
		"O",
		"P",
		"LBRACKET",
		"RBRACKET",
		"RETURN",
		"LCONTROL",
		"A",
		"S",
		"D",
		"F",
		"G",
		"H",
		"J",
		"K",
		"L",
		"SEMICOLON",
		"APOSTROPHE",
		"GRAVE",
		"LSHIFT",
		"BACKSLASH",
		"Z",
		"X",
		"C",
		"V",
		"B",
		"N",
		"M",
		"COMMA",
		"PERIOD",
		"SLASH",
		"RSHIFT",
		"MULTIPLY",
		"LMENU",
		"SPACE",
		"CAPITAL",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"NUMLOCK",
		"SCROLL",
		"NUMPAD7",
		"NUMPAD8",
		"NUMPAD9",
		"SUBTRACT",
		"NUMPAD4",
		"NUMPAD5",
		"NUMPAD6",
		"ADD",
		"NUMPAD1",
		"NUMPAD2",
		"NUMPAD3",
		"NUMPAD0",
		"DECIMAL",
    "","",
		"OEM_102",
		"F11",
		"F12",
    "","","","","","","","","","","",
		"F13",
		"F14",
		"F15",
    "","","","","","","","","","",
		"KANA",
    "","",
		"ABNT_C1",
    "","","","","",
		"CONVERT",
    "",
		"NOCONVERT",
    "",
		"YEN",
		"ABNT_C2",
    "","","","","","","","","","","","","","",
		"NUMPADEQUALS",
    "","",
		"PREVTRACK",
		"AT",
		"COLON",
		"UNDERLINE",
		"KANJI",
		"STOP",
		"AX",
		"UNLABELED",
		"NEXTTRACK",
    "","",
		"NUMPADENTER",
		"RCONTROL",
    "","",
		"MUTE",
		"CALCULATOR",
		"PLAYPAUSE",
    "",
		"MEDIASTOP",
    "","","","","","","","","",
		"VOLUMEDOWN",
    "",
		"VOLUMEUP",
    "",
		"WEBHOME",
		"NUMPADCOMMA",
    "",
		"DIVIDE",
    "",
		"SYSRQ",
		"RMENU",
    "","","","","","","","","","","","",
		"PAUSE",
    "",
		"HOME",
		"UP",
		"PGUP",
    "",
		"LEFT",
    "",
		"RIGHT",
    "",
		"END",
		"DOWN",
		"PGDOWN",
		"INSERT",
		"DELETE",
    "","","","","","","",
		"LWIN",
		"RWIN",
		"APPS",
		"POWER",
		"SLEEP",
    "","","",
		"WAKE",
    "",
		"WEBSEARCH",
		"WEBFAVORITES",
		"WEBREFRESH",
		"WEBSTOP",
		"WEBFORWARD",
		"WEBBACK",
		"MYCOMPUTER",
		"MAIL",
		"MEDIASELECT"
    };
    for (int i = 0; i < numberOfKeys_s; i++)
      keyNames_[i] = keyNames[i];

    std::string mouseButtonNames[] = {
    "MouseLeft", "MouseRight", "MouseMiddle",
    "MouseButton3", "MouseButton4", "MouseButton5",
    "MouseButton6", "MouseButton7" };
    for (int i = 0; i < numberOfMouseButtons_s; i++)
      mouseButtonNames_[i] = mouseButtonNames[i];

    for (int i = 0; i < numberOfJoyStickButtons_s; i++)
      joyStickButtonNames_[i] = "JoyStick" + getConvertedValue<int, std::string>(i);
  }

  /**
    @brief Destructor
  */
  KeyBinder::~KeyBinder()
  {
  }

  /**
    @brief Loader for the key bindings, managed by config values.
  */
  void KeyBinder::setConfigValues()
  {
    ConfigValueContainer* cont;
    std::string modes[] = {"P_", "R_", "H_"};

    // keys
    for (int i = 0; i < numberOfKeys_s; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        cont = getIdentifier()->getConfigValueContainer(modes[j] + keyNames_[i]);
        if (!cont)
        {
          cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), modes[j] + keyNames_[i], "");
          getIdentifier()->addConfigValueContainer(modes[j] + keyNames_[i], cont);
        }
        switch (j)
        {
          // note: the first element in the struct is the string, so the following pointer
          //       arithmetic works.
          case 0:
            cont->getValue(&bindingsKeyPress_[i].commandStr);
            break;
          case 1:
            cont->getValue(bindingsKeyRelease_ + i);
            break;
          case 2:
            cont->getValue(bindingsKeyHold_ + i);
        }
      }
    }

    // mouse buttons
    for (int i = 0; i < numberOfMouseButtons_s; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        cont = getIdentifier()->getConfigValueContainer(modes[j] + mouseButtonNames_[i]);
        if (!cont)
        {
          cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), modes[j] + mouseButtonNames_[i], "");
          getIdentifier()->addConfigValueContainer(modes[j] + mouseButtonNames_[i], cont);
        }
        switch (j)
        {
          case 0:
            cont->getValue(bindingsMouseButtonPress_ + i);
            break;
          case 1:
            cont->getValue(bindingsMouseButtonRelease_ + i);
            break;
          case 2:
            cont->getValue(bindingsMouseButtonHold_ + i);
        }
      }
    }

    // joy stick buttons
    for (int i = 0; i < numberOfJoyStickButtons_s; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        cont = getIdentifier()->getConfigValueContainer(modes[j] + joyStickButtonNames_[i]);
        if (!cont)
        {
          cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), modes[j] + joyStickButtonNames_[i], "");
          getIdentifier()->addConfigValueContainer(modes[j] + joyStickButtonNames_[i], cont);
        }
        switch (j)
        {
          case 0:
            cont->getValue(bindingsJoyStickButtonPress_ + i);
            break;
          case 1:
            cont->getValue(bindingsJoyStickButtonRelease_ + i);
            break;
          case 2:
            cont->getValue(bindingsJoyStickButtonHold_ + i);
        }
      }
    }
  }

  /**
    @brief Overwrites all bindings with ""
  */
  void KeyBinder::clearBindings()
  {
    for (int i = 0; i < numberOfKeys_s; i++)
    {
      bindingsKeyPress_  [i].commandStr = "";
      bindingsKeyRelease_[i].commandStr = "";
      bindingsKeyHold_   [i].commandStr = "";
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

    ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, "keybindings.ini");
    setConfigValues();

    // evaluate the key bindings
    // TODO: what if binding is invalid?
    //for (int i = 0; i < numberOfKeys_s; i++)
    //{
    //  if (bindingsKeyPress_[i].commandStr != "")
    //  {
    //    bindingsKeyPress_[i].evaluation = CommandExecutor::evaluate(bindingsKeyPress_[i].commandStr);
    //    bindingsKeyPress_[i].commandStr = bindingsKeyPress_[i].evaluation.getCommandString();
    //  }
    //}

    COUT(ORX_DEBUG) << "KeyBinder: Loading key bindings done." << std::endl;
    return true;
  }

  bool KeyBinder::executeBinding(KeyBinding& binding)
  {
    if (binding.commandStr != "")
    {
      //if (binding.commandStr != binding.evaluation.getCommandString())
      //{
      //  // key binding has changed, reevaluate the command string.
      //  binding.evaluation = CommandExecutor::evaluate(binding.commandStr);
      //  binding.commandStr = binding.evaluation.getCommandString();
      //}
      COUT(3) << "Executing command: " << binding.commandStr << std::endl;
      CommandExecutor::execute(binding.commandStr);
    }

    return true;
  }


  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool KeyBinder::keyPressed(const OIS::KeyEvent &e)
  {
    COUT(3) << "Key: " << e.key << std::endl;
    // find the appropriate key binding
    executeBinding(bindingsKeyPress_[int(e.key)]);
     
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool KeyBinder::keyReleased(const OIS::KeyEvent &e)
  {
    // find the appropriate key binding
    executeBinding(bindingsKeyRelease_[int(e.key)]);

    return true;
  }

  /**
    @brief Event handler for the keyHeld Event.
    @param e Event information
  */
  bool KeyBinder::keyHeld(const OIS::KeyEvent &e)
  {
    // find the appropriate key binding
    executeBinding(bindingsKeyHold_[int(e.key)]);

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

  bool KeyBinder::buttonPressed(int joyStickID, const OIS::JoyStickEvent &arg, int button)
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

  bool KeyBinder::buttonReleased(int joyStickID, const OIS::JoyStickEvent &arg, int button)
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

  bool KeyBinder::buttonHeld(int joyStickID, const OIS::JoyStickEvent &arg, int button)
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

  bool KeyBinder::axisMoved(int joyStickID, const OIS::JoyStickEvent &arg, int axis)
  {
    return true;
  }

  bool KeyBinder::sliderMoved(int joyStickID, const OIS::JoyStickEvent &arg, int id)
  {
    return true;
  }

  bool KeyBinder::povMoved(int joyStickID, const OIS::JoyStickEvent &arg, int id)
  {
    return true;
  }

  bool KeyBinder::vector3Moved(int joyStickID, const OIS::JoyStickEvent &arg, int id)
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
