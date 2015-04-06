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
    @ingroup Command ConsoleCommand
    @brief Declaration of the orxonox::ConsoleCommand class.
*/

#ifndef _ConsoleCommand_H__
#define _ConsoleCommand_H__

#include "core/CorePrereqs.h"

#include <stack>
#include <vector>

#include "ArgumentCompletionFunctions.h"
#include "Executor.h"

namespace orxonox
{
    /**
        @brief A small collection of functions that can be used in DeclareConsoleCommand() if
        you don't want to use the real function-pointer.
    */
    namespace prototype
    {
        inline void void__void(void) {}
        inline void void__string(const std::string&) {}

        inline std::string string__bool(bool) { return ""; }
        inline std::string string__string(const std::string&) { return ""; }
        inline std::string string__uint_uint_bool(unsigned int, unsigned int, bool) { return ""; }
    }

    namespace AccessLevel
    {
        /**
            @brief Possible access levels: A command can only be executed if the program is in the state which is requested by the access level.
        */
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

    /**
        @brief The ConsoleCommand class stores all information about a console command which can be executed by CommandExecutor.

        Console commands can be entered by the user into the shell, called in scripts, or
        used for key-bindings. They are simple text strings that can be executed by
        CommandExecutor. CommandExecutor will search for a ConsoleCommand with the given
        group and name and will execute it's Executor (which again calls the Functor and
        this finally calls the command-function).

        @see See @ref ConsoleCommandExample "ConsoleCommand.h" for more information and some examples.
    */
    class _CoreExport ConsoleCommand
    {
        friend struct ConsoleCommandManipulator;

        /**
            @brief Helper class that is used to put the current state of the ConsoleCommand on a stack.
        */
        struct Command
        {
            ExecutorPtr executor_;              ///< The executor
            FunctorPtr functor_;                ///< The function that is used with the executor - has to be stored separatley because the executor is often used with different functors
            std::vector<void*> objectStack_;    ///< The object stack
        };

        public:
            /**
                @brief Helper class that is used to manipulate console commands.

                An instance of this class is returned if you call the ModifyConsoleCommand macro.
                This class provides an interface which wraps some functions of ConsoleCommand. It
                allows access to some private functions like setFunction() (that can't be called
                right after SetConsoleCommand()) but it also hides some functions that shouln't be
                called after the static declaration like addShortcut() or description().

                @see See @ref ConsoleCommandExample "ConsoleCommand.h" for more information and examples.
            */
            struct ConsoleCommandManipulator
            {
                public:
                    /// Constructor: Creates a manipulator for a given ConsoleCommand.
                    ConsoleCommandManipulator(ConsoleCommand* command) : command_(command) {}

                    /// Changes the current function of the command. @param function The new function-pointer @param bForce If true, the new function-pointer is always assigned, even if the headers don't match
                    template <class F>
                    inline ConsoleCommandManipulator& setFunction(F function, bool bForce = false)
                        {
                            if (this->command_)
                            {
                                // check if the headers match. If they do, only change the function-pointer of the current Functor instead of creating a new Functor
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
                    /// Changes the current function of the command. @param function The new function-pointer @param object The new object-pointer (for member-functions) @param bForce If true, the new function-pointer is always assigned, even if the headers don't match
                    template <class F, class O>
                    inline ConsoleCommandManipulator& setFunction(F function, O* object, bool bForce = false)
                        {
                            if (this->command_)
                            {
                                // check if the headers match. If they do, only change the function-pointer of the current Functor instead of creating a new Functor
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
                    /// Changes the current Functor of the command. @param functor The new Functor @param bForce If true, the new Functor is always assigned, even if the headers don't match
                    inline ConsoleCommandManipulator& setFunction(const FunctorPtr& functor, bool bForce = false)
                        { if (this->command_) { this->command_->setFunction(functor, bForce); } return *this; }
                    /// Changes the current Executor of the command. @param executor The new Executor @param bForce If true, the new Executor is always assigned, even if the headers don't match
                    inline ConsoleCommandManipulator& setFunction(const ExecutorPtr& executor, bool bForce = false)
                        { if (this->command_) { this->command_->setFunction(executor, bForce); } return *this; }

                    /// Pushes a copy of the current Executor on the command-stack, that can be altered without changing the old Executor. @details This function is especially useful if you don't wan't to change the function, but only the default values of the executor.
                    inline ConsoleCommandManipulator& pushFunction()
                        { if (this->command_) { this->command_->pushFunction(); } return *this; }
                    /// Pushes a new function on the command-stack. @param function The new function-pointer @param bForce If true, the new function-pointer is always assigned, even if the headers don't match
                    template <class F>
                    inline ConsoleCommandManipulator& pushFunction(F function, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(createFunctor(function), bForce); } return *this; }
                    /// Pushes a new function on the command-stack. @param function The new function-pointer @param object The new object-pointer (for member-functions) @param bForce If true, the new function-pointer is always assigned, even if the headers don't match
                    template <class F, class O>
                    inline ConsoleCommandManipulator& pushFunction(F function, O* object, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(createFunctor(function, object), bForce); } return *this; }
                    /// Pushes a new Functor on the command-stack. @param functor The new Functor @param bForce If true, the new Functor is always assigned, even if the headers don't match
                    inline ConsoleCommandManipulator& pushFunction(const FunctorPtr& functor, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(functor, bForce); } return *this; }
                    /// Pushes a new Executor on the command-stack. @param executor The new Executor @param bForce If true, the new Executor is always assigned, even if the headers don't match
                    inline ConsoleCommandManipulator& pushFunction(const ExecutorPtr& executor, bool bForce = false)
                        { if (this->command_) { this->command_->pushFunction(executor, bForce); } return *this; }

                    /// Removes the current function from the stack and restores the old state. If there's no other function on the stack, the command is deactivated.
                    inline ConsoleCommandManipulator& popFunction()
                        { if (this->command_) { this->command_->popFunction(); } return *this; }

                    /// Sets the current function-pointer to NULL, which also deactivates the command.
                    inline ConsoleCommandManipulator& resetFunction()
                        { if (this->command_) { this->command_->resetFunction(); } return *this; }

                    /// Changes the current object (used for member-functions).
                    inline ConsoleCommandManipulator& setObject(void* object)
                        { if (this->command_) { this->command_->setObject(object); } return *this; }
                    /// Pushes a new object on the object-stack.
                    inline ConsoleCommandManipulator& pushObject(void* object)
                        { if (this->command_) { this->command_->pushObject(object); } return *this; }
                    /// Removes the current object from the object-stack and restores the old object (or NULL if there's no object left on the stack).
                    inline ConsoleCommandManipulator& popObject()
                        { if (this->command_) { this->command_->popObject(); } return *this; }

                    /// Changes the activity of the command.
                    inline ConsoleCommandManipulator& setActive(bool bActive)
                        { if (this->command_) { this->command_->setActive(bActive); } return *this; }
                    /// Activates the command.
                    inline ConsoleCommandManipulator& activate()
                        { return this->setActive(true); }
                    /// Deactivates the command.
                    inline ConsoleCommandManipulator& deactivate()
                        { return this->setActive(false); }

                    /// Changes the visibility of the command.
                    inline ConsoleCommandManipulator& setHidden(bool bHidden)
                        { if (this->command_) { this->command_->setHidden(bHidden); } return *this; }
                    /// Hides the command (can still be executed, but is not visible in the list of available commands).
                    inline ConsoleCommandManipulator& hide()
                        { return this->setHidden(true); }
                    /// Makes the command visible.
                    inline ConsoleCommandManipulator& show()
                        { return this->setHidden(false); }

                    /// Changes the default values of the current executor (doesn't modify executors on deeper levels of the command-stack).
                    inline ConsoleCommandManipulator& defaultValues(const MultiType& arg1)
                        { if (this->command_) { this->command_->defaultValues(arg1); } return *this; }
                    /// Changes the default values of the current executor (doesn't modify executors on deeper levels of the command-stack).
                    inline ConsoleCommandManipulator& defaultValues(const MultiType& arg1, const MultiType& arg2)
                        { if (this->command_) { this->command_->defaultValues(arg1, arg2); } return *this; }
                    /// Changes the default values of the current executor (doesn't modify executors on deeper levels of the command-stack).
                    inline ConsoleCommandManipulator& defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3)
                        { if (this->command_) { this->command_->defaultValues(arg1, arg2, arg3); } return *this; }
                    /// Changes the default values of the current executor (doesn't modify executors on deeper levels of the command-stack).
                    inline ConsoleCommandManipulator& defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4)
                        { if (this->command_) { this->command_->defaultValues(arg1, arg2, arg3, arg4); } return *this; }
                    /// Changes the default values of the current executor (doesn't modify executors on deeper levels of the command-stack).
                    inline ConsoleCommandManipulator& defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5)
                        { if (this->command_) { this->command_->defaultValues(arg1, arg2, arg3, arg4, arg5); } return *this; }
                    /// Changes the default value of the argument with given index of the current executor (doesn't modify executors on deeper levels of the command-stack).
                    inline ConsoleCommandManipulator& defaultValue(unsigned int index, const MultiType& arg)
                        { if (this->command_) { this->command_->defaultValue(index, arg); } return *this; }

                    /// Changes the access level of the command.
                    inline ConsoleCommandManipulator& accessLevel(AccessLevel::Enum level)
                        { if (this->command_) { this->command_->accessLevel(level); } return *this; }

                    /// Changes the argument completer for the given parameter.
                    inline ConsoleCommandManipulator& argumentCompleter(unsigned int index, ArgumentCompleter* completer)
                        { if (this->command_) { this->command_->argumentCompleter(index, completer); } return *this; }

                    /// Defines the command to be an input command.
                    inline ConsoleCommandManipulator& setAsInputCommand()
                        { if (this->command_) { this->command_->setAsInputCommand(); } return *this; }
                    /// Changes the keybind mode of the command.
                    inline ConsoleCommandManipulator& keybindMode(KeybindMode::Value mode)
                        { if (this->command_) { this->command_->changeKeybindMode(mode); } return *this; }
                    /// Sets the input configured param to the given index.
                    inline ConsoleCommandManipulator& inputConfiguredParam(int index)
                        { if (this->command_) { this->command_->inputConfiguredParam(index); } return *this; }

                private:
                    ConsoleCommand* command_;   ///< The command which is being manipulated by this object
            };

        public:
            ConsoleCommand(const std::string& group, const std::string& name, const ExecutorPtr& executor, bool bInitialized = true);
            ~ConsoleCommand();

            ConsoleCommand& addShortcut();
            ConsoleCommand& addShortcut(const std::string&  name);
            ConsoleCommand& addGroup(const std::string& group);
            ConsoleCommand& addGroup(const std::string& group, const std::string&  name);

            /// Returns the name that was first used for this command.
            inline const std::string& getName() const
                { return this->baseName_; }

            const ExecutorPtr& getExecutor() const;
            /// Returns the functor that defines the required header for this command (but isn't necessarily executed).
            inline const FunctorPtr& getBaseFunctor() const
                { return this->baseFunctor_; }

            /// Changes the activity of the command.
            inline ConsoleCommand& setActive(bool bActive)
                { this->bActive_ = bActive; return *this; }
            /// Activates the command.
            inline ConsoleCommand& activate()
                { return this->setActive(true); }
            /// Deactivates the command.
            inline ConsoleCommand& deactivate()
                { return this->setActive(false); }

            /// Changes the visibility of the command.
            inline ConsoleCommand& setHidden(bool bHidden)
                { this->bHidden_ = bHidden; return *this; }
            /// Hides the command (can still be executed, but is not visible in the list of available commands).
            inline ConsoleCommand& hide()
                { return this->setHidden(true); }
            /// Makes the command visible.
            inline ConsoleCommand& show()
                { return this->setHidden(false); }

            bool isActive() const;
            bool hasAccess() const;
            /// Returns true if the command is currently hidden.
            inline bool isHidden() const
                { return this->bHidden_; }

            ConsoleCommand& description(const std::string& description);
            const std::string& getDescription() const;

            ConsoleCommand& descriptionParam(unsigned int index, const std::string& description);
            const std::string& getDescriptionParam(unsigned int index) const;

            ConsoleCommand& descriptionReturnvalue(const std::string& description);
            const std::string& getDescriptionReturnvalue(int index) const;

            ConsoleCommand& defaultValues(const MultiType& arg1);
            ConsoleCommand& defaultValues(const MultiType& arg1, const MultiType& arg2);
            ConsoleCommand& defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3);
            ConsoleCommand& defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4);
            ConsoleCommand& defaultValues(const MultiType& arg1, const MultiType& arg2, const MultiType& arg3, const MultiType& arg4, const MultiType& arg5);
            ConsoleCommand& defaultValue(unsigned int index, const MultiType& arg);

            /// Changes the access level of the command.
            inline ConsoleCommand& accessLevel(AccessLevel::Enum level)
                { this->accessLevel_ = level; return *this; }
            /// Returns the access level of the command.
            inline AccessLevel::Enum getAccessLevel() const
                { return this->accessLevel_; }

            ConsoleCommand& argumentCompleter(unsigned int index, ArgumentCompleter* completer);
            ArgumentCompleter* getArgumentCompleter(unsigned int index) const;

            /// Defines the command to be an input command
            inline ConsoleCommand& setAsInputCommand()
            {
                this->keybindMode(KeybindMode::OnHold);
                this->defaultValue(0, Vector2(0.0f, 0.0f));
                this->inputConfiguredParam(0);
                return *this;
            }

            /// Sets the keybind mode. Note: use changeKeybindMode if you intend to change the mode.
            inline ConsoleCommand& keybindMode(KeybindMode::Value mode)
                { this->keybindMode_ = mode; return *this; }
            /// Returns the keybind mode
            inline KeybindMode::Value getKeybindMode() const
                { return this->keybindMode_; }

            ConsoleCommand& changeKeybindMode(KeybindMode::Value mode);

            /// Changes the input configured param to the given index.
            inline ConsoleCommand& inputConfiguredParam(int index)
                { this->inputConfiguredParam_ = index; return *this; }
            /// Returns the input configured param.
            inline int getInputConfiguredParam() const
                { return this->inputConfiguredParam_; }

            /// Returns a manipulator for this command.
            inline ConsoleCommandManipulator getManipulator()
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

            bool bActive_;                                                  ///< True if the command should be active (it can still be inactive, for example if the function is missing)
            bool bHidden_;                                                  ///< True if the command is hidden (it is still executable, but not visible in the list of available commands)
            AccessLevel::Enum accessLevel_;                                 ///< The access level (the state of the game in which you can access the command)
            std::string baseName_;                                          ///< The name that was first assigned to the command
            FunctorPtr baseFunctor_;                                        ///< The functor that defines the header of the command-function

            ExecutorPtr executor_;                                          ///< The Executor that is used to execute the command
            std::stack<Command> commandStack_;                              ///< A stack of commands, used to push and pop different functions
            std::vector<void*> objectStack_;                                ///< A stack of objects, used to push and pop different objects for a function

            ArgumentCompleter* argumentCompleter_[MAX_FUNCTOR_ARGUMENTS];   ///< ArgumentCompleter for each argument

            KeybindMode::Value keybindMode_;                                ///< The keybind mode
            int inputConfiguredParam_;                                      ///< The input configured param

            LanguageEntryLabel description_;                                ///< The description of the command
            LanguageEntryLabel descriptionReturnvalue_;                     ///< A description of the return-value
            LanguageEntryLabel descriptionParam_[MAX_FUNCTOR_ARGUMENTS];    ///< A description for each argument
    };
}

#endif /* _ConsoleCommand_H__ */
