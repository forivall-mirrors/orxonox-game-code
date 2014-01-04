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
#include "util/StringUtils.h"
#include "util/Output.h"
#include "core/command/ConsoleCommand.h"
#include "core/command/CommandEvaluation.h"
#include "core/command/CommandExecutor.h"
#include "core/config/ConfigFile.h"

namespace orxonox
{
    /**
    @note
        bButtonThresholdUser_: We set it to true so that setConfigValues in KeyBinder sets the value
        correctly the first time. It is then set to false first and changed later in Button::parse().
    */
    Button::Button()
        : bButtonThresholdUser_(false)
        , paramCommandBuffer_(0)
    {
        nCommands_[0]=0;
        nCommands_[1]=0;
        nCommands_[2]=0;
    }

    Button::~Button()
    {
        this->clear();
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
        }
        this->bindingString_.clear();
    }

    void Button::readBinding(ConfigFile* configFile, ConfigFile* fallbackFile)
    {
        std::string binding = configFile->getOrCreateValue(groupName_, name_, "", true);
        if (binding.empty() && fallbackFile)
            binding = fallbackFile->getValue(groupName_, name_, true);
        this->parse(binding);
    }

    void Button::setBinding(ConfigFile* configFile, ConfigFile* fallbackFile, const std::string& binding, bool bTemporary)
    {
        if (!bTemporary)
            configFile->setValue(groupName_, name_, binding, true);
        this->parse(binding);
    }

    void Button::parse(const std::string& binding)
    {
        if (binding == this->bindingString_)
            return;

        // delete all commands
        clear();
        this->bindingString_ = binding;

        if (isEmpty(bindingString_) || removeTrailingWhitespaces(getLowercase(binding)) == "nobinding")
            return;

        // reset this to false first when parsing (was true before when parsing for the first time)
        bButtonThresholdUser_ = false;

        // use std::vector for a temporary dynamic array
        std::vector<BaseCommand*> commands[3];

        // separate the commands
        SubString commandStrings(bindingString_, "|", SubString::WhiteSpaces, false,
            '\\', false, '"', false, '{', '}', false, '\0');

        for (unsigned int iCommand = 0; iCommand < commandStrings.size(); iCommand++)
        {
            if (!commandStrings[iCommand].empty())
            {
                SubString tokens(commandStrings[iCommand], " ", SubString::WhiteSpaces, false,
                    '\\', false, '"', false, '{', '}', false, '\0');

                KeybindMode::Value mode = KeybindMode::None;
                float paramModifier = 1.0f;
                std::string commandStr;

                for (unsigned int iToken = 0; iToken < tokens.size(); ++iToken)
                {
                    const std::string& token = getLowercase(tokens[iToken]);

                    if (token == "onpress")
                        mode = KeybindMode::OnPress;
                    else if (token == "onrelease")
                        mode = KeybindMode::OnRelease;
                    else if (token == "onpressandrelease")
                        mode = KeybindMode::OnPressAndRelease;
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
                        else
                            this->bButtonThresholdUser_ = true;
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
                            commandStr += tokens[iToken++] + ' ';
                    }
                }

                if (commandStr.empty())
                {
                    parseError("No command string given.", false);
                    continue;
                }

                // evaluate the command
                CommandEvaluation eval = CommandExecutor::evaluate(commandStr);
                if (!eval.isValid() || eval.evaluateArguments(true))
                {
                    parseError("Command evaluation of \"" + commandStr + "\"failed.", true);
                    continue;
                }

                // check for param command
                int paramIndex = eval.getConsoleCommand()->getInputConfiguredParam_();
                if (paramIndex >= 0)
                {
                    // parameter supported command
                    ParamCommand* cmd = new ParamCommand();
                    cmd->scale_ = paramModifier;

                    // add command to the buffer if not yet existing
                    for (unsigned int iParamCmd = 0; iParamCmd < paramCommandBuffer_->size(); iParamCmd++)
                    {
                        if ((*paramCommandBuffer_)[iParamCmd]->evaluation_.getConsoleCommand()
                            == eval.getConsoleCommand())
                        {
                            // already in list
                            cmd->paramCommand_ = (*paramCommandBuffer_)[iParamCmd];
                            break;
                        }
                    }
                    if (cmd->paramCommand_ == 0)
                    {
                        cmd->paramCommand_ = new BufferedParamCommand();
                        paramCommandBuffer_->push_back(cmd->paramCommand_);
                        cmd->paramCommand_->evaluation_ = eval;
                        cmd->paramCommand_->paramIndex_ = paramIndex;
                    }


                    // we don't know whether this is an actual axis or just a button
                    if (mode != KeybindMode::None || !addParamCommand(cmd))
                        this->addCommand(cmd, mode, commands);
                }
                else
                {
                    SimpleCommand* cmd = new SimpleCommand();
                    cmd->evaluation_ = eval;

                    this->addCommand(cmd, mode, commands);
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

    inline void Button::addCommand(BaseCommand* cmd, KeybindMode::Value mode, std::vector<BaseCommand*> commands[3])
    {
        if (mode == KeybindMode::None)
            mode = cmd->getEvaluation()->getConsoleCommand()->getKeybindMode();
        else
            cmd->setFixedKeybindMode(true);

        if (mode == KeybindMode::OnPressAndRelease)
        {
            BaseCommand* cmd2 = cmd->clone();

            commands[KeybindMode::OnPress].push_back(cmd);
            commands[KeybindMode::OnRelease].push_back(cmd2); // clone
        }
        else
            commands[mode].push_back(cmd);
    }

    inline void Button::parseError(const std::string& message, bool serious)
    {
        if (serious)
        {
            orxout(internal_error, context::input) << "Error while parsing binding for button/axis " << this->name_ << ". "
                << message << endl;
        }
        else
        {
            orxout(internal_warning, context::input) << "Warning while parsing binding for button/axis " << this->name_ << ". "
                << message << endl;
        }
    }
}
