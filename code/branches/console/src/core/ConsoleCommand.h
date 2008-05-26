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
#include "ClassManager.h"
#include "Identifier.h"
#include "CommandExecutor.h"
#include "ArgumentCompletionFunctions.h"


#define SetConsoleCommand(classname, function,  bCreateShortcut) \
    SetConsoleCommandGeneric(classname##function##consolecommand__, classname, orxonox::createConsoleCommand(orxonox::createFunctor(&classname::function), #function), bCreateShortcut)

#define SetConsoleCommandGeneric(fakevariable, classname, command, bCreateShortcut) \
    ConsoleCommand& fakevariable = ClassManager<classname>::getIdentifier()->addConsoleCommand(command, bCreateShortcut)


#define SetConsoleCommandShortcut(classname, function) \
    SetConsoleCommandShortcutGeneric(function##consolecommand__, orxonox::createConsoleCommand(orxonox::createFunctor(&classname::function), #function))

#define SetConsoleCommandShortcutExtern(function) \
    SetConsoleCommandShortcutGeneric(function##consolecommand__, orxonox::createConsoleCommand(orxonox::createFunctor(&function), #function))

#define SetConsoleCommandShortcutGeneric(fakevariable, command) \
    ConsoleCommand& fakevariable = CommandExecutor::addConsoleCommandShortcut(command)


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

            inline ConsoleCommand& setDescription(const std::string& description)
                { this->ExecutorStatic::setDescription(description); return (*this); }
            inline ConsoleCommand& setDescriptionParam(int param, const std::string& description)
                { this->ExecutorStatic::setDescriptionParam(param, description); return (*this); }
            inline ConsoleCommand& setDescriptionReturnvalue(const std::string& description)
                { this->ExecutorStatic::setDescriptionReturnvalue(description); return (*this); }
            inline ConsoleCommand& setDefaultValues(const MultiTypeMath& param1)
                { this->ExecutorStatic::setDefaultValues(param1); return (*this); }
            inline ConsoleCommand& setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2)
                { this->ExecutorStatic::setDefaultValues(param1, param2); return (*this); }
            inline ConsoleCommand& setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3)
                { this->ExecutorStatic::setDefaultValues(param1, param2, param3); return (*this); }
            inline ConsoleCommand& setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4)
                { this->ExecutorStatic::setDefaultValues(param1, param2, param3, param4); return (*this); }
            inline ConsoleCommand& setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5)
                { this->ExecutorStatic::setDefaultValues(param1, param2, param3, param4, param5); return (*this); }
            inline ConsoleCommand& setDefaultValue(unsigned int index, const MultiTypeMath& param)
                { this->ExecutorStatic::setDefaultValue(index, param); return (*this); }

            inline ConsoleCommand& setAccessLevel(AccessLevel::Level level)
                { this->accessLevel_ = level; return (*this); }
            inline AccessLevel::Level getAccessLevel() const
                { return this->accessLevel_; }

            ConsoleCommand& setArgumentCompleter(unsigned int param, ArgumentCompleter* completer);
            ArgumentCompleter* getArgumentCompleter(unsigned int param) const;

            void createArgumentCompletionList(unsigned int param, const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "", const std::string& param5 = "");
            const std::list<std::pair<std::string, std::string> >& getArgumentCompletionList() const
                { return this->argumentList_; }
            std::list<std::pair<std::string, std::string> >::const_iterator getArgumentCompletionListBegin() const
                { return this->argumentList_.begin(); }
            std::list<std::pair<std::string, std::string> >::const_iterator getArgumentCompletionListEnd() const
                { return this->argumentList_.end(); }

        private:
            AccessLevel::Level accessLevel_;
            ArgumentCompleter* argumentCompleter_[5];
            std::list<std::pair<std::string, std::string> > argumentList_;
    };

    inline ConsoleCommand* createConsoleCommand(FunctorStatic* functor, const std::string& name = "")
    {
        return new ConsoleCommand(functor, name);
    }
}

#endif /* _ConsoleCommand_H__ */
