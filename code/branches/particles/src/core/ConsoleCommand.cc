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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "ConsoleCommand.h"

namespace orxonox
{
    ConsoleCommand::ConsoleCommand(Functor* functor, const std::string& name) : Executor(functor, name)
    {
        this->accessLevel_ = AccessLevel::None;
        this->argumentCompleter_[0] = 0;
        this->argumentCompleter_[1] = 0;
        this->argumentCompleter_[2] = 0;
        this->argumentCompleter_[3] = 0;
        this->argumentCompleter_[4] = 0;

        this->keybindMode_ = KeybindMode::OnPress;
        this->inputConfiguredParam_ = -1;
    }

    ConsoleCommand& ConsoleCommand::argumentCompleter(unsigned int param, ArgumentCompleter* completer)
    {
        if (param < 5)
            this->argumentCompleter_[param] = completer;
        else
        {
            COUT(2) << "Warning: Couldn't add autocompletion-function for param " << param << ": index out of bound." << std::endl;
        }
        return (*this);
    }

    ArgumentCompleter* ConsoleCommand::getArgumentCompleter(unsigned int param) const
    {
        if (param < 5)
            return this->argumentCompleter_[param];
        else
            return 0;
    }

    void ConsoleCommand::createArgumentCompletionList(unsigned int param, const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4, const std::string& param5)
    {
        if (param < 5 && this->argumentCompleter_[param])
            this->argumentList_ = (*this->argumentCompleter_[param])(param1, param2, param3, param4, param5);
        else
            this->argumentList_.clear();
    }
}
