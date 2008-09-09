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

#include "KeyDetector.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "core/CommandExecutor.h"
#include "core/CommandEvaluation.h"
#include "InputCommands.h"
#include "Button.h"

namespace orxonox
{
    /**
    @brief
        Constructor
    */
    KeyDetector::KeyDetector()
    {
        RegisterObject(KeyDetector);
    }

    /**
    @brief
        Destructor
    */
    KeyDetector::~KeyDetector()
    {
    }

    /**
    @brief
        Loads the key and button bindings.
    @return
        True if loading succeeded.
    */
    void KeyDetector::loadBindings(const std::string& command)
    {
        this->command_ = command;
        clearBindings();
        setConfigValues();
    }

    void KeyDetector::readTrigger(Button& button)
    {
        SimpleCommand* cmd = new SimpleCommand();
        cmd->evaluation_ = CommandExecutor::evaluate(this->command_ + " " + button.name_);
        button.commands_[KeybindMode::OnPress] = new BaseCommand*[1];
        button.commands_[KeybindMode::OnPress][0] = cmd;
        button.nCommands_[KeybindMode::OnPress] = 1;
    }
}
