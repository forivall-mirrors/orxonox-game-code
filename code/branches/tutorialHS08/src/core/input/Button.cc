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
@brief
    Implementation of the different input handlers.
*/

#include "Button.h"
#include "util/Convert.h"
#include "util/SubString.h"
#include "util/String.h"
#include "util/Debug.h"
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

                KeybindMode::Enum mode = KeybindMode::None;
                float paramModifier = 1.0f;
                std::string commandStr = "";

                for (unsigned int iToken = 0; iToken < tokens.size(); ++iToken)
                {
                    std::string token = getLowercase(tokens[iToken]);

                    if (token == "onpress")
                        mode = KeybindMode::OnPress;
                    else if (token == "onrelease")
                        mode = KeybindMode::OnRelease;
                    else if (token == "onhold")
                        mode = KeybindMode::OnHold;
                    else if (token == "buttonthreshold")
                    {
                        // for real axes, we can feed a ButtonThreshold argument
                        ++iToken;
                        if (iToken == tokens.size())
                            continue;
                        // may fail, but doesn't matter (default value already set)
                        if (!convertValue(&buttonThreshold_, tokens[iToken + 1]))
                            parseError("Could not parse 'ButtonThreshold' argument. \
                                Switching to default value.", true);
                    }
                    else if (token == "scale")
                    {
                        ++iToken;
                        if (iToken == tokens.size() || !convertValue(&paramModifier, tokens[iToken]))
                            parseError("Could not parse 'scale' argument. Switching to default value.", true);
                    }
                    else
                    {
                        // no input related argument
                        // we interpret everything from here as a command string
                        while (iToken != tokens.size())
                            commandStr += tokens[iToken++] + " ";
                    }
                }

                if (commandStr == "")
                {
                    parseError("No command string given.", false);
                    continue;
                }

                // evaluate the command
                CommandEvaluation eval = CommandExecutor::evaluate(commandStr);
                if (!eval.isValid())
                {
                    parseError("Command evaluation failed.", true);
                    continue;
                }

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

    inline void Button::parseError(std::string message, bool serious)
    {
        if (serious)
        {
            COUT(2) << "Error while parsing binding for button/axis " << this->name_ << ". "
                << message << std::endl;
        }
        else
        {
            COUT(3) << "Warning while parsing binding for button/axis " << this->name_ << ". "
                << message << std::endl;
        }
    }
}
