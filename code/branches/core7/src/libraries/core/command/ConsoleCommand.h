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

/**
    @file
    @ingroup Command ConsoleCommand
    @brief Declaration of the orxonox::ConsoleCommand class and the SetConsoleCommand() macro.

    @anchor ConsoleCommandExample

    Console commands can be used to write scripts, use key-bindings or simply to be
    entered into the shell by the user. Instances of orxonox::ConsoleCommand define
    the function of a command, and also more information like, for example, if it is
    active, default values, and possible arguments.

    Commands need to be registered to the system statically on startup by using the
    SetConsoleCommand() or DeclareConsoleCommand() macros outside of a function.
    This ensures that commands are known to the system at any time, so they can be
    evaluated (see orxonox::CommandExecutor::evaluate()), for example for key-bindings.

    Example:
    @code
    void myCoutFunction(const std::string& text)        // Define a static function
    {
        orxout() << "Text: " << text << endl;           // Print the text to the console
    }

    SetConsoleCommand("cout", &myCoutFunction);         // Register the function as command with name "cout"
    @endcode

    Now you can open the shell and execute the command:
    @code
    $ cout Hello World
    @endcode

    Internally this command is now passed to orxonox::CommandExecutor::execute():
    @code
    CommandExecutor::execute("cout HelloWorld");
    @endcode

    CommandExecutor searches for a command with name "cout" and passes the arguments
    "Hello World" to it. Because we registered myCoutFunction() with this command,
    as a result the following text will be printed to the console:
    @code
    Text: Hello World
    @endcode

    You can add more attributes to the ConsoleCommand, by using the command-chain feature
    of SetConsoleCommand(). For example like this:
    @code
    SetConsoleCommand("cout", &myCoutFunction)
        .addGroup("output", "text")
        .accessLevel(AccessLevel::Offline)
        .defaultValues("no text");
    @endcode

    Open the shell again and try it:
    @code
    $ cout Hello World
    Text: Hello World
    $ output text Hello World
    Text: Hello World
    $ cout
    Text: no text
    @endcode

    If you execute it online (note: the access level is "Offline"), you will see the
    following (or something similar):
    @code
    $ cout Hello World
    Error: Can't execute command "cout", access denied.
    @endcode

    If a command is executed, the arguments are passed to an underlying function,
    whitch is wrapped by an orxonox::Functor which again is wrapped by an orxonox::Executor.
    The Functor contains the function-pointer, as well as the object-pointer in
    case of a non-static member-function. The executor stores possible default-values
    for each argument of the function.

    The function of a command can be changed at any time. It's possible to just exchange
    the function-pointer of the underlying Functor if the headers of the functions are
    exactly the same. But you can also exchange the Functor itself or even completely
    replace the Executor. Also the other attributes of a ConsoleCommand can be modified
    during the game, for example it can be activated or deactivated.

    To do so, the function ModifyConsoleCommand() has to be used. It returns an instance
    of orxonox::ConsoleCommand::ConsoleCommandManipulator which has an interface similar to
    orxonox::ConsoleCommand, but with slight differences. You can use it the same way like
    SetConsoleCommand(), meaning you can use command-chains to change different attributes at
    the same time. ModifyConsoleCommand() must not be executed statically, but rather in a
    function at some point of the execution of the program.

    Example:
    @code
    void myOtherCoutFunction(const std::string& text)                       // Define a new static function
    {
        orxout() << "Uppercase: " << getUppercase(text) << endl;            // Print the text in uppercase to the console
    }

    {
        // ...                                                              // somewhere in the code

        ModifyConsoleCommand("cout").setFunction(&myOtherCoutFunction);     // Modify the underlying function of the command

        // ...
    }
    @endcode

    If you now enter the command into the shell, you'll see a different behavior:
    @code
    $ cout Hello World
    Uppercase: HELLO WORLD
    $ cout
    Uppercase: NO TEXT
    @endcode

    A few important notes about changing functions:

    Instead of changing the function with setFunction(), you can also create a command-stack
    by using pushFunction() and popFunction(). It's important to note a few things about that,
    because the underlying structure of Executor and Functor has a few pitfalls:
     - If you push a new function-pointer, the same executor as before will be used (and, if
       the headers match, even the same functor can be used, which is very fast)
     - If you push a new Functor, the same executor as before will be used
     - If you push a new Executor, everything is changed

    Note that the executor contains the @b default @b values, so if you just exchange the
    Functor, the default values remain the same. However if you decide to change the default
    values at any point of the stack, <b>this will also change the default values on all
    other stack-levels</b> that share the same executor. If you don't like this behavior,
    you have to explicitly push a new executor before changing the default values, either by
    calling pushFunction(executor) or by calling pushFunction(void) which pushes a copy of
    the current executor to the stack.

    Another important point are object pointers in case of non-static member-functions.
    Whenever you set or push a new function, <b>you must add the object pointer again</b>
    because objects are stored in the Functor which is usually exchanged if you change
    the function.

    You can also use a stack for objects, but note that this <b>object-stack is different for each
    function</b> - so if you set a new function, the object-stack will be cleared. If you push
    a new function, the old object-stack is stored in the stack, so it can be restored if
    you pop the function.

    %DeclareConsoleCommand():

    Appart from SetConsoleCommand() you can also call DeclareConsoleCommand(). In contrast
    to SetConsoleCommand(), this doesn't assign a function to the command. Indeed you have
    to pass a function-pointer to DeclareConsoleCommand(), but it is only used to determine
    the header of the future command-function. This allows to declare a command statically,
    thus it's possible to evaluate key-bindings of this command, but the actual function
    can be assigned at a later point.

    Example:
    @code
    DeclareConsoleCommand("cout", &prototype::void__string);
    @endcode

    If you try to execute the command now, you see the following (or something similar):
    @code
    $ cout Hello World
    Error: Can't execute command "cout", command is not active.
    @endcode

    You first have to assign a function to use the command:
    @code
    {
        // ...

        ModifyConsoleCommand("cout").setFunction(&myCoutFunction);

        // ...
    }
    @endcode

    Now you can use it:
    @code
    $ cout Hello World
    Text: Hello World
    @endcode

    Note that the initial function prototype::void__string is defined in the namespace
    orxonox::prototype. If there's no function with the desired header, you can extend
    the collection of functions or simply use another function that has the same header.
*/

#ifndef _ConsoleCommand_H__
#define _ConsoleCommand_H__

#include "core/CorePrereqs.h"

#include <stack>
#include <vector>

#include "util/VA_NARGS.h"
#include "ArgumentCompletionFunctions.h"
#include "Executor.h"
#include "ConsoleCommandManager.h"


/**
    @brief Defines a console command. The macro is overloaded for 2-4 parameters.

    This is an overloaded macro. Depending on the number of arguments a different
    overloaded implementation of the macro will be chosen.

    Console commands created with SetConsoleCommand() become active immediately and
    the given function-pointer (and optionally the object) will be used to execute
    the command.
*/
#define SetConsoleCommand(...) \
    BOOST_PP_EXPAND(BOOST_PP_CAT(SetConsoleCommand, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__))
/**
    @brief This macro is executed if you call SetConsoleCommand() with 2 arguments.
    @param name The name (string) of the console command
    @param functionpointer The function-pointer of the corresponding command-function
*/
#define SetConsoleCommand2(name, functionpointer) \
    SetConsoleCommandGeneric("", name, orxonox::createFunctor(functionpointer))
/**
    @brief This macro is executed if you call SetConsoleCommand() with 3 arguments.
    @param group The group (string) of the console command
    @param name The name (string) of the console command
    @param functionpointer The function-pointer of the corresponding command-function
*/
#define SetConsoleCommand3(group, name, functionpointer) \
    SetConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer))
/**
    @brief This macro is executed if you call SetConsoleCommand() with 4 arguments.
    @param group The group (string) of the console command
    @param name The name (string) of the console command
    @param functionpointer The function-pointer of the corresponding command-function
    @param object The object that will be assigned to the command. Used for member-functions.
*/
#define SetConsoleCommand4(group, name, functionpointer, object) \
    SetConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer, object))

/// Internal macro
#define SetConsoleCommandGeneric(group, name, functor) \
    static orxonox::ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __UNIQUE_NUMBER__) = (*orxonox::createConsoleCommand(group, name, orxonox::createExecutor(functor)))


/**
    @brief Declares a console command. The macro is overloaded for 2-3 parameters.

    This is an overloaded macro. Depending on the number of arguments a different
    overloaded implementation of the macro will be chosen.

    Console commands created with DeclareConsoleCommand() don't use the the given
    function-pointer to execute the command, it is only used to define the header
    of the future command-function. The command is inactive until you manually
    set a function with orxonox::ModifyConsoleCommand(). You can use a different
    function-pointer than in the final command, as long as it has the same header.
*/
#define DeclareConsoleCommand(...) \
    BOOST_PP_EXPAND(BOOST_PP_CAT(DeclareConsoleCommand, ORXONOX_VA_NARGS(__VA_ARGS__))(__VA_ARGS__))
/**
    @brief This macro is executed if you call DeclareConsoleCommand() with 2 arguments.
    @param name The name (string) of the console command
    @param functionpointer The function-pointer of an arbitrary function that has the same header as the final function
*/
#define DeclareConsoleCommand2(name, functionpointer) \
    DeclareConsoleCommandGeneric("", name, orxonox::createFunctor(functionpointer))
/**
    @brief This macro is executed if you call DeclareConsoleCommand() with 3 arguments.
    @param group The group (string) of the console command
    @param name The name (string) of the console command
    @param functionpointer The function-pointer of an arbitrary function that has the same header as the final function
*/
#define DeclareConsoleCommand3(group, name, functionpointer) \
    DeclareConsoleCommandGeneric(group, name, orxonox::createFunctor(functionpointer))

/// Internal macro
#define DeclareConsoleCommandGeneric(group, name, functor) \
    static orxonox::ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __UNIQUE_NUMBER__) = (*orxonox::createConsoleCommand(group, name, orxonox::createExecutor(functor), false))


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

    /**
        @brief Creates a new ConsoleCommand.
        @param name The name of the command
        @param executor The executor of the command
        @param bInitialized If true, the command is ready to be executed, otherwise it has to be activated first.
    */
    inline ConsoleCommand* createConsoleCommand(const std::string& name, const ExecutorPtr& executor, bool bInitialized = true)
        { return new ConsoleCommand("", name, executor, bInitialized); }
    /**
        @brief Creates a new ConsoleCommand.
        @param group The group of the command
        @param name The name of the command
        @param executor The executor of the command
        @param bInitialized If true, the command is ready to be executed, otherwise it has to be activated first.
    */
    inline ConsoleCommand* createConsoleCommand(const std::string& group, const std::string& name, const ExecutorPtr& executor, bool bInitialized = true)
        { return new ConsoleCommand(group, name, executor, bInitialized); }


    /**
        @brief Returns a manipulator for a command with the given name.

        @note If the command doesn't exist, the manipulator contains a NULL pointer to the command,
        but it can still be used without checks, because all functions of ConsoleCommandManipulator
        check internally if the command exists.
    */
    inline ConsoleCommand::ConsoleCommandManipulator ModifyConsoleCommand(const std::string& name)
        { return ConsoleCommandManager::getCommand(name, true); }
    /**
        @brief Returns a manipulator for a command with the given group and name.

        @note If the command doesn't exist, the manipulator contains a NULL pointer to the command,
        but it can still be used without checks, because all functions of ConsoleCommandManipulator
        check internally if the command exists.
    */
    inline ConsoleCommand::ConsoleCommandManipulator ModifyConsoleCommand(const std::string& group, const std::string& name)
        { return ConsoleCommandManager::getCommand(group, name, true); }
}

#endif /* _ConsoleCommand_H__ */
