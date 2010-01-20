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

#include "util/VA_NARGS.h"
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


#define _SetConsoleCommand(...) \
    BOOST_PP_CAT(_SetConsoleCommand, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define _SetConsoleCommand2(name, functionpointer) \
    _SetConsoleCommandGeneric("", name, orxonox::createFunctor(functionpointer))
#define _SetConsoleCommand3(group, name, functionpointer) \
    _SetConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer))
#define _SetConsoleCommand4(group, name, functionpointer, object) \
    _SetConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer, object))

#define _SetConsoleCommandGeneric(group, name, functor) \
    orxonox::_ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __LINE__) = (*orxonox::_createConsoleCommand(group, name, functor))


#define _DeclareConsoleCommand(...) \
    BOOST_PP_CAT(_DeclareConsoleCommand, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define _DeclareConsoleCommand2(name, functionpointer) \
    _DeclareConsoleCommandGeneric("", name, functionpointer)
#define _DeclareConsoleCommand3(group, name, functionpointer) \
    _DeclareConsoleCommandGeneric(group, name, functionpointer)

#define _DeclareConsoleCommandGeneric(group, name, functionpointer) \
    orxonox::_ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __LINE__) = (*orxonox::_createConsoleCommand(group, name, orxonox::createFunctor(functionpointer), orxonox::_ConsoleCommand::State::UninitializedActive))


#define _ModifyConsoleCommand(...) \
    orxonox::_ConsoleCommand::getCommand(__VA_ARGS__, true)->getManipulator()


namespace orxonox
{
    class _CoreExport _ConsoleCommand : protected Executor
    {
        friend struct _ConsoleCommandManipulator;

        public:
            struct State
            {
                enum Enum
                {
                    UninitializedActive,
                    UninitializedInactive,
                    Active,
                    Inactive
                };
            };

            struct ObjectPointer
            {
                enum Enum
                {
                    Null,
                    RawCopy,
                    CastViaBaseObject
                };
            };

            struct _ConsoleCommandManipulator
            {
                public:
                    _ConsoleCommandManipulator(const _ConsoleCommand* command) : command_(const_cast<_ConsoleCommand*>(command)) {}

                    template <class F>
                    inline _ConsoleCommandManipulator& setFunction(F function, _ConsoleCommand::ObjectPointer::Enum mode = _ConsoleCommand::ObjectPointer::Null)
                        { if (this->command_) { this->command_->setFunctor(createFunctor(function), mode); } return *this; }
                    template <class F, class O>
                    inline _ConsoleCommandManipulator& setFunction(F function, O* object)
                        { if (this->command_) { this->command_->setFunctor(createFunctor(function, object)); } return *this; }
                    inline _ConsoleCommandManipulator& setFunction(Functor* functor)
                        { if (this->command_) { this->command_->setFunctor(functor); } return *this; }
                    inline _ConsoleCommandManipulator& setFunction(const _ConsoleCommand* command)
                        { if (this->command_) { this->command_->setFunctor(command->functor_); } return *this; }
                    inline _ConsoleCommandManipulator& setFunction(const _ConsoleCommandManipulator& manipulator)
                        { if (this->command_) { this->command_->setFunctor(manipulator.command_->functor_); } return *this; }

                    template <class F>
                    inline _ConsoleCommandManipulator& pushFunction(F function, _ConsoleCommand::ObjectPointer::Enum mode = _ConsoleCommand::ObjectPointer::Null)
                        { if (this->command_) { this->command_->pushFunctor(createFunctor(function), mode); } return *this; }
                    template <class F, class O>
                    inline _ConsoleCommandManipulator& pushFunction(F function, O* object)
                        { if (this->command_) { this->command_->pushFunctor(createFunctor(function, object)); } return *this; }
                    inline _ConsoleCommandManipulator& pushFunction(Functor* functor)
                        { if (this->command_) { this->command_->pushFunctor(functor); } return *this; }
                    inline _ConsoleCommandManipulator& pushFunction(const _ConsoleCommand* command)
                        { if (this->command_) { this->command_->pushFunctor(command->functor_); } return *this; }
                    inline _ConsoleCommandManipulator& pushFunction(const _ConsoleCommandManipulator& manipulator)
                        { if (this->command_) { this->command_->pushFunctor(manipulator.command_->functor_); } return *this; }

                    inline _ConsoleCommandManipulator& popFunction()
                        { if (this->command_) { this->command_->popFunctor(); } return *this; }

                    inline _ConsoleCommandManipulator& setObject(void* object)
                        { if (this->command_) { this->command_->setObject(object); } return *this; }
                    inline _ConsoleCommandManipulator& setObject(BaseObject* object)
                        { if (this->command_) { this->command_->setObject(object); } return *this; }

                    inline _ConsoleCommandManipulator& setActive(bool bActive)
                        { if (this->command_) { this->command_->setActive(bActive); } return *this; }

                private:
                    _ConsoleCommand* command_;
            };

        public:
            _ConsoleCommand(const std::string& group, const std::string& name, Functor* functor, State::Enum state = State::Active);

            _ConsoleCommand& addShortcut();
            _ConsoleCommand& addShortcut(const std::string&  name);
            _ConsoleCommand& addGroup(const std::string& group);
            _ConsoleCommand& addGroup(const std::string& group, const std::string&  name);

            void setActive(bool bActive);
            inline State::Enum getState() const
                { return this->state_; }
            inline bool isActive() const
                { return (this->state_ == State::Active); }

            static inline const std::map<std::string, std::map<std::string, _ConsoleCommand*> >& getCommands()
                { return _ConsoleCommand::getCommandMap(); }

            static inline const _ConsoleCommand* getCommand(const std::string& name, bool bPrintError = false)
                { return _ConsoleCommand::getCommand("", name, bPrintError); }
            static const _ConsoleCommand* getCommand(const std::string& group, const std::string& name, bool bPrintError = false);

            inline _ConsoleCommandManipulator getManipulator() const
                { return this; }

        private:
            static std::map<std::string, std::map<std::string, _ConsoleCommand*> >& getCommandMap();
            static void registerCommand(const std::string& group, const std::string& name, _ConsoleCommand* command);

            void setInitialized(bool bInitialized);

            void setFunctor(Functor* functor, _ConsoleCommand::ObjectPointer::Enum mode = _ConsoleCommand::ObjectPointer::Null);
            void pushFunctor(Functor* functor, _ConsoleCommand::ObjectPointer::Enum mode = _ConsoleCommand::ObjectPointer::Null);
            void popFunctor();
            bool functionHeaderMatches(Functor* functor) const;

            void setObject(void* object);
            void setObject(BaseObject* object);

            State::Enum state_;
            const std::type_info& functionHeader_;
    };

    inline _ConsoleCommand* _createConsoleCommand(const std::string& name, Functor* functor, _ConsoleCommand::State::Enum state = _ConsoleCommand::State::Active)
    {
        return new _ConsoleCommand("", name, functor, state);
    }

    inline _ConsoleCommand* _createConsoleCommand(const std::string& group, const std::string& name, Functor* functor, _ConsoleCommand::State::Enum state = _ConsoleCommand::State::Active)
    {
        return new _ConsoleCommand(group, name, functor, state);
    }
}

#endif /* _ConsoleCommand_H__ */
