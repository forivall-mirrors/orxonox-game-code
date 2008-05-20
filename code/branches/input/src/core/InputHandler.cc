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
  // ######      Button       ######
  // ###############################

  bool BufferedParamCommand::execute()
  {
    if (nValuesAdded_)
    {
      BufferedParamCommand& cmd = *this;
      cmd.evaluation_.setEvaluatedParameter(cmd.paramIndex_, cmd.value_);
      // reset
      cmd.nValuesAdded_ = 0;
      cmd.value_ = 0;
      return CommandExecutor::execute(cmd.evaluation_);
    }
    else
      return true;
  }

  bool SimpleCommand::execute(float abs, float rel)
  {
    return CommandExecutor::execute(evaluation_);
  }

  bool ParamCommand::execute(float abs, float rel)
  {
    BufferedParamCommand& paramCommand = *paramCommand_;
    // command has an additional parameter
    if (bRelative_)
    {
      // we have to calculate a relative movement.
      // amplitude says how much one keystroke is
      paramCommand.value_ += paramModifier_ * rel;
    }
    else
    {
      // we have to calculate absolute position of the axis.
      // for a key this simply is 1, but multiplied by a user defined factor
      // since there might be another axis that is affected, we have to wait and
      // store the result in a temporary place
      paramCommand.value_ = (paramCommand.value_ * paramCommand.nValuesAdded_ + paramModifier_ * abs)
                            /++paramCommand.nValuesAdded_;
    }
    return true;
  }

  void Button::clear()
  {
    for (unsigned int j = 0; j < 3; j++)
    {
      if (nCommands_[j])
      {
        // delete all commands and the command pointer array
        for (unsigned int i = 0; i < nCommands_[j]; i++)
          delete commands_[j][i];
        delete[] commands_[j];
        commands_[j] = 0;
        nCommands_[j] = 0;
      }
      else
      {
        commands_[j] = 0;
      }
    }
  }

  void Button::parse(std::vector<BufferedParamCommand*>& paramCommandBuffer)
  {
    if (isEmpty(bindingString_))
    {
      clear();
      return;
    }

    // use std::vector for a temporary dynamic array
    std::vector<BaseCommand*> commands[3];


    // separate the commands
    SubString commandStrings(bindingString_, "|", SubString::WhiteSpaces, false,
        '\\', false, '"', false, '(', ')', false, '\0');

    for (unsigned int iCommand = 0; iCommand < commandStrings.size(); iCommand++)
    {
      if (commandStrings[iCommand] != "")
      {
        SubString tokens(commandStrings[iCommand], " ", SubString::WhiteSpaces, false,
            '\\', false, '"', false, '(', ')', false, '\0');
        
        unsigned int iToken = 0;

        // for real axes, we can feed a ButtonThreshold argument as entire command
        if (getLowercase(tokens[0]) == "buttonthreshold")
        {
          if (tokens.size() == 1)
            continue;
          // may fail, but doesn't matter
          convertValue(&buttonThreshold_, tokens[1]);
          continue;
        }

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

        // second argument can be the amplitude for the case it as an axis command
        // default amplitude is 1.0f
        float paramModifier = 1.0f;
        if (getLowercase(tokens[iToken]) == "axisamp")
        {
          iToken++;
          if (iToken == tokens.size() || !convertValue(&paramModifier, tokens[iToken]))
          {
            COUT(2) << "Error while parsing key binding " << name_
                << ". Numeric expression expected afer 'AxisAmp', switching to default value" << std::endl;
            if (iToken == tokens.size())
              continue;
          }
          iToken++;
        }

        // no more arguments expected except for the actual command
        if (iToken == tokens.size())
          continue;

        std::string commandStr;
        while (iToken != tokens.size())
          commandStr += tokens[iToken++] + " ";

        // evaluate the command
        CommandEvaluation& eval = CommandExecutor::evaluate(commandStr);
        if (!eval.isValid())
          continue;

        // check for param command
        int paramIndex = eval.getEvaluatedExecutor()->getAxisParamIndex();
        // TODO: check in Executor for correct paramIndex
        if (paramIndex >= 0)
        {
          // parameter supported command
          ParamCommand* cmd = new ParamCommand();
          cmd->paramModifier_ = paramModifier;
          cmd->bRelative_ = eval.getEvaluatedExecutor()->getIsAxisRelative();

          // add command to the buffer if not yet existing
          for (unsigned int iParamCmd = 0; iParamCmd < paramCommandBuffer.size(); iParamCmd++)
          {
            if (getLowercase(paramCommandBuffer[iParamCmd]->evaluation_.getCommandString())
                == getLowercase(commandStr))
            {
              // already in list
              cmd->paramCommand_ = paramCommandBuffer[iParamCmd];
              break;
            }
          }
          if (cmd->paramCommand_ == 0)
          {
            cmd->paramCommand_ = new BufferedParamCommand();
            paramCommandBuffer.push_back(cmd->paramCommand_);
            cmd->paramCommand_->evaluation_ = eval;
            cmd->paramCommand_->paramIndex_ = paramIndex;
          }


          // we don't know whether this is an actual axis or just a button
          if (mode == KeybindMode::None)
          {
            if (!addParamCommand(cmd))
            {
              mode = eval.getEvaluatedExecutor()->getKeybindMode();
              commands[mode].push_back(cmd);
            }
          }
        }
        else
        {
          SimpleCommand* cmd = new SimpleCommand();
          cmd->evaluation_ = eval;

          //TODO: check CommandEvaluation for correct KeybindMode
          if (mode == KeybindMode::None)
            mode = eval.getEvaluatedExecutor()->getKeybindMode();

          commands[mode].push_back(cmd);
        }
      }
    }

    for (unsigned int j = 0; j < 3; j++)
    {
      nCommands_[j] = commands[j].size();
      if (nCommands_[j])
      {
        commands_[j] = new BaseCommand*[nCommands_[j]];
        for (unsigned int i = 0; i < commands[j].size(); i++)
          commands_[j][i] = commands[j][i];
      }
      else
        commands_[j] = 0;
    }
  }

  bool Button::execute(KeybindMode::Enum mode)
  {
    // execute all the parsed commands in the string
    for (unsigned int iCommand = 0; iCommand < nCommands_[mode]; iCommand++)
      commands_[mode][iCommand]->execute();
    return true;
  }

  void HalfAxis::clear()
  {
    Button::clear();
    if (nParamCommands_)
    {
      // delete all commands and the command pointer array
      for (unsigned int i = 0; i < nParamCommands_; i++)
        delete paramCommands_[i];
      delete[] paramCommands_;
    }
    else
    {
      nParamCommands_ = 0; nParamCommands_ = 0;
    }
  }
  
  bool HalfAxis::addParamCommand(ParamCommand* command)
  {
    ParamCommand** cmds = paramCommands_;
    paramCommands_ = new ParamCommand*[++nParamCommands_];
    unsigned int i;
    for (i = 0; i < nParamCommands_ - 1; i++)
      paramCommands_[i] = cmds[i];
    paramCommands_[i] = command;
    delete[] cmds;
    return true;
  }

  bool HalfAxis::execute()
  {
    bool success = true;
    for (unsigned int i = 0; i < nParamCommands_; i++)
      success = success && paramCommands_[i]->execute(absVal_, relVal_);
    return success;
  }


  // ###############################
  // ######     KeyBinder     ######
  // ###############################

  /**
    @brief Constructor that does as little as necessary.
  */
  KeyBinder::KeyBinder()
  {
    RegisterObject(KeyBinder);

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
    for (int i = 0; i < nKeys_s; i++)
      keys_[i].name_ = "Key" + keyNames[i];

    // mouse buttons
    std::string mouseButtonNames[] = {
      "MouseLeft", "MouseRight", "MouseMiddle",
      "MouseButton3", "MouseButton4", "MouseButton5",
      "MouseButton6", "MouseButton7" };
    for (int i = 0; i < nMouseButtons_s; i++)
      mouseButtons_[i].name_ = mouseButtonNames[i];

    // joy stick buttons
    for (int i = 0; i < 32; i++)
      joyStickButtons_[i].name_ = "JoyButton" + getConvertedValue<int, std::string>(i);
    for (int i = 32; i < nJoyStickButtons_s; i += 4)
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
    rawNames[2] = "MouseWheel1";
    rawNames[3] = "MouseWheel2";
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

    ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, "keybindings.ini");
    clearBindings();
    setConfigValues();

    COUT(3) << "KeyBinder: Loading key bindings done." << std::endl;
  }

  /**
    @brief Loader for the key bindings, managed by config values.
  */
  void KeyBinder::setConfigValues()
  {
    SetConfigValue(analogThreshold_, 0.01f).description("Threshold for analog axes until which the state is 0.");
    float oldThresh = buttonThreshold_;
    SetConfigValue(buttonThreshold_, 0.80f).description("Threshold for analog axes until which the button is not pressed.");
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
    ConfigValueContainer* cont = getIdentifier()->getConfigValueContainer(button.name_);
    if (!cont)
    {
      cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), button.name_, "");
      getIdentifier()->addConfigValueContainer(button.name_, cont);
    }
    std::string old = button.bindingString_;
    cont->getValue(&button.bindingString_);

    // keybinder stuff
    if (old != button.bindingString_)
    {
      // binding has changed
      button.parse(paramCommandBuffer_);
    }
  }

  /**
    @brief Overwrites all bindings with ""
  */
  void KeyBinder::clearBindings(bool bInit)
  {
    for (int i = 0; i < nKeys_s; i++)
      keys_[i].clear();

    for (int i = 0; i < nMouseButtons_s; i++)
      mouseButtons_[i].clear();

    for (int i = 0; i < nJoyStickButtons_s; i++)
      joyStickButtons_[i].clear();

    for (int i = 0; i < nHalfAxes_s; i++)
      halfAxes_[i].clear();

    for (unsigned int i = 0; i < paramCommandBuffer_.size(); i++)
      delete paramCommandBuffer_[i];
    paramCommandBuffer_.clear();
  }

  void KeyBinder::tick(float dt)
  {
    // we have to process all the analog input since there is e.g. no 'mouseDoesntMove' event.
    for (unsigned int i = 0; i < nHalfAxes_s; i++)
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
        if (halfAxes_[i].wasDown_)
        {
          if (halfAxes_[i].nCommands_[KeybindMode::OnHold])
            halfAxes_[i].execute(KeybindMode::OnHold);
        }
        halfAxes_[i].hasChanged_ = false;
      }

      // these are the actually useful axis bindings for analog input AND output
      if (halfAxes_[i].relVal_ > analogThreshold_ || halfAxes_[i].absVal_ > analogThreshold_)
      {
        halfAxes_[i].execute();
      }
    }

    // execute all buffered bindings (addional parameter)
    for (unsigned int i = 0; i < paramCommandBuffer_.size(); i++)
      paramCommandBuffer_[i]->execute();

    // always reset the relative movement of the mouse
    for (unsigned int i = 0; i < 4; i++)
      halfAxes_[i].relVal_ = 0;
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
  void KeyBinder::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
  {
    // translate absolute mouse position into joystick like behaviour
    if (clippingSize.x > clippingSize.y)
    {
      int margin = (clippingSize.x - clippingSize.y) / 2;
      if (abs.x - margin > clippingSize.y)
      {
        halfAxes_[0].absVal_ = 1.0f;
        halfAxes_[1].absVal_ = 0.0f;
      }
      else if (abs.x < margin)
      {
        halfAxes_[0].absVal_ = 0.0f;
        halfAxes_[1].absVal_ = 1.0f;
      }
      else
      {
        float temp = ((float)abs.x) / clippingSize.y * 2 - 1;
        if (temp > 0)
        {
          halfAxes_[0].absVal_ = temp;
          halfAxes_[1].absVal_ = 0.0f;
        }
        else
        {
          halfAxes_[0].absVal_ = 0.0f;
          halfAxes_[1].absVal_ = -temp;
        }
      }

      float temp = -((float)abs.y) / clippingSize.y * 2 + 1;
      if (temp > 0)
      {
        halfAxes_[2].absVal_ = temp;
        halfAxes_[3].absVal_ = 0.0;
      }
      else
      {
        halfAxes_[2].absVal_ = 0.0;
        halfAxes_[3].absVal_ = -temp;
      }
    }
    else
    {
      float temp = ((float)abs.x) / clippingSize.x * 2 - 1;
      if (temp > 0)
      {
        halfAxes_[0].absVal_ = temp;
        halfAxes_[1].absVal_ = 0.0;
      }
      else
      {
        halfAxes_[0].absVal_ = 0.0;
        halfAxes_[1].absVal_ = -temp;
      }

      int margin = (clippingSize.y - clippingSize.x) / 2;
      if (abs.y - margin > clippingSize.x)
      {
        halfAxes_[2].absVal_ = 0.0;
        halfAxes_[3].absVal_ = 1.0;
      }
      else if (abs.y < margin)
      {
        halfAxes_[2].absVal_ = 1.0;
        halfAxes_[3].absVal_ = 0.0;
      }
      else
      {
        float temp = -((float)abs.y) / clippingSize.x * 2 + 1;
        if (temp > 0)
        {
          halfAxes_[2].absVal_ = temp;
          halfAxes_[3].absVal_ = 0.0;
        }
        else
        {
          halfAxes_[2].absVal_ = 0.0;
          halfAxes_[3].absVal_ = -temp;
        }
      }
    }

    // relative movements
    if (rel.x > 0)
    {
      halfAxes_[0].hasChanged_ = true;
      halfAxes_[1].hasChanged_ = true;
      halfAxes_[0].relVal_ = rel.x;
      halfAxes_[1].relVal_ = 0.0;
    }
    else if (rel.x < 0)
    {
      halfAxes_[0].hasChanged_ = true;
      halfAxes_[1].hasChanged_ = true;
      halfAxes_[0].relVal_ = 0.0;
      halfAxes_[1].relVal_ = rel.x;
    }

    if (rel.y /*!*/ < /*!*/ 0)
    {
      halfAxes_[2].hasChanged_ = true;
      halfAxes_[3].hasChanged_ = true;
      halfAxes_[0].relVal_ = -rel.y;
      halfAxes_[1].relVal_ = 0.0;
    }
    else if (rel.y > 0)
    {
      halfAxes_[2].hasChanged_ = true;
      halfAxes_[3].hasChanged_ = true;
      halfAxes_[0].relVal_ = 0.0;
      halfAxes_[1].relVal_ = -rel.y;
    }
  }

  /**
    @brief Event handler for the mouseScrolled Event.
    @param e Mouse state information
  */
  void KeyBinder::mouseScrolled(int abs, int rel)
  {
    // TODO: obvious...
  }

  void KeyBinder::joyStickAxisMoved(int joyStickID, int axis, int value)
  {
    // TODO: check whether 16 bit integer as general axis value is a good idea (works under windows)
    //CCOUT(3) << axis << std::endl;
    if (value >= 0)
    {
      halfAxes_[8 + axis].absVal_ = ((float)value)/0x8000;
      halfAxes_[8 + axis].relVal_ = ((float)value)/0x8000;
      halfAxes_[8 + axis].hasChanged_ = true;
    }
    else
    {
      halfAxes_[8 + axis + 1].absVal_ = -((float)value)/0x8000;
      halfAxes_[8 + axis + 1].relVal_ = -((float)value)/0x8000;
      halfAxes_[8 + axis + 1].hasChanged_ = true;
    }
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
