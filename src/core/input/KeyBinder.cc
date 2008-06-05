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

#include "KeyBinder.h"
#include <fstream>
#include <string>
#include "util/Convert.h"
#include "core/Debug.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "InputCommands.h"

namespace orxonox
{
  /**
    @brief Constructor that does as little as necessary.
  */
  KeyBinder::KeyBinder() : deriveTime_(0.0f)
  {
    mouseRelative_[0] = 0;
    mouseRelative_[1] = 0;
    mousePosition_[0] = 0;
    mousePosition_[1] = 0;

    RegisterRootObject(KeyBinder);

    // keys
    std::string keyNames[] = {
      "UNASSIGNED",
      "ESCAPE",
      "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
      "MINUS", "EQUALS", "BACK", "TAB",
      "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
      "LBRACKET", "RBRACKET",
      "RETURN", "LCONTROL",
      "A", "S", "D", "F", "G", "H", "J", "K", "L",
      "SEMICOLON", "APOSTROPHE", "GRAVE",
      "LSHIFT", "BACKSLASH",
      "Z", "X", "C", "V", "B", "N", "M",
      "COMMA", "PERIOD", "SLASH",
      "RSHIFT",
      "MULTIPLY",
      "LMENU",
      "SPACE",
      "CAPITAL",
      "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
      "NUMLOCK", "SCROLL",
      "NUMPAD7", "NUMPAD8", "NUMPAD9",
      "SUBTRACT",
      "NUMPAD4", "NUMPAD5", "NUMPAD6",
      "ADD",
      "NUMPAD1", "NUMPAD2", "NUMPAD3", "NUMPAD0",
      "DECIMAL",
      "","",
      "OEM_102",
      "F11", "F12",
      "","","","","","","","","","","",
      "F13", "F14", "F15",
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
      "COLON", "UNDERLINE",
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
      "END", "DOWN", "PGDOWN", "INSERT", "DELETE",
      "","","","","","","",
      "LWIN", "RWIN", "APPS",
      "POWER", "SLEEP",
      "","","",
      "WAKE",
      "",
      "WEBSEARCH", "WEBFAVORITES", "WEBREFRESH", "WEBSTOP", "WEBFORWARD", "WEBBACK",
      "MYCOMPUTER", "MAIL", "MEDIASELECT"
    };
    for (unsigned int i = 0; i < nKeys_s; i++)
      keys_[i].name_ = "Key" + keyNames[i];

    // mouse buttons
    std::string mouseButtonNames[] = {
      "MouseLeft", "MouseRight", "MouseMiddle",
      "MouseButton3", "MouseButton4", "MouseButton5",
      "MouseButton6", "MouseButton7",
      "MouseWheel1Up", "MouseWheel1Down",
      "MouseWheel2Up", "MouseWheel2Down" };
    for (unsigned int i = 0; i < nMouseButtons_s; i++)
      mouseButtons_[i].name_ = mouseButtonNames[i];

    // joy stick buttons
    for (unsigned int i = 0; i < 32; i++)
      joyStickButtons_[i].name_ = "JoyButton" + getConvertedValue<int, std::string>(i);
    for (unsigned int i = 32; i < nJoyStickButtons_s; i += 4)
    {
		  joyStickButtons_[i + 0].name_ = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "North";
		  joyStickButtons_[i + 1].name_ = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "South";
		  joyStickButtons_[i + 2].name_ = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "East";
		  joyStickButtons_[i + 3].name_ = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "West";
    }

    // half axes
    std::string rawNames[nHalfAxes_s/2];
    rawNames[0] = "MouseX";
    rawNames[1] = "MouseY";
    rawNames[2] = "Empty1";
    rawNames[3] = "Empty2";
    for (unsigned int i = 4; i < nHalfAxes_s/2; i++)
      rawNames[i] = "JoyAxis" + getConvertedValue<int, std::string>(i - 3);
    for (unsigned int i = 0; i < nHalfAxes_s/2; i++)
    {
      halfAxes_[i * 2 + 0].name_ = rawNames[i] + "Pos";
      halfAxes_[i * 2 + 1].name_ = rawNames[i] + "Neg";
    }

    for (unsigned int i = 0; i < this->nHalfAxes_s; i++)
      halfAxes_[i].buttonThreshold_ = buttonThreshold_;
  }

  /**
    @brief Destructor
  */
  KeyBinder::~KeyBinder()
  {
    // almost no destructors required because most of the arrays are static.
    clearBindings(); // does some destruction work
  }

  /**
    @brief Loads the key and button bindings.
    @return True if loading succeeded.
  */
  void KeyBinder::loadBindings()
  {
    COUT(3) << "KeyBinder: Loading key bindings..." << std::endl;

    clearBindings();

    std::ifstream infile;
    infile.open("keybindings.ini");
    if (!infile)
    {
      ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, "def_keybindings.ini");
      ConfigFileManager::getSingleton()->save(CFT_Keybindings, "keybindings.ini");
    }
    else
      infile.close();
    ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, "keybindings.ini");

    // parse key bindings
    setConfigValues();

    COUT(3) << "KeyBinder: Loading key bindings done." << std::endl;
  }

  /**
    @brief Loader for the key bindings, managed by config values.
  */
  void KeyBinder::setConfigValues()
  {
    SetConfigValueGeneric(KeyBinder, analogThreshold_, 0.01f)  .description("Threshold for analog axes until which the state is 0.");
    SetConfigValueGeneric(KeyBinder, mouseSensitivity_, 1.0f)  .description("Mouse sensitivity.");
    SetConfigValueGeneric(KeyBinder, bDeriveMouseInput_, false).description("Whether or not to derive moues movement for the absolute value.");
    SetConfigValueGeneric(KeyBinder, derivePeriod_, 0.5f).description("Accuracy of the mouse input deriver. The higher the more precise, but laggier.");
    SetConfigValueGeneric(KeyBinder, mouseSensitivityDerived_, 1.0f).description("Mouse sensitivity if mouse input is derived.");
    SetConfigValueGeneric(KeyBinder, bClipMouse_, true).description("Whether or not to clip absolute value of mouse in non derive mode.");

    float oldThresh = buttonThreshold_;
    SetConfigValueGeneric(KeyBinder, buttonThreshold_, 0.80f).description("Threshold for analog axes until which the button is not pressed.");
    if (oldThresh != buttonThreshold_)
      for (unsigned int i = 0; i < nHalfAxes_s; i++)
        if (halfAxes_[i].buttonThreshold_ == oldThresh)
          halfAxes_[i].buttonThreshold_ = buttonThreshold_;

    // keys
    for (unsigned int i = 0; i < nKeys_s; i++)
      readTrigger(keys_[i]);
    // mouse buttons
    for (unsigned int i = 0; i < nMouseButtons_s; i++)
      readTrigger(mouseButtons_[i]);
    // joy stick buttons
    for (unsigned int i = 0; i < nJoyStickButtons_s; i++)
      readTrigger(joyStickButtons_[i]);
    // half axes
    for (unsigned int i = 0; i < nHalfAxes_s; i++)
      readTrigger(halfAxes_[i]);
  }

  void KeyBinder::readTrigger(Button& button)
  {
    // config value stuff
    ConfigValueContainer* cont = ClassIdentifier<KeyBinder>::getIdentifier()->getConfigValueContainer(button.name_);
    if (!cont)
    {
      cont = new ConfigValueContainer(CFT_Keybindings, ClassIdentifier<KeyBinder>::getIdentifier(), button.name_, "");
      ClassIdentifier<KeyBinder>::getIdentifier()->addConfigValueContainer(button.name_, cont);
    }
    std::string old = button.bindingString_;
    cont->getValue(&button.bindingString_);

    // keybinder stuff
    if (old != button.bindingString_)
    {
      // clear everything so we don't get old axis ParamCommands mixed up
      button.clear();

      // binding has changed
      button.parse(paramCommandBuffer_);
    }
  }

  /**
    @brief Overwrites all bindings with ""
  */
  void KeyBinder::clearBindings()
  {
    for (unsigned int i = 0; i < nKeys_s; i++)
      keys_[i].clear();

    for (unsigned int i = 0; i < nMouseButtons_s; i++)
      mouseButtons_[i].clear();

    for (unsigned int i = 0; i < nJoyStickButtons_s; i++)
      joyStickButtons_[i].clear();

    for (unsigned int i = 0; i < nHalfAxes_s; i++)
      halfAxes_[i].clear();

    for (unsigned int i = 0; i < paramCommandBuffer_.size(); i++)
      delete paramCommandBuffer_[i];
    paramCommandBuffer_.clear();
  }

  void KeyBinder::resetJoyStickAxes()
  {
    for (unsigned int i = 8; i < nHalfAxes_s; i++)
    {
      halfAxes_[i].absVal_ = 0.0f;
      halfAxes_[i].relVal_ = 0.0f;
    }
  }

  void KeyBinder::tickInput(float dt, const HandlerState& state)
  {
    // we have to process all the analog input since there is e.g. no 'mouseDoesntMove' event.
    unsigned int iBegin = 8;
    unsigned int iEnd   = 8;
    if (state.joyStick)
      iEnd = nHalfAxes_s;
    if (state.mouse)
      iBegin = 0;
    for (unsigned int i = iBegin; i < iEnd; i++)
    {
      if (halfAxes_[i].hasChanged_)
      {
        if (!halfAxes_[i].wasDown_ && halfAxes_[i].absVal_ > halfAxes_[i].buttonThreshold_)
        {
          halfAxes_[i].wasDown_ = true;
          if (halfAxes_[i].nCommands_[KeybindMode::OnPress])
            halfAxes_[i].execute(KeybindMode::OnPress);
        }
        else if (halfAxes_[i].wasDown_ && halfAxes_[i].absVal_ < halfAxes_[i].buttonThreshold_)
        {
          halfAxes_[i].wasDown_ = false;
          if (halfAxes_[i].nCommands_[KeybindMode::OnRelease])
            halfAxes_[i].execute(KeybindMode::OnRelease);
        }
        halfAxes_[i].hasChanged_ = false;
      }

      if (halfAxes_[i].wasDown_)
      {
        if (halfAxes_[i].nCommands_[KeybindMode::OnHold])
          halfAxes_[i].execute(KeybindMode::OnHold);
      }

      // these are the actually useful axis bindings for analog input AND output
      if (halfAxes_[i].relVal_ > analogThreshold_ || halfAxes_[i].absVal_ > analogThreshold_)
      {
        //COUT(3) << halfAxes_[i].name_ << "\t" << halfAxes_[i].absVal_ << std::endl;
        halfAxes_[i].execute();
      }
    }

    if (bDeriveMouseInput_ && state.mouse)
    {
      if (deriveTime_ > derivePeriod_)
      {
        //CCOUT(3) << "mouse abs: ";
        for (int i = 0; i < 2; i++)
        {
          if (mouseRelative_[i] > 0)
          {
            halfAxes_[2*i + 0].absVal_ =  mouseRelative_[i] / deriveTime_ * 0.0005 * mouseSensitivityDerived_;
            halfAxes_[2*i + 1].absVal_ = 0.0f;
          }
          else if (mouseRelative_[i] < 0)
          {
            halfAxes_[2*i + 0].absVal_ = 0.0f;
            halfAxes_[2*i + 1].absVal_ = -mouseRelative_[i] / deriveTime_ * 0.0005 * mouseSensitivityDerived_;
          }
          else
          {
            halfAxes_[2*i + 0].absVal_ = 0.0f;
            halfAxes_[2*i + 1].absVal_ = 0.0f;
          }
          //COUT(3) << mouseRelative_[i] << " | ";
          mouseRelative_[i] = 0;
          halfAxes_[2*i + 0].hasChanged_ = true;
          halfAxes_[2*i + 1].hasChanged_ = true;
        }
        deriveTime_ = 0.0f;
        //COUT(3) << std::endl;
      }
      else
        deriveTime_ += dt;
    }

    // execute all buffered bindings (addional parameter)
    for (unsigned int i = 0; i < paramCommandBuffer_.size(); i++)
      paramCommandBuffer_[i]->execute();

    // always reset the relative movement of the mouse
    if (state.mouse)
      for (unsigned int i = 0; i < 8; i++)
        halfAxes_[i].relVal_ = 0.0f;
  }

  void KeyBinder::keyPressed (const KeyEvent& evt)
  { keys_[evt.key].execute(KeybindMode::OnPress); }

  void KeyBinder::keyReleased(const KeyEvent& evt)
  { keys_[evt.key].execute(KeybindMode::OnRelease); }

  void KeyBinder::keyHeld    (const KeyEvent& evt)
  { keys_[evt.key].execute(KeybindMode::OnHold); }


  void KeyBinder::mouseButtonPressed (MouseButton::Enum id)
  { mouseButtons_[id].execute(KeybindMode::OnPress); }

  void KeyBinder::mouseButtonReleased(MouseButton::Enum id)
  { mouseButtons_[id].execute(KeybindMode::OnRelease); }

  void KeyBinder::mouseButtonHeld    (MouseButton::Enum id)
  { mouseButtons_[id].execute(KeybindMode::OnHold); }


  void KeyBinder::joyStickButtonPressed (int joyStickID, int button)
  { joyStickButtons_[button].execute(KeybindMode::OnPress); }

  void KeyBinder::joyStickButtonReleased(int joyStickID, int button)
  { joyStickButtons_[button].execute(KeybindMode::OnRelease); }

  void KeyBinder::joyStickButtonHeld    (int joyStickID, int button)
  { joyStickButtons_[button].execute(KeybindMode::OnHold); }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Mouse state information
  */
  void KeyBinder::mouseMoved(IntVector2 abs_, IntVector2 rel_, IntVector2 clippingSize)
  {
    // y axis of mouse input is inverted
    int rel[] = { rel_.x, -rel_.y };

    if (!bDeriveMouseInput_)
    {
      for (int i = 0; i < 2; i++)
      {
        if (rel[i])
        {
          // absolute
          halfAxes_[2*i + 0].hasChanged_ = true;
          halfAxes_[2*i + 1].hasChanged_ = true;
          mousePosition_[i] += rel[i];

          if (bClipMouse_)
          {
            if (mousePosition_[i] > 1024)
              mousePosition_[i] =  1024;
            if (mousePosition_[i] < -1024)
              mousePosition_[i] = -1024;
          }

          if (mousePosition_[i] >= 0)
          {
            halfAxes_[2*i + 0].absVal_ =   mousePosition_[i]/1024.0f * mouseSensitivity_;
            halfAxes_[2*i + 1].absVal_ =  0.0f;
          }
          else
          {
            halfAxes_[2*i + 0].absVal_ =  0.0f;
            halfAxes_[2*i + 1].absVal_ =  -mousePosition_[i]/1024.0f * mouseSensitivity_;
          }
        }
      }
    }
    else
    {
      mouseRelative_[0] += rel[0];
      mouseRelative_[1] += rel[1];
    }

    // relative
    for (int i = 0; i < 2; i++)
    {
      if (rel[i] > 0)
        halfAxes_[0 + 2*i].relVal_ =  ((float)rel[i])/1024 * mouseSensitivity_;
      else
        halfAxes_[1 + 2*i].relVal_ = -((float)rel[i])/1024 * mouseSensitivity_;
    }
  }

  /**
    @brief Event handler for the mouseScrolled Event.
    @param e Mouse state information
  */
  void KeyBinder::mouseScrolled(int abs, int rel)
  {
    //COUT(3) << mouseButtons_[8].name_ << "   " << abs << " | " << rel << std::endl;

    if (rel > 0)
      for (int i = 0; i < rel/120; i++)
        mouseButtons_[8].execute(KeybindMode::OnPress, ((float)abs)/120.0f);
    else
      for (int i = 0; i < -rel/120; i++)
        mouseButtons_[9].execute(KeybindMode::OnPress, ((float)abs)/120.0f);
  }

  void KeyBinder::joyStickAxisMoved(int joyStickID, int axis, float value)
  {
    // TODO: Use proper calibration values instead of generally 16-bit integer
    int i = 8 + axis * 2;
    if (value >= 0)
    {
      //if (value > 10000)
      //{ CCOUT(3) << halfAxes_[i].name_ << std::endl; }

      halfAxes_[i].absVal_ = value;
      halfAxes_[i].relVal_ = value;
      halfAxes_[i].hasChanged_ = true;
      if (halfAxes_[i + 1].absVal_ > 0.0f)
      {
        halfAxes_[i + 1].absVal_ = -0.0f;
        halfAxes_[i + 1].relVal_ = -0.0f;
        halfAxes_[i + 1].hasChanged_ = true;
      }
    }
    else
    {
      //if (value < -10000)
      //{ CCOUT(3) << halfAxes_[i + 1].name_ << std::endl; }

      halfAxes_[i + 1].absVal_ = -value;
      halfAxes_[i + 1].relVal_ = -value;
      halfAxes_[i + 1].hasChanged_ = true;
      if (halfAxes_[i].absVal_ > 0.0f)
      {
        halfAxes_[i].absVal_ = -0.0f;
        halfAxes_[i].relVal_ = -0.0f;
        halfAxes_[i].hasChanged_ = true;
      }
    }
  }
}
