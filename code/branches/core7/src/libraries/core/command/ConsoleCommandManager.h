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
     * A static class that stores all existing ConsoleCommands.
     */
    class _CoreExport ConsoleCommandManager
    {
        public:
            static void registerCommand(ConsoleCommand* command);
            static void registerCommand(const std::string& group, const std::string& name, ConsoleCommand* command);
            static void unregisterCommand(ConsoleCommand* command);

            /// Returns the map with all groups and commands.
            static inline const std::map<std::string, std::map<std::string, ConsoleCommand*> >& getCommands()
                { return ConsoleCommandManager::getCommandMap(); }
            /// Returns the map with all groups and commands in lowercase.
            static inline const std::map<std::string, std::map<std::string, ConsoleCommand*> >& getCommandsLC()
                { return ConsoleCommandManager::getCommandMapLC(); }

            /// Returns a command (shortcut) with given name. @param name The name of the command shortcut @param bPrintError If true, an error is printed if the command doesn't exist
            static inline ConsoleCommand* getCommand(const std::string& name, bool bPrintError = false)
                { return ConsoleCommandManager::getCommand("", name, bPrintError); }
            /// Returns a command (shortcut) with given name in lowercase. @param name The lowercase name of the command shortcut @param bPrintError If true, an error is printed if the command doesn't exist
            static inline ConsoleCommand* getCommandLC(const std::string& name, bool bPrintError = false)
                { return ConsoleCommandManager::getCommandLC("", name, bPrintError); }

            static ConsoleCommand* getCommand(const std::string& group, const std::string& name, bool bPrintError = false);
            static ConsoleCommand* getCommandLC(const std::string& group, const std::string& name, bool bPrintError = false);

            static void destroyAll();

        private:
            static std::map<std::string, std::map<std::string, ConsoleCommand*> >& getCommandMap();
            static std::map<std::string, std::map<std::string, ConsoleCommand*> >& getCommandMapLC();
    };
}

#endif /* _ConsoleCommandManager_H__ */
