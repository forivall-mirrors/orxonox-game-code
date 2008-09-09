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

#ifndef _ConsoleCommand_H__
#define _ConsoleCommand_H__

#include "CorePrereqs.h"

#include "Executor.h"
#include "Identifier.h"
#include "CommandExecutor.h"
#include "ArgumentCompletionFunctions.h"


#define SetConsoleCommand(classname, function, bCreateShortcut) \
    SetConsoleCommandAliasMulti(classname, function, #function, 0, bCreateShortcut)
#define SetConsoleCommandAlias(classname, function, name, bCreateShortcut) \
    SetConsoleCommandAliasMulti(classname, function, name, 0, bCreateShortcut)
#define SetConsoleCommandAliasMulti(classname, function, name, number, bCreateShortcut) \
    SetConsoleCommandGeneric(classname##function##consolecommand__##number, classname, orxonox::createConsoleCommand(orxonox::createFunctor(&classname::function), name), bCreateShortcut)

#define SetConsoleCommandGeneric(fakevariable, classname, command, bCreateShortcut) \
    orxonox::ConsoleCommand& fakevariable = orxonox::ClassIdentifier<classname>::getIdentifier(#classname)->addConsoleCommand(command, bCreateShortcut)


#define SetConsoleCommandShortcut(classname, function) \
    SetConsoleCommandShortcutAliasMulti(classname, function, #function, 0)
#define SetConsoleCommandShortcutAlias(classname, function, name) \
    SetConsoleCommandShortcutAliasMulti(classname, function, name, 0)
#define SetConsoleCommandShortcutAliasMulti(classname, function, name, number) \
    SetConsoleCommandShortcutGeneric(function##consolecommand__##number, orxonox::createConsoleCommand(orxonox::createFunctor(&classname::function), name))

#define SetConsoleCommandShortcutExtern(function) \
    SetConsoleCommandShortcutExternAliasMulti(function, #function, 0)
#define SetConsoleCommandShortcutExternAlias(function, name) \
    SetConsoleCommandShortcutExternAliasMulti(function, name, 0)
#define SetConsoleCommandShortcutExternAliasMulti(function, name, number) \
    SetConsoleCommandShortcutGeneric(function##consolecommand__##number, orxonox::createConsoleCommand(orxonox::createFunctor(&function), name))

#define SetConsoleCommandShortcutGeneric(fakevariable, command) \
    orxonox::ConsoleCommand& fakevariable = orxonox::CommandExecutor::addConsoleCommandShortcut(command)


namespace orxonox
{
    namespace AccessLevel
    {
        enum Level
        {
            None,
            User,
            Admin,
            Offline,
            Debug,
            Disabled
        };
    }

    class _CoreExport ConsoleCommand : public ExecutorStatic
    {
        public:
            ConsoleCommand(FunctorStatic* functor, const std::string& name = "");

            inline ConsoleCommand& description(const std::string& description)
                { this->ExecutorStatic::setDescription(description); return (*this); }
            inline ConsoleCommand& descriptionParam(int param, const std::string& description)
                { this->ExecutorStatic::setDescriptionParam(param, description); return (*this); }
            inline ConsoleCommand& descriptionReturnvalue(const std::string& description)
                { this->ExecutorStatic::setDescriptionReturnvalue(description); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1)
                { this->ExecutorStatic::setDefaultValues(param1); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2)
                { this->ExecutorStatic::setDefaultValues(param1, param2); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
                { this->ExecutorStatic::setDefaultValues(param1, param2, param3); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
                { this->ExecutorStatic::setDefaultValues(param1, param2, param3, param4); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
                { this->ExecutorStatic::setDefaultValues(param1, param2, param3, param4, param5); return (*this); }
            inline ConsoleCommand& defaultValue(unsigned int index, const MultiType& param)
                { this->ExecutorStatic::setDefaultValue(index, param); return (*this); }

            inline ConsoleCommand& accessLevel(AccessLevel::Level level)
                { this->accessLevel_ = level; return (*this); }
            inline AccessLevel::Level getAccessLevel() const
                { return this->accessLevel_; }

            ConsoleCommand& argumentCompleter(unsigned int param, ArgumentCompleter* completer);
            ArgumentCompleter* getArgumentCompleter(unsigned int param) const;

            void createArgumentCompletionList(unsigned int param, const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "", const std::string& param5 = "");
            const ArgumentCompletionList& getArgumentCompletionList() const
                { return this->argumentList_; }
            ArgumentCompletionList::const_iterator getArgumentCompletionListBegin() const
                { return this->argumentList_.begin(); }
            ArgumentCompletionList::const_iterator getArgumentCompletionListEnd() const
                { return this->argumentList_.end(); }

            inline ConsoleCommand& keybindMode(KeybindMode::Enum mode)
                { this->keybindMode_ = mode; return *this; }
            inline KeybindMode::Enum getKeybindMode() const
                { return this->keybindMode_; }

            inline ConsoleCommand& axisParamIndex(int index)
                { this->axisParamIndex_ = index; return *this; }
            inline int getAxisParamIndex() const
                { return this->axisParamIndex_; }

            inline ConsoleCommand& isAxisRelative(bool val)
                { this->bAxisRelative_ = val; return *this; }
            inline int getIsAxisRelative() const
                { return this->bAxisRelative_; }

        private:
            AccessLevel::Level accessLevel_;
            ArgumentCompleter* argumentCompleter_[5];
            ArgumentCompletionList argumentList_;

            KeybindMode::Enum keybindMode_;
            int axisParamIndex_;
            bool bAxisRelative_;
    };

    inline ConsoleCommand* createConsoleCommand(FunctorStatic* functor, const std::string& name = "")
    {
        return new ConsoleCommand(functor, name);
    }
}

#endif /* _ConsoleCommand_H__ */
