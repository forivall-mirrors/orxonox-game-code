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
#include "util/Convert.h"
#include "util/SubString.h"
#include "util/String.h"
#include "Debug.h"
#include "ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "CommandExecutor.h"
#include "Executor.h"

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
    clearBindings(true);

    // keys
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
    for (int i = 0; i < nKeys_s; i++)
      namesKeys_[i] = "Key" + keyNames[i];

    // mouse buttons
    std::string mouseButtonNames[] = {
      "MouseLeft", "MouseRight", "MouseMiddle",
      "MouseButton3", "MouseButton4", "MouseButton5",
      "MouseButton6", "MouseButton7" };
    for (int i = 0; i < nMouseButtons_s; i++)
      namesMouseButtons_[i] = mouseButtonNames[i];

    // joy stick buttons
    for (int i = 0; i < 32; i++)
      namesJoyStickButtons_[i] = "JoyButton" + getConvertedValue<int, std::string>(i);
    for (int i = 32; i < nJoyStickButtons_s; i += 4)
    {
		  namesJoyStickButtons_[i + 0] = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "North";
		  namesJoyStickButtons_[i + 1] = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "South";
		  namesJoyStickButtons_[i + 2] = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "East";
		  namesJoyStickButtons_[i + 3] = "JoyPOV" + getConvertedValue<int, std::string>((i - 32)/4 + 1) + "West";
    }

    // half axes
    std::string rawNames[nHalfAxes_s/2];
    rawNames[0] = "MouseX";
    rawNames[1] = "MouseY";
    rawNames[2] = "MouseWheel1";
    rawNames[3] = "MouseWheel2";
    for (unsigned int i = 4; i < nHalfAxes_s/2; i++)
      rawNames[i] = "JoyAxis" + getConvertedValue<int, std::string>(i - 3);
    for (unsigned int i = 0; i < nHalfAxes_s/2; i++)
    {
      namesHalfAxes_[i * 2 + 0] = rawNames[i] + "Pos";
      namesHalfAxes_[i * 2 + 1] = rawNames[i] + "Neg";
    }
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
    @brief Loader for the key bindings, managed by config values.
  */
  void KeyBinder::setConfigValues()
  {
    bool success = true;
    // keys
    success |= readBindings(namesKeys_, bindingStringsKeys_, bindingsKeys_, nKeys_s);
    // mouse buttons
    success |= readBindings(namesMouseButtons_, bindingStringsMouseButtons_, bindingsMouseButtons_, nMouseButtons_s);
    // joy stick buttons
    success |= readBindings(namesJoyStickButtons_, bindingStringsJoyStickButtons_,
        bindingsJoyStickButtons_, nJoyStickButtons_s);
    // half axes
    success |= readBindings(namesHalfAxes_, bindingStringsHalfAxes_, bindingsHalfAxes_, nHalfAxes_s);
    
    // TODO: what happens if parsing didn't succeed in all parts? nothing?
  }

  bool KeyBinder::readBindings(std::string* names, std::string* bindingStrings,
      KeyBindingBundle* bindings, unsigned int size)
  {
    for (unsigned int i = 0; i < size; i++)
    {
      // config value stuff
      ConfigValueContainer* cont = getIdentifier()->getConfigValueContainer(names[i]);
      if (!cont)
      {
        cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), names[i], "");
        getIdentifier()->addConfigValueContainer(names[i], cont);
      }
      std::string old = bindingStrings[i];
      cont->getValue(&bindingStrings[i]);

      // keybinder stuff
      if (old != bindingStrings[i])
      {
        // binding has changed
        if (bindingStrings[i] == "")
        {
          // empty binding, occurs at least the first time since init value is " "
          bindings[i].OnPress.clear();
          bindings[i].OnRelease.clear();
          bindings[i].OnHold.clear();
        }
        else
        {
          // actually parse the command(s)
          SubString commands(bindingStrings[i], "|", SubString::WhiteSpaces, false,
              '\\', false, '"', false, '(', ')', false, '\0');
          bindings[i].OnHold.nCommands = 0;
          bindings[i].OnHold.commands = new SimpleCommand[64];
          bindings[i].OnPress.nCommands = 0;
          bindings[i].OnPress.commands = new SimpleCommand[64];
          bindings[i].OnRelease.nCommands = 0;
          bindings[i].OnRelease.commands = new SimpleCommand[64];
          for (unsigned int iCommand = 0; iCommand < commands.size(); iCommand++)
          {
            if (commands[iCommand] != "")
            {
              SubString tokens(commands[iCommand], " ", SubString::WhiteSpaces, false,
                  '\\', false, '"', false, '(', ')', false, '\0');
              
              unsigned int iToken = 0;

              // first argument can be OnPress, OnHold OnRelease or nothing
              KeybindMode::Enum mode = KeybindMode::None;
              if (getLowercase(tokens[iToken]) == "onpress")
                mode = KeybindMode::OnPress,   iToken++;
              if (getLowercase(tokens[iToken]) == "onrelease")
                mode = KeybindMode::OnRelease, iToken++;
              if (getLowercase(tokens[iToken]) == "onhold")
                mode = KeybindMode::OnHold,    iToken++;

              if (iToken == tokens.size())
                continue;

              SimpleCommand* cmd = new SimpleCommand();

              // second argument can be the amplitude for the case it as an axis command
              // default amplitude is 1.0f
              if (getLowercase(tokens[iToken]) == "axisamp")
              {
                iToken++;
                float value;
                if (iToken == tokens.size() || !convertValue(&value, tokens[iToken]))
                {
                  CCOUT(2) << "Error while parsing key binding " << names[i]
                      << ". Numeric expression expected afer 'AxisAmp', switching to default value" << std::endl;
                  if (iToken == tokens.size())
                  {
                    delete cmd;
                    continue;
                  }
                  cmd->axisModifier = 1.0f;
                }
                else
                  cmd->axisModifier = value;
                iToken++;
              }
              else
                cmd->axisModifier = 1.0f;

              // no more arguments expected except for the actual command
              if (iToken == tokens.size())
              { // no command given
                delete cmd;
                continue;
              }
              while (iToken != tokens.size())
                cmd->commandStr += tokens[iToken++] + " ";

              // check whether we exceed 64 commands...
              if (bindings[i].OnHold.nCommands == 64 || bindings[i].OnPress.nCommands == 64
                  || bindings[i].OnRelease.nCommands == 64)
              {
                CCOUT(2) << "Error while parsing key binding " << names[i]
                    << ". You shouldn't assign more than 64 key bindings to one key "
                    << "just to test the parser" << std::endl;
              }

              // evaluate the command
              cmd->axisCommand = 0;
              CommandEvaluation& eval = CommandExecutor::evaluate(cmd->commandStr);
              // TOOD: check for axis command
              if (false)
              {
                cmd->axisCommand->commandStr = cmd->commandStr;
                cmd->commandStr = "";
                cmd->axisCommand->evaluation = eval;
                // add command to the buffer if not yet existing
                for (unsigned int iAxisCmd = 0; iAxisCmd < axisCommands_.size(); iAxisCmd++)
                {
                  if (getLowercase(axisCommands_[iAxisCmd]->commandStr) == getLowercase(cmd->commandStr))
                  {
                    // already in list
                    cmd->axisCommand = axisCommands_[iAxisCmd];
                    break;
                  }
                }
                if (cmd->axisCommand == 0)
                {
                  cmd->axisCommand = new AxisCommand();
                  axisCommands_.push_back(cmd->axisCommand);
                }
                // TODO: check for relative/absolute command
                cmd->axisCommand->bRelative = false;

                // axis commands are always OnHold
                *(bindings[i].OnHold.commands + bindings[i].OnHold.nCommands++) = *cmd;
              }
              else
              {
                cmd->evaluation = eval;

                // TODO: determine whether the command is OnHold, OnPress or OnRelease
                switch (mode)
                {
                case KeybindMode::None:
                  *(bindings[i].OnPress.commands + bindings[i].OnPress.nCommands++) = *cmd;
                  break;
                case KeybindMode::OnPress:
                  *(bindings[i].OnPress.commands + bindings[i].OnPress.nCommands++) = *cmd;
                  break;
                case KeybindMode::OnHold:
                  *(bindings[i].OnHold.commands + bindings[i].OnHold.nCommands++) = *cmd;
                  break;
                case KeybindMode::OnRelease:
                  *(bindings[i].OnRelease.commands + bindings[i].OnRelease.nCommands++) = *cmd;
                  break;                      
                }
              }
            }
          }

          // redimension arrays with simple commands
          SimpleCommand* sCmd = bindings[i].OnHold.commands;
          if (bindings[i].OnHold.nCommands)
          {
            bindings[i].OnHold.commands = new SimpleCommand[bindings[i].OnHold.nCommands];
            for (unsigned int iCmd = 0; iCmd < bindings[i].OnHold.nCommands; iCmd++)
              bindings[i].OnHold.commands[iCmd] = sCmd[iCmd];
          }
          else
            bindings[i].OnHold.commands = 0;
          delete[] sCmd;

          sCmd = bindings[i].OnPress.commands;
          if (bindings[i].OnPress.nCommands)
          {
            bindings[i].OnPress.commands = new SimpleCommand[bindings[i].OnPress.nCommands];
            for (unsigned int iCmd = 0; iCmd < bindings[i].OnPress.nCommands; iCmd++)
              bindings[i].OnPress.commands[iCmd] = sCmd[iCmd];
          }
          else
            bindings[i].OnPress.commands = 0;
          delete[] sCmd;

          sCmd = bindings[i].OnRelease.commands;
          if (bindings[i].OnRelease.nCommands)
          {
            bindings[i].OnRelease.commands = new SimpleCommand[bindings[i].OnRelease.nCommands];
            for (unsigned int iCmd = 0; iCmd < bindings[i].OnRelease.nCommands; iCmd++)
              bindings[i].OnRelease.commands[iCmd] = sCmd[iCmd];
          }
          else
            bindings[i].OnRelease.commands = 0;
          delete[] sCmd;
        }
      }
    }
    return true;
  }

  /**
    @brief Overwrites all bindings with ""
  */
  void KeyBinder::clearBindings(bool bInit)
  {
    for (int i = 0; i < nKeys_s; i++)
    {
      clearBundle(bindingsKeys_[i], bInit);
      bindingStringsKeys_[i] = " ";
    }
    for (int i = 0; i < nMouseButtons_s; i++)
    {
      clearBundle(bindingsMouseButtons_[i], bInit);
      bindingStringsMouseButtons_[i] = " ";
    }
    for (int i = 0; i < nJoyStickButtons_s; i++)
    {
      clearBundle(bindingsJoyStickButtons_[i], bInit);
      bindingStringsJoyStickButtons_[i] = " ";
    }
    for (int i = 0; i < nHalfAxes_s; i++)
    {
      clearBundle(bindingsHalfAxes_[i], bInit);
      bindingStringsHalfAxes_[i] = " ";
    }
    for (unsigned int i = 0; i < axisCommands_.size(); i++)
      delete axisCommands_[i];
    axisCommands_.clear();
  }

  void KeyBinder::clearBundle(KeyBindingBundle& bundle, bool bInit)
  {
    if (!bInit)
    {
      if (bundle.OnHold.nCommands)
        delete[] bundle.OnHold.commands;
      if (bundle.OnPress.nCommands)
        delete[] bundle.OnPress.commands;
      if (bundle.OnRelease.nCommands)
        delete[] bundle.OnRelease.commands;
    }
    bundle.OnPress.nCommands = 0;
    bundle.OnHold.nCommands = 0;
    bundle.OnRelease.nCommands = 0;
  }

  /**
    @brief Loads the key and button bindings.
    @return True if loading succeeded.
  */
  bool KeyBinder::loadBindings()
  {
    COUT(3) << "KeyBinder: Loading key bindings..." << std::endl;

    // clear half axes
    for (unsigned int i = 0; i < nHalfAxes_s; i++)
    {
      halfAxes_[i].hasChanged = false;
      halfAxes_[i].abs = 0.0f;
      halfAxes_[i].rel = 0.0f;
      halfAxes_[i].wasDown = false;
      halfAxes_[i].threshold = 0.01f;
    }

    ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, "keybindings.ini");
    clearBindings();
    setConfigValues();

    COUT(3) << "KeyBinder: Loading key bindings done." << std::endl;
    return true;
  }

  void KeyBinder::tick(float dt)
  {
    // we have to process all the analog input since there is e.g. no 'mouseDoesntMove' event.
    for (unsigned int i = 0; i < nHalfAxes_s; i++)
    {
      if (!halfAxes_[i].hasChanged)
      {
        if (!halfAxes_[i].wasDown && halfAxes_[i].abs > halfAxes_[i].threshold)
        {
          halfAxes_[i].wasDown = true;
          if (bindingsHalfAxes_[i].OnPress.nCommands)
            executeBinding(bindingsHalfAxes_[i].OnPress, halfAxes_[i].rel, halfAxes_[i].abs);
        }
        else if (halfAxes_[i].wasDown && halfAxes_[i].abs < halfAxes_[i].threshold)
        {
          halfAxes_[i].wasDown = false;
          if (bindingsHalfAxes_[i].OnRelease.nCommands)
            executeBinding(bindingsHalfAxes_[i].OnRelease, halfAxes_[i].rel, halfAxes_[i].abs);
        }
        if (halfAxes_[i].wasDown)
        {
          executeBinding(bindingsHalfAxes_[i].OnHold, halfAxes_[i].rel, halfAxes_[i].abs);
        }
        halfAxes_[i].hasChanged = false;
      }
    }

    // execute all buffered bindings (addional parameter)
    for (unsigned int i = 0; i < axisCommands_.size(); i++)
    {
      if (axisCommands_[i]->nValuesAdded > 0)
      {
        axisCommands_[i]->evaluation.setEvaluatedParameter(0, axisCommands_[i]->value);
        // reset
        axisCommands_[i]->nValuesAdded = 0;
        axisCommands_[i]->value = 0.0f;
      }
    }
  }

  bool KeyBinder::executeBinding(KeyBinding& binding, float axisRel, float axisAbs)
  {
    // execute all the parsed commands in the string
    for (unsigned int iCommand = 0; iCommand < binding.nCommands; iCommand++)
    {
      SimpleCommand& command = binding.commands[iCommand];
      if (command.axisCommand)
      {
        AxisCommand& axisCommand = *command.axisCommand;
        // command has an additional parameter
        if (command.axisCommand->bRelative)
        {
          // we have to calculate a relative movement.
          // amplitude says how much one keystroke is
          axisCommand.value += command.axisModifier * axisRel;
        }
        else
        {
          // we have to calculate absolute position of the axis.
          // for a key this simply is 1, but multiplied by a user defined factor
          // since there might be another axis that is affected, we have to wait and
          // store the result in a temporary place
          axisCommand.value =
              (axisCommand.value * (axisCommand.nValuesAdded++) + command.axisModifier * axisAbs)
              / axisCommand.nValuesAdded;
        }
      }
      else
      {
        // simple command, just execute directly
        // TODO: calculate whether this a Press, Release or Hold event
        CommandExecutor::execute(command.evaluation);
      }
    }
    return true;
  }


  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool KeyBinder::keyPressed(const KeyEvent& evt)
  {
    // find the appropriate key binding
    executeBinding(bindingsKeys_[int(evt.key)].OnPress, 1.0, 1.0);

    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool KeyBinder::keyReleased(const KeyEvent& evt)
  {
    // find the appropriate key binding
    executeBinding(bindingsKeys_[int(evt.key)].OnRelease, 1.0, 1.0);

    return true;
  }

  /**
    @brief Event handler for the keyHeld Event.
    @param e Mouse state information
  */
  bool KeyBinder::keyHeld(const KeyEvent& evt)
  {
    // find the appropriate key binding
    executeBinding(bindingsKeys_[int(evt.key)].OnHold, 1.0, 1.0);

    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Mouse state information
  */
  bool KeyBinder::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
  {
    halfAxes_[0].hasChanged = true;
    halfAxes_[1].hasChanged = true;
    halfAxes_[2].hasChanged = true;
    halfAxes_[3].hasChanged = true;
    // translate absolute mouse position into joystick like behaviour
    if (clippingSize.x > clippingSize.y)
    {
      int margin = (clippingSize.x - clippingSize.y) / 2;
      if (abs.x - margin > clippingSize.y)
      {
        halfAxes_[0].abs = 1.0f;
        halfAxes_[1].abs = 0.0f;
      }
      else if (abs.x < margin)
      {
        halfAxes_[0].abs = 0.0f;
        halfAxes_[1].abs = 1.0f;
      }
      else
      {
        float temp = ((float)abs.x) / clippingSize.y * 2 - 1;
        if (temp > 0)
        {
          halfAxes_[0].abs = temp;
          halfAxes_[1].abs = 0.0f;
        }
        else
        {
          halfAxes_[0].abs = 0.0f;
          halfAxes_[1].abs = -temp;
        }
      }

      float temp = -((float)abs.y) / clippingSize.y * 2 + 1;
      if (temp > 0)
      {
        halfAxes_[2].abs = temp;
        halfAxes_[3].abs = 0.0;
      }
      else
      {
        halfAxes_[2].abs = 0.0;
        halfAxes_[3].abs = -temp;
      }
    }
    else
    {
      float temp = ((float)abs.x) / clippingSize.x * 2 - 1;
      if (temp > 0)
      {
        halfAxes_[0].abs = temp;
        halfAxes_[1].abs = 0.0;
      }
      else
      {
        halfAxes_[0].abs = 0.0;
        halfAxes_[1].abs = -temp;
      }

      int margin = (clippingSize.y - clippingSize.x) / 2;
      if (abs.y - margin > clippingSize.x)
      {
        halfAxes_[2].abs = 0.0;
        halfAxes_[3].abs = 1.0;
      }
      else if (abs.y < margin)
      {
        halfAxes_[2].abs = 1.0;
        halfAxes_[3].abs = 0.0;
      }
      else
      {
        float temp = -((float)abs.y) / clippingSize.x * 2 + 1;
        if (temp > 0)
        {
          halfAxes_[2].abs = temp;
          halfAxes_[3].abs = 0.0;
        }
        else
        {
          halfAxes_[2].abs = 0.0;
          halfAxes_[3].abs = -temp;
        }
      }
    }
    
    // relative movements
    if (rel.x > 0)
    {
      halfAxes_[0].rel = rel.x;
      halfAxes_[1].rel = 0.0;
    }
    else
    {
      halfAxes_[0].rel = 0.0;
      halfAxes_[1].rel = rel.x;
    }

    if (rel.y /*!*/ < /*!*/ 0)
    {
      halfAxes_[0].rel = -rel.y;
      halfAxes_[1].rel = 0.0;
    }
    else
    {
      halfAxes_[0].rel = 0.0;
      halfAxes_[1].rel = -rel.y;
    }

    return true;
  }

  /**
    @brief Event handler for the mouseScrolled Event.
    @param e Mouse state information
  */
  bool KeyBinder::mouseScrolled(int abs, int rel)
  {
    // TODO: obvious...
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool KeyBinder::mouseButtonPressed(MouseButton::Enum id)
  {
    // find the appropriate key binding
    executeBinding(bindingsMouseButtons_[int(id)].OnPress, 1.0, 1.0);

    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool KeyBinder::mouseButtonReleased(MouseButton::Enum id)
  {
    // find the appropriate key binding
    executeBinding(bindingsMouseButtons_[int(id)].OnRelease, 1.0, 1.0);

    return true;
  }

  /**
    @brief Event handler for the mouseHeld Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool KeyBinder::mouseButtonHeld(MouseButton::Enum id)
  {
    // find the appropriate key binding
    executeBinding(bindingsMouseButtons_[int(id)].OnHold, 1.0, 1.0);

    return true;
  }

  bool KeyBinder::joyStickButtonPressed(int joyStickID, int button)
  {
    // find the appropriate key binding
    executeBinding(bindingsJoyStickButtons_[button].OnPress, 1.0, 1.0);

    return true;
  }

  bool KeyBinder::joyStickButtonReleased(int joyStickID, int button)
  {
    // find the appropriate key binding
    executeBinding(bindingsJoyStickButtons_[button].OnRelease, 1.0, 1.0);

    return true;
  }

  bool KeyBinder::joyStickButtonHeld(int joyStickID, int button)
  {
    // find the appropriate key binding
    executeBinding(bindingsJoyStickButtons_[button].OnHold, 1.0, 1.0);

    return true;
  }

  bool KeyBinder::joyStickAxisMoved(int joyStickID, int axis, int value)
  {
    // TODO: check whether 16 bit integer as general axis value is a good idea (works under windows)
    halfAxes_[8 + axis].hasChanged = true;
    if (value >= 0)
    {
      halfAxes_[8 + axis].abs = ((float)value)/0x1000;
      halfAxes_[8 + axis].hasChanged = true;
    }
    else
    {
      halfAxes_[8 + axis + 1].abs = -((float)value)/0x1000;
      halfAxes_[8 + axis + 1].hasChanged = true;
    }
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
  //bool GUIInputHandler::mousePressed(const OIS::MouseEvent &e, OIS::MouseButton id)
  //{
    ////CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));
  //  return true;
  //}

  ///**
  //  @brief Event handler for the mouseReleased Event.
  //  @param e Event information
  //  @param id The ID of the mouse button
  //*/
  //bool GUIInputHandler::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButton id)
  //{
    ////CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));
  //  return true;
  //}

}
