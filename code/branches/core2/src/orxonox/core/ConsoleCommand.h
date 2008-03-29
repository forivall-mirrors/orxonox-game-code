/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#ifndef _ConsoleCommand_H__
#define _ConsoleCommand_H__

#include "Executor.h"
#include "ClassManager.h"
#include "Identifier.h"
#include "CommandExecutor.h"



#define ConsoleCommand(classname, function, accesslevel, bCreateShortcut) \
    ConsoleCommandGeneric(classname##function##consolecommand__, classname, orxonox::createExecutor(orxonox::createFunctor(&classname::function), #function, accesslevel), bCreateShortcut)

#define ConsoleCommandGeneric(fakevariable, classname, executor, bCreateShortcut) \
    Executor& fakevariable = ClassManager<classname>::getIdentifier()->addConsoleCommand((ExecutorStatic*)executor, bCreateShortcut)


#define ConsoleCommandShortcut(function, accesslevel) \
    ConsoleCommandShortcutGeneric(function##consolecommand__, orxonox::createExecutor(orxonox::createFunctor(&classname::function), #function, accesslevel))

#define ConsoleCommandShortcutGeneric(fakevariable, executor) \
    bool fakevariable = CommandExecutor::addConsoleCommandShortcut((ExecutorStatic*)executor)


#endif /* _ConsoleCommand_H__ */
