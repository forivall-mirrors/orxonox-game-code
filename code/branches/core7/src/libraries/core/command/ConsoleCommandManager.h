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
    @defgroup ConsoleCommand Console commands
    @ingroup Command
*/

#ifndef _ConsoleCommandManager_H__
#define _ConsoleCommandManager_H__

#include "core/CorePrereqs.h"

namespace orxonox
{
    /**
     * A singleton that stores all existing ConsoleCommands.
     */
    class _CoreExport ConsoleCommandManager
    {
        public:
            static ConsoleCommandManager& getInstance();

            void registerCommand(ConsoleCommand* command);
            void registerCommand(const std::string& group, const std::string& name, ConsoleCommand* command);
            void unregisterCommand(ConsoleCommand* command);

            /// Returns the map with all groups and commands.
            inline const std::map<std::string, std::map<std::string, ConsoleCommand*> >& getCommands()
                { return this->commandMap_; }
            /// Returns the map with all groups and commands in lowercase.
            inline const std::map<std::string, std::map<std::string, ConsoleCommand*> >& getCommandsLC()
                { return this->commandMapLC_; }

            /// Returns a command (shortcut) with given name. @param name The name of the command shortcut @param bPrintError If true, an error is printed if the command doesn't exist
            inline ConsoleCommand* getCommand(const std::string& name, bool bPrintError = false)
                { return this->getCommand("", name, bPrintError); }
            /// Returns a command (shortcut) with given name in lowercase. @param name The lowercase name of the command shortcut @param bPrintError If true, an error is printed if the command doesn't exist
            inline ConsoleCommand* getCommandLC(const std::string& name, bool bPrintError = false)
                { return this->getCommandLC("", name, bPrintError); }

            ConsoleCommand* getCommand(const std::string& group, const std::string& name, bool bPrintError = false);
            ConsoleCommand* getCommandLC(const std::string& group, const std::string& name, bool bPrintError = false);

            void destroyAll();

        private:
            std::map<std::string, std::map<std::string, ConsoleCommand*> > commandMap_;
            std::map<std::string, std::map<std::string, ConsoleCommand*> > commandMapLC_;
    };
}

#endif /* _ConsoleCommandManager_H__ */
