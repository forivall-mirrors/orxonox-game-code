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
    @ingroup Command ArgumentCompletion
    @brief Declaration of all argument completion functions and macros used to define them.

    @anchor ArgumentCompletionExample

    Argument completion functions are used to create a list of possible arguments
    for an orxonox::ConsoleCommand. These functions are usually wrapped by an instance
    of orxonox::ArgumentCompleter.

    Argument completion functions can be declared and implemented by using the macros
    ARGUMENT_COMPLETION_FUNCTION_DECLARATION() and ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION()
    respectively. They are necessary because they don't simply define the function, but they also
    create a static helper function that returns an instance of orxonox::ArgumentCompleter which
    wraps the defined function. This allows easier referencing of argument completion functions
    by simply calling autocompletion::functionname().

    Argument completion functions can take up to 5 arguments, all of type std::string.
    The first argument is always the current argument which is being entered by the user
    in the shell. The second argument is the argument before, so in fact arguments from
    the shell are sent in reversed order to the argument completion function. This is
    necessary because the number of arguments can be variable

    Example: The user types the following into the shell:
    @code
    $ commandname argument1 argument2 argum
    @endcode
    Then he presses the @a tab key to print the possible arguments. Now the argument
    completion function for the @a third argument of @a commandname will be called in
    the following way:
    @code
    list = argcompfunction3("argum", "argument2", "argument1");
    @endcode

    Usually each argument is one word (without whitespaces in it), but some argument completion
    functions need more than one word. This can be achieved by using ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI().
    In this case all supernumerous words are passed to the first (!) argument.

    An example to show how to declare, implement, and use an argument completion function:
    @code
    // ArgumentCompletionFunctions.h:
    // ------------------------------

    // Declaration of the function:
    ARGUMENT_COMPLETION_FUNCTION_DECLARATION(month)(const std::string& fragment);

    // ArgumentCompletionFunctions.cc:
    // -------------------------------

    // Implementation of the function
    ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(month)(const std::string& fragment)
    {
        ArgumentCompletionList list;

        // check if the first part of the argument is a number - if yes, the user likely wants to enter the month as a number
        if (isNumber(fragment))
        {
            for (int month = 1; month <= 12; ++month)
                list.push_back(ArgumentCompletionListElement(multi_cast<std::string>(month)));
        }
        else
        {
            list.push_back(ArgumentCompletionListElement("January",   "january"));
            list.push_back(ArgumentCompletionListElement("February",  "february"));
            list.push_back(ArgumentCompletionListElement("March",     "march"));
            list.push_back(ArgumentCompletionListElement("April",     "april"));
            list.push_back(ArgumentCompletionListElement("May",       "may"));
            list.push_back(ArgumentCompletionListElement("June",      "june"));
            list.push_back(ArgumentCompletionListElement("July",      "july"));
            list.push_back(ArgumentCompletionListElement("August",    "august"));
            list.push_back(ArgumentCompletionListElement("September", "september"));
            list.push_back(ArgumentCompletionListElement("October",   "october"));
            list.push_back(ArgumentCompletionListElement("November",  "november"));
            list.push_back(ArgumentCompletionListElement("December",  "december"));
        }

        return list;
    }

    // SomeFile:
    // ---------

    // A class to manage the date:
    class Date
    {
        public:
            static void setDate(int day, const std::string& month, int year);
    };

    // Define a console command that needs a date. Add argument completion for the month:
    SetConsoleCommand("setDate", &Date::setDate).argumentCompleter(1, autocompletion::month());
    @endcode

    This example defines an argument completion function that returns a list of possible
    months. If the first part of the argument is a number, it returns the numbers 1-12,
    otherwise the name of the months are returned. Note how the list is composed by
    instances of orxonox::ArgumentCompletionListElement. For the name of the months,
    two strings are provided, one in normal case and one in lower case. See the documentation
    of orxonox::ArgumentCompletionListElement for more information about this.

    Also note that the argument completion list is assigned to the console command by using
    @ref orxonox::ConsoleCommand::argumentCompleter "argumentCompleter()". The first argument
    is the index of the argument:
     - 0 is the first argument (@a day)
     - 1 is the second argument (@a month)
     - 2 is the third argument (@a year)

    @a day and @a year don't need an argument completion function as they are just integers.

    The function @c autocompletion::month() is automatically created by the macros
    ARGUMENT_COMPLETION_FUNCTION_DECLARATION() and ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION()
    and returns an orxonox::ArgumentCompleter that wraps the defined argument completion function.

    The implemented argument completion function uses only one argument, the fragment of the
    currently entered argument. More complex functions can also use the previous arguments
    to return different arguments depending on the other arguments (for example to list the
    members of a class, where the class-name is the first argument and the member the second).
*/

#ifndef _ArgumentCompletionFunctions_H__
#define _ArgumentCompletionFunctions_H__

#include "core/CorePrereqs.h"
#include "ArgumentCompleter.h"

/**
    @brief Used to declare an argument completion function with name @a functionname.
    @param functionname The name of the function, will also be used for the implementation of the function.

    The macro also defines a static function that returns an orxonox::ArgumentCompleter
    which wraps the defined function. This can be accessed by calling autocompletion::functionname();
*/
#define ARGUMENT_COMPLETION_FUNCTION_DECLARATION(functionname) \
    _CoreExport ArgumentCompleter* functionname(); \
    _CoreExport ArgumentCompletionList acf_##functionname

/**
    @brief Used to implement an argument completion function.
    @param functionname The name of the function
*/
#define ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(functionname) \
    _ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_INTERNAL(functionname, false)

/**
    @brief Used to implement an argument completion function which allows multiple words.
    @param functionname The name of the function
*/
#define ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI(functionname) \
    _ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_INTERNAL(functionname, true)

/// Internal macro
#define _ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_INTERNAL(functionname, bUseMultipleWords) \
    ArgumentCompleter* functionname() \
    { \
        static ArgumentCompleter completer = ArgumentCompleter(&acf_##functionname, bUseMultipleWords); \
        return &completer; \
    } \
    \
    ArgumentCompletionList acf_##functionname

/// Calls an argument completion function. Used for functions that return the results of another argument completion function.
#define ARGUMENT_COMPLETION_FUNCTION_CALL(functionname) acf_##functionname


namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(fallback)();
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(groupsandcommands)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(subcommands)(const std::string& fragment, const std::string& group);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(command)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(hiddencommand)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(files)(const std::string& fragment);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(settingssections)();
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(settingsentries)(const std::string& fragment, const std::string& section);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(settingsvalue)(const std::string& fragment, const std::string& entry, const std::string& section);
        ARGUMENT_COMPLETION_FUNCTION_DECLARATION(tclthreads)();
    }
}

#endif /* _ArgumentCompletionFunctions_H__ */
