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

#include "Button.h"
#include "util/Convert.h"
#include "util/SubString.h"
#include "util/String.h"
#include "core/Debug.h"
#include "core/ConsoleCommand.h"
#include "core/CommandEvaluation.h"
#include "core/CommandExecutor.h"
#include "InputCommands.h"

namespace orxonox
{
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
        if (getLowercase(tokens[iToken]) == "scale")
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
        CommandEvaluation eval = CommandExecutor::evaluate(commandStr);
        if (!eval.isValid())
          continue;

        // check for param command
        int paramIndex = eval.getConsoleCommand()->getAxisParamIndex();
        if (paramIndex >= 0)
        {
          // parameter supported command
          ParamCommand* cmd = new ParamCommand();
          cmd->paramModifier_ = paramModifier;
          cmd->bRelative_ = eval.getConsoleCommand()->getIsAxisRelative();

          // add command to the buffer if not yet existing
          for (unsigned int iParamCmd = 0; iParamCmd < paramCommandBuffer.size(); iParamCmd++)
          {
            if (getLowercase(paramCommandBuffer[iParamCmd]->evaluation_.getOriginalCommand())
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
              mode = eval.getConsoleCommand()->getKeybindMode();
              commands[mode].push_back(cmd);
            }
          }
        }
        else
        {
          SimpleCommand* cmd = new SimpleCommand();
          cmd->evaluation_ = eval;

          if (mode == KeybindMode::None)
            mode = eval.getConsoleCommand()->getKeybindMode();

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

  bool Button::execute(KeybindMode::Enum mode, float abs, float rel)
  {
    // execute all the parsed commands in the string
    for (unsigned int iCommand = 0; iCommand < nCommands_[mode]; iCommand++)
      commands_[mode][iCommand]->execute(abs, rel);
    return true;
  }
}
