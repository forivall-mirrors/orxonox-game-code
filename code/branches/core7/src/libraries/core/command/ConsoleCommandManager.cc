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

/**
    @file
    @brief Implementation of the ConsoleCommand class.
*/

#include "ConsoleCommandManager.h"

#include "ConsoleCommand.h"
#include "util/StringUtils.h"

namespace orxonox
{
    /* static */ ConsoleCommandManager& ConsoleCommandManager::getInstance()
    {
        static ConsoleCommandManager instance;
        return instance;
    }

    /**
        @brief Returns the command with given group an name.
        @param group The group of the requested command
        @param name The group of the requested command
        @param bPrintError If true, an error is printed if the command doesn't exist
    */
    ConsoleCommand* ConsoleCommandManager::getCommand(const std::string& group, const std::string& name, bool bPrintError)
    {
        // find the group
        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = this->commandMap_.find(group);
        if (it_group != this->commandMap_.end())
        {
            // find the name
            std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.find(name);
            if (it_name != it_group->second.end())
            {
                // return the pointer
                return it_name->second;
            }
        }
        if (bPrintError)
        {
            if (group == "")
                orxout(internal_error, context::commands) << "Couldn't find console command with shortcut \"" << name << "\"" << endl;
            else
                orxout(internal_error, context::commands) << "Couldn't find console command with group \"" << group << "\" and name \"" << name << "\"" << endl;
        }
        return 0;
    }

    /**
        @brief Returns the command with given group an name in lowercase.
        @param group The group of the requested command in lowercase
        @param name The group of the requested command in lowercase
        @param bPrintError If true, an error is printed if the command doesn't exist
    */
    ConsoleCommand* ConsoleCommandManager::getCommandLC(const std::string& group, const std::string& name, bool bPrintError)
    {
        std::string groupLC = getLowercase(group);
        std::string nameLC = getLowercase(name);

        // find the group
        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = this->commandMapLC_.find(groupLC);
        if (it_group != this->commandMapLC_.end())
        {
            // find the name
            std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.find(nameLC);
            if (it_name != it_group->second.end())
            {
                // return the pointer
                return it_name->second;
            }
        }
        if (bPrintError)
        {
            if (group == "")
                orxout(internal_error, context::commands) << "Couldn't find console command with shortcut \"" << name << "\"" << endl;
            else
                orxout(internal_error, context::commands) << "Couldn't find console command with group \"" << group << "\" and name \"" << name << "\"" << endl;
        }
        return 0;
    }

    /**
        @brief Registers a new command with the groups and names that are defined by ConsoleCommand::getNames().
    */
    void ConsoleCommandManager::registerCommand(ConsoleCommand* command)
    {
        for (size_t i = 0; i < command->getNames().size(); ++i)
        {
            const ConsoleCommand::CommandName& name = command->getNames()[i];
            this->registerCommand(name.group_, name.name_, command);
        }
    }

    /**
        @brief Registers a new command with given group an name by adding it to the command map.
    */
    void ConsoleCommandManager::registerCommand(const std::string& group, const std::string& name, ConsoleCommand* command)
    {
        if (name == "")
            return;

        // check if a command with this name already exists
        if (this->getCommand(group, name) != 0)
        {
            if (group == "")
                orxout(internal_warning, context::commands) << "A console command with shortcut \"" << name << "\" already exists." << endl;
            else
                orxout(internal_warning, context::commands) << "A console command with name \"" << name << "\" already exists in group \"" << group << "\"." << endl;
        }
        else
        {
            // add the command to the map
            this->commandMap_[group][name] = command;
            this->commandMapLC_[getLowercase(group)][getLowercase(name)] = command;
        }
    }

    /**
        @brief Removes the command from the command map.
    */
    void ConsoleCommandManager::unregisterCommand(ConsoleCommand* command)
    {
        // iterate through all groups
        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::iterator it_group = this->commandMap_.begin(); it_group != this->commandMap_.end(); )
        {
            // iterate through all commands of each group
            for (std::map<std::string, ConsoleCommand*>::iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); )
            {
                // erase the command
                if (it_name->second == command)
                    it_group->second.erase(it_name++);
                else
                    ++it_name;
            }

            // erase the group if it is empty now
            if (it_group->second.empty())
                this->commandMap_.erase(it_group++);
            else
                ++it_group;
        }

        // now the same for the lowercase-map:

        // iterate through all groups
        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::iterator it_group = this->commandMapLC_.begin(); it_group != this->commandMapLC_.end(); )
        {
            // iterate through all commands of each group
            for (std::map<std::string, ConsoleCommand*>::iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); )
            {
                // erase the command
                if (it_name->second == command)
                    it_group->second.erase(it_name++);
                else
                    ++it_name;
            }

            // erase the group if it is empty now
            if (it_group->second.empty())
                this->commandMapLC_.erase(it_group++);
            else
                ++it_group;
        }
    }

    /**
        @brief Deletes all commands
    */
    void ConsoleCommandManager::destroyAll()
    {
        // delete entries until the map is empty
        while (!this->commandMap_.empty() && !this->commandMap_.begin()->second.empty())
            delete this->commandMap_.begin()->second.begin()->second;
    }
}
