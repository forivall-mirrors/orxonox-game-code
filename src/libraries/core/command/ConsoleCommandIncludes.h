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
    @brief Declaration the SetConsoleCommand() macro.

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

#ifndef _ConsoleCommandIncludes_H__
#define _ConsoleCommandIncludes_H__

#include "core/CorePrereqs.h"

#include "ConsoleCommand.h"
#include "ConsoleCommandManager.h"
#include "util/VA_NARGS.h"
#include "core/module/StaticallyInitializedInstance.h"


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
    static orxonox::ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __UNIQUE_NUMBER__) \
        = (new orxonox::SI_CC(new orxonox::ConsoleCommand(group, name, orxonox::createExecutor(functor))))->getCommand()


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
    static orxonox::ConsoleCommand& BOOST_PP_CAT(__consolecommand_, __UNIQUE_NUMBER__) \
        = (new orxonox::SI_CC(new orxonox::ConsoleCommand(group, name, orxonox::createExecutor(functor), false)))->getCommand()


namespace orxonox
{
    class _CoreExport StaticallyInitializedConsoleCommand : public StaticallyInitializedInstance
    {
        public:
            StaticallyInitializedConsoleCommand(ConsoleCommand* command) : command_(command) {}

            virtual void load();
            virtual void unload();

            inline ConsoleCommand& getCommand()
                { return *this->command_; }

        private:
            ConsoleCommand* command_;
    };

    typedef StaticallyInitializedConsoleCommand SI_CC;

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

#endif /* _ConsoleCommandIncludes_H__ */
