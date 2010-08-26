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

#include "core/CorePrereqs.h"

#include <stack>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include "util/VA_NARGS.h"
#include "ArgumentCompletionFunctions.h"
#include "Executor.h"


#define _SetConsoleCommand(...) \
    BOOST_PP_EXPAND(BOOST_PP_CAT(_SetConsoleCommand, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__))
#define _SetConsoleCommand2(name, functionpointer) \
    _SetConsoleCommandGeneric("", name, orxonox::createFunctor(functionpointer))
#define _SetConsoleCommand3(group, name, functionpointer) \
    _SetConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer))
#define _SetConsoleCommand4(group, name, functionpointer, object) \
    _SetConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer, object))

#define _SetConsoleCommandGeneric(group, name, functor) \
    static orxonox::_ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __LINE__) = (*orxonox::_createConsoleCommand(group, name, orxonox::createExecutor(functor)))


#define _DeclareConsoleCommand(...) \
    BOOST_PP_CAT(_DeclareConsoleCommand, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define _DeclareConsoleCommand2(name, functionpointer) \
    _DeclareConsoleCommandGeneric("", name, orxonox::createFunctor(functionpointer))
#define _DeclareConsoleCommand3(group, name, functionpointer) \
    _DeclareConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer))
#define _DeclareConsoleCommand4(group, name, functionpointer, object) \
    _DeclareConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer, object))

#define _DeclareConsoleCommandGeneric(group, name, functor) \
    static orxonox::_ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __LINE__) = (*orxonox::_createConsoleCommand(group, name, orxonox::createExecutor(functor), false))


namespace orxonox
{
    namespace prototype
    {
        inline void void__void(void) {}
        inline void void__string(const std::string&) {}
    }

    namespace AccessLevel
    {
        enum Enum
        {
            All,
            Standalone,
            Master,
            Server,
            Client,
            Online,
            Offline,
            None
        };
    }

    class _CoreExport _ConsoleCommand
    {
        friend struct _ConsoleCommandManipulator;

        struct Command
        {
            ExecutorPtr executor_;
            FunctorPtr functor_;
        };

        public:
            struct _ConsoleCommandManipulator
            {
                public:
                    _ConsoleCommandManipulator(const _ConsoleCommand* command) : command_(const_cast<_ConsoleCommand*>(command)) {}

                    template <class F>
                    inline _ConsoleCommandManipulator& setFunction(F function, bool bForce = false)
                        {
                            if (this->command_)
                            {
                                if (this->command_->getExecutor() && this->command_->getExecutor()->getFunctor() && this->command_->getExecutor()->getFunctor()->getFullIdentifier() == typeid(F))
                                {
                                    FunctorPointer<F>* functor = static_cast<FunctorPointer<F>*>(this->command_->getExecutor()->getFunctor().get());
                                    functor->setFunction(function);
                                    return *this;
                                }
                                this->command_->setFunction(createFunctor(function), bForce);
                            }
                            return *this;
                        }
                    template <class F, class O>
                    inline _ConsoleCommandManipulator& setFunction(F function, O* object, bool bForce = false)
                        {
                            if (this->command_)
                            {
                                if (this->command_->getExecutor() && this->command_->getExecutor()->getFunctor() && this->command_->getExecutor()->getFunctor()->getFullIdentifier() == typeid(F))
                                {
                                    FunctorPointer<F, O>* functor = static_cast<FunctorPointer<F, O>*>(this->command_->getExecutor()->getFunctor().get());
                                    functor->setFunction(function);
                                    functor->setObject(object);
                                    return *this;
                                }
                                this->command_->setFunction(createFunctor(function, object), bForce);
                            }
                            return *this;
                        }
                    inline _ConsoleCommandManipulator& setFunction(const FunctorPtr& functor, bool bForce = false)
                        { if (this->command_) { this->command_->setFunction(functor, bForce); } return *this; }
                    inline _ConsoleCommandManipulator& setFunction(const ExecutorPtr& executor, bool bForce = false)
                        { if (this->command_) { this->command_->setFunction(executor, bForce); } return *this; }

                    template <class F>
                    inline _ConsoleCommandManipulator& pushFunction(F function, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(createFunctor(function), bForce); } return *this; }
                    template <class F, class O>
                    inline _ConsoleCommandManipulator& pushFunction(F function, O* object, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(createFunctor(function, object), bForce); } return *this; }
                    inline _ConsoleCommandManipulator& pushFunction(const FunctorPtr& functor, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(functor, bForce); } return *this; }
                    inline _ConsoleCommandManipulator& pushFunction(const ExecutorPtr& executor, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(executor, bForce); } return *this; }

                    inline _ConsoleCommandManipulator& popFunction()
                        { if (this->command_) { this->command_->popFunction(); } return *this; }

                    inline _ConsoleCommandManipulator& resetFunction()
                        { if (this->command_) { this->command_->resetFunction(); } return *this; }

                    inline _ConsoleCommandManipulator& setObject(void* object)
                        { if (this->command_) { this->command_->setObject(object); } return *this; }
                    inline _ConsoleCommandManipulator& pushObject(void* object)
                        { if (this->command_) { this->command_->pushObject(object); } return *this; }
                    inline _ConsoleCommandManipulator& popObject()
                        { if (this->command_) { this->command_->popObject(); } return *this; }

                    inline _ConsoleCommandManipulator& setActive(bool bActive)
                        { if (this->command_) { this->command_->setActive(bActive); } return *this; }
                    inline _ConsoleCommandManipulator& activate()
                        { return this->setActive(true); }
                    inline _ConsoleCommandManipulator& deactivate()
                        { return this->setActive(false); }

                    inline _ConsoleCommandManipulator& setHidden(bool bHidden)
                        { if (this->command_) { this->command_->setHidden(bHidden); } return *this; }
                    inline _ConsoleCommandManipulator& hide()
                        { return this->setHidden(true); }
                    inline _ConsoleCommandManipulator& show()
                        { return this->setHidden(false); }

                    inline _ConsoleCommandManipulator& defaultValues(const MultiType& param1)
                        { if (this->command_) { this->command_->defaultValues(param1); } return *this; }
                    inline _ConsoleCommandManipulator& defaultValues(const MultiType& param1, const MultiType& param2)
                        { if (this->command_) { this->command_->defaultValues(param1, param2); } return *this; }
                    inline _ConsoleCommandManipulator& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
                        { if (this->command_) { this->command_->defaultValues(param1, param2, param3); } return *this; }
                    inline _ConsoleCommandManipulator& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
                        { if (this->command_) { this->command_->defaultValues(param1, param2, param3, param4); } return *this; }
                    inline _ConsoleCommandManipulator& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
                        { if (this->command_) { this->command_->defaultValues(param1, param2, param3, param4, param5); } return *this; }
                    inline _ConsoleCommandManipulator& defaultValue(unsigned int index, const MultiType& param)
                        { if (this->command_) { this->command_->defaultValue(index, param); } return *this; }

                    inline _ConsoleCommandManipulator& accessLevel(AccessLevel::Enum level)
                        { if (this->command_) { this->command_->accessLevel(level); } return *this; }

                    inline _ConsoleCommandManipulator& argumentCompleter(unsigned int param, ArgumentCompleter* completer)
                        { if (this->command_) { this->command_->argumentCompleter(param, completer); } return *this; }

                    inline _ConsoleCommandManipulator& setAsInputCommand()
                        { if (this->command_) { this->command_->setAsInputCommand(); } return *this; }
                    inline _ConsoleCommandManipulator& keybindMode(KeybindMode::Value mode)
                        { if (this->command_) { this->command_->keybindMode(mode); } return *this; }
                    inline _ConsoleCommandManipulator& inputConfiguredParam(int index)
                        { if (this->command_) { this->command_->inputConfiguredParam(index); } return *this; }

                private:
                    _ConsoleCommand* command_;
            };

        public:
            _ConsoleCommand(const std::string& group, const std::string& name, const ExecutorPtr& executor, bool bInitialized = true);
            ~_ConsoleCommand();

            _ConsoleCommand& addShortcut();
            _ConsoleCommand& addShortcut(const std::string&  name);
            _ConsoleCommand& addGroup(const std::string& group);
            _ConsoleCommand& addGroup(const std::string& group, const std::string&  name);

            inline const std::string& getName() const
                { return this->baseName_; }

            const ExecutorPtr& getExecutor() const;
            inline const ExecutorPtr& getBaseExecutor() const
                { return this->baseExecutor_; }

            inline _ConsoleCommand& setActive(bool bActive)
                { this->bActive_ = bActive; return *this; }
            inline _ConsoleCommand& activate()
                { return this->setActive(true); }
            inline _ConsoleCommand& deactivate()
                { return this->setActive(false); }

            inline _ConsoleCommand& setHidden(bool bHidden)
                { this->bHidden_ = bHidden; return *this; }
            inline _ConsoleCommand& hide()
                { return this->setHidden(true); }
            inline _ConsoleCommand& show()
                { return this->setHidden(false); }

            bool isActive() const;
            bool hasAccess() const;
            bool isHidden() const;

            _ConsoleCommand& description(const std::string& description);
            const std::string& getDescription() const;

            _ConsoleCommand& descriptionParam(unsigned int param, const std::string& description);
            const std::string& getDescriptionParam(unsigned int param) const;

            _ConsoleCommand& descriptionReturnvalue(const std::string& description);
            const std::string& getDescriptionReturnvalue(int param) const;

            _ConsoleCommand& defaultValues(const MultiType& param1);
            _ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2);
            _ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3);
            _ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4);
            _ConsoleCommand& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5);
            _ConsoleCommand& defaultValue(unsigned int index, const MultiType& param);

            inline _ConsoleCommand& accessLevel(AccessLevel::Enum level)
                { this->accessLevel_ = level; return *this; }
            inline AccessLevel::Enum getAccessLevel() const
                { return this->accessLevel_; }

            _ConsoleCommand& argumentCompleter(unsigned int param, ArgumentCompleter* completer);
            ArgumentCompleter* getArgumentCompleter(unsigned int param) const;

            inline _ConsoleCommand& setAsInputCommand()
            {
                this->keybindMode(KeybindMode::OnHold);
                this->defaultValue(0, Vector2(0.0f, 0.0f));
                this->inputConfiguredParam(0);
                return *this;
            }

            inline _ConsoleCommand& keybindMode(KeybindMode::Value mode)
                { this->keybindMode_ = mode; return *this; }
            inline KeybindMode::Value getKeybindMode() const
                { return this->keybindMode_; }

            inline _ConsoleCommand& inputConfiguredParam(int index)
                { this->inputConfiguredParam_ = index; return *this; }
            inline int getInputConfiguredParam_() const
                { return this->inputConfiguredParam_; }

            inline _ConsoleCommandManipulator getManipulator() const
                { return this; }

        private:
            bool headersMatch(const FunctorPtr& functor);
            bool headersMatch(const ExecutorPtr& executor);

            bool setFunction(const ExecutorPtr& executor, bool bForce = false);
            bool setFunction(const FunctorPtr& functor, bool bForce = false);
            void pushFunction(const ExecutorPtr& executor, bool bForce = false);
            void pushFunction(const FunctorPtr& functor, bool bForce = false);
            void pushFunction();
            void popFunction();
            void resetFunction();

            bool setObject(void* object);
            void pushObject(void* object);
            void popObject();
            void* getObject() const;

            bool bActive_;
            bool bHidden_;
            AccessLevel::Enum accessLevel_;
            std::string baseName_;
            ExecutorPtr baseExecutor_;

            ExecutorPtr executor_;
            std::stack<Command> commandStack_;
            std::stack<void*> objectStack_;

            ArgumentCompleter* argumentCompleter_[5];

            KeybindMode::Value keybindMode_;
            int inputConfiguredParam_;

            LanguageEntryLabel description_;
            LanguageEntryLabel descriptionReturnvalue_;
            LanguageEntryLabel descriptionParam_[MAX_FUNCTOR_ARGUMENTS];

        public:
            static inline const std::map<std::string, std::map<std::string, _ConsoleCommand*> >& getCommands()
                { return _ConsoleCommand::getCommandMap(); }

            static inline const _ConsoleCommand* getCommand(const std::string& name, bool bPrintError = false)
                { return _ConsoleCommand::getCommand("", name, bPrintError); }
            static const _ConsoleCommand* getCommand(const std::string& group, const std::string& name, bool bPrintError = false);

            static void destroyAll();

        private:
            static std::map<std::string, std::map<std::string, _ConsoleCommand*> >& getCommandMap();
            static void registerCommand(const std::string& group, const std::string& name, _ConsoleCommand* command);
            static void unregisterCommand(_ConsoleCommand* command);
    };

    inline _ConsoleCommand* _createConsoleCommand(const std::string& name, const ExecutorPtr& executor, bool bInitialized = true)
        { return new _ConsoleCommand("", name, executor, bInitialized); }
    inline _ConsoleCommand* _createConsoleCommand(const std::string& group, const std::string& name, const ExecutorPtr& executor, bool bInitialized = true)
        { return new _ConsoleCommand(group, name, executor, bInitialized); }

    inline _ConsoleCommand::_ConsoleCommandManipulator _ModifyConsoleCommand(const std::string& name)
        { return _ConsoleCommand::getCommand(name, true); }
    inline _ConsoleCommand::_ConsoleCommandManipulator _ModifyConsoleCommand(const std::string& group, const std::string& name)
        { return _ConsoleCommand::getCommand(group, name, true); }
}

#endif /* _ConsoleCommand_H__ */
