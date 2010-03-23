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

#include <boost/preprocessor/cat.hpp>

#include "ArgumentCompletionFunctions.h"
#include "CommandExecutor.h"
#include "Executor.h"
#include "Identifier.h"


#define SetConsoleCommand(classname, function, bCreateShortcut) \
    SetConsoleCommandGeneric(classname, function, #function, bCreateShortcut)
#define SetConsoleCommandAlias(classname, function, name, bCreateShortcut) \
    SetConsoleCommandGeneric(classname, function, name, bCreateShortcut)

#define SetConsoleCommandGeneric(classname, function, name, bCreateShortcut) \
    orxonox::ConsoleCommand& BOOST_PP_CAT(classname##function##consolecommand__, __LINE__) = orxonox::ClassIdentifier<classname>::getIdentifier(#classname)->addConsoleCommand(orxonox::createConsoleCommand(orxonox::createFunctor(&classname::function), name), bCreateShortcut)


#define SetConsoleCommandShortcut(classname, function) \
    SetConsoleCommandShortcutAliasGeneric(classname, function, #function)
#define SetConsoleCommandShortcutAlias(classname, function, name) \
    SetConsoleCommandShortcutAliasGeneric(classname, function, name)
#define SetConsoleCommandShortcutAliasGeneric(classname, function, name) \
    SetConsoleCommandShortcutGeneric(BOOST_PP_CAT(function##consolecommand__, __LINE__), orxonox::createConsoleCommand(orxonox::createFunctor(&classname::function), name))

#define SetConsoleCommandShortcutExtern(function) \
    SetConsoleCommandShortcutExternAliasGeneric(function, #function)
#define SetConsoleCommandShortcutExternAlias(function, name) \
    SetConsoleCommandShortcutExternAliasGeneric(function, name)
#define SetConsoleCommandShortcutExternAliasGeneric(function, name) \
    SetConsoleCommandShortcutGeneric(BOOST_PP_CAT(function##consolecommand__, __LINE__), orxonox::createConsoleCommand(orxonox::createFunctor(&function), name))

#define SetConsoleCommandShortcutGeneric(fakevariable, command) \
    orxonox::ConsoleCommand& fakevariable = orxonox::CommandExecutor::addConsoleCommandShortcut(command, true)


namespace orxonox
{
    namespace AccessLevel
    {
        enum Value
        {
            None,
            User,
            Admin,
            Offline,
            Debug,
            Disabled
        };
    }

    class _CoreExport ConsoleCommand : public Executor
    {
        public:
            ConsoleCommand(Functor* functor, const std::string& name = "");

            inline ConsoleCommand& description(const std::string& description)
                { this->Executor::setDescription(description); return (*this); }
            inline ConsoleCommand& descriptionParam(int param, const std::string& description)
                { this->Executor::setDescriptionParam(param, description); return (*this); }
            inline ConsoleCommand& descriptionReturnvalue(const std::string& description)
                { this->Executor::setDescriptionReturnvalue(description); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1)
                { this->Executor::setDefaultValues(param1); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2)
                { this->Executor::setDefaultValues(param1, param2); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
                { this->Executor::setDefaultValues(param1, param2, param3); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
                { this->Executor::setDefaultValues(param1, param2, param3, param4); return (*this); }
            inline ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
                { this->Executor::setDefaultValues(param1, param2, param3, param4, param5); return (*this); }
            inline ConsoleCommand& defaultValue(unsigned int index, const MultiType& param)
                { this->Executor::setDefaultValue(index, param); return (*this); }

            inline ConsoleCommand& accessLevel(AccessLevel::Value level)
                { this->accessLevel_ = level; return (*this); }
            inline AccessLevel::Value getAccessLevel() const
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

            inline ConsoleCommand& setAsInputCommand()
            {
                this->keybindMode(KeybindMode::OnHold);
                this->defaultValue(0, Vector2(0.0f, 0.0f));
                this->inputConfiguredParam(0);
                return *this;
            }

            inline ConsoleCommand& keybindMode(KeybindMode::Value mode)
                { this->keybindMode_ = mode; return *this; }
            inline KeybindMode::Value getKeybindMode() const
                { return this->keybindMode_; }

            inline ConsoleCommand& inputConfiguredParam(int index)
                { this->inputConfiguredParam_ = index; return *this; }
            inline int getInputConfiguredParam_() const
                { return this->inputConfiguredParam_; }

        private:
            AccessLevel::Value accessLevel_;
            ArgumentCompleter* argumentCompleter_[5];
            ArgumentCompletionList argumentList_;

            KeybindMode::Value keybindMode_;
            int inputConfiguredParam_;
    };

    inline ConsoleCommand* createConsoleCommand(Functor* functor, const std::string& name = "")
    {
        return new ConsoleCommand(functor, name);
    }
}

#endif /* _ConsoleCommand_H__ */
