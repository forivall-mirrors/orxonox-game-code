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
    @defgroup ArgumentCompletion Argument completion
    @ingroup Command
*/

/**
    @file
    @ingroup Command ArgumentCompletion
    @brief Definition of the orxonox::ArgumentCompleter class that is used to execute @ref ArgumentCompletionFunctions.h "argument completion functions".

    An ArgumentCompleter can be assigned to an orxonox::ConsoleCommand using
    @ref orxonox::ConsoleCommand::argumentCompleter "argumentCompleter()".
    The ArgumentCompleter calls an argument completion function that is defined
    in ArgumentCompletionFunctions.h. This can be used to list possible arguments
    for console commands and to allow auto-completion.

    Instances of ArgumentCompleter are usually not created manually but rather
    by the macros defined in ArgumentCompletionFunctions.h. There you'll also
    find some examples.

    @see See the @ref ArgumentCompletionExample "examples".
*/

#ifndef _ArgumentCompleter_H__
#define _ArgumentCompleter_H__

#include "core/CorePrereqs.h"
#include "ArgumentCompletionListElement.h"

namespace orxonox
{
    /**
        @brief This class executes an argument completion function and returns a list of the possible arguments.

        ArgumentCompleter is used to wrap argument completion functions as defined
        in ArgumentCompletionFunctions.h and can be assigned to a ConsoleCommand to
        create a list of possible arguments.

        @see See ArgumentCompleter.h for more information.
        @see See @ref ArgumentCompletionExample "ArgumentCompletionFunctions.h" for an example.
    */
    class _CoreExport ArgumentCompleter
    {
        public:
            ArgumentCompleter(ArgumentCompletionList (*function) (void), bool bUseMultipleWords) : bUseMultipleWords_(bUseMultipleWords), paramCount_(0), function_0_(function) {} ///< Constructor, assigns a function-pointer with no arguments.
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1), bool bUseMultipleWords) : bUseMultipleWords_(bUseMultipleWords), paramCount_(1), function_1_(function) {} ///< Constructor, assigns a function-pointer with one argument.
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2), bool bUseMultipleWords) : bUseMultipleWords_(bUseMultipleWords), paramCount_(2), function_2_(function) {} ///< Constructor, assigns a function-pointer with two arguments.
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2, const std::string& param3), bool bUseMultipleWords) : bUseMultipleWords_(bUseMultipleWords), paramCount_(3), function_3_(function) {} ///< Constructor, assigns a function-pointer with three arguments.
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4), bool bUseMultipleWords) : bUseMultipleWords_(bUseMultipleWords), paramCount_(4), function_4_(function) {} ///< Constructor, assigns a function-pointer with four arguments.
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4, const std::string& param5), bool bUseMultipleWords) : bUseMultipleWords_(bUseMultipleWords), paramCount_(5), function_5_(function) {} ///< Constructor, assigns a function-pointer with five arguments.

            /**
                @brief Calls the argument completion function with a maximum of five parameters.
                @return Returns the list of possible arguments, created by the argument completion function
            */
            ArgumentCompletionList operator()(const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "", const std::string& param5 = "")
            {
                switch (this->paramCount_)
                {
                    case 0:
                        return (*this->function_0_)();
                    case 1:
                        return (*this->function_1_)(param1);
                    case 2:
                        return (*this->function_2_)(param1, param2);
                    case 3:
                        return (*this->function_3_)(param1, param2, param3);
                    case 4:
                        return (*this->function_4_)(param1, param2, param3, param4);
                    case 5:
                        return (*this->function_5_)(param1, param2, param3, param4, param5);
                    default:
                        return ArgumentCompletionList();
                }
            }

            /// Returns true if the argument completion list supports multiple words.
            inline bool useMultipleWords() const
                { return this->bUseMultipleWords_; }

        private:
            bool bUseMultipleWords_;    ///< If true, the argument completion list supports multiple words
            unsigned char paramCount_;  ///< The number of parameters of the argument completion function
            ArgumentCompletionList (*function_0_) (void);   ///< Function-pointer for an argument completion function with no arguments
            ArgumentCompletionList (*function_1_) (const std::string& param1);   ///< Function-pointer for an argument completion function with one argument
            ArgumentCompletionList (*function_2_) (const std::string& param1, const std::string& param2);   ///< Function-pointer for an argument completion function with two arguments
            ArgumentCompletionList (*function_3_) (const std::string& param1, const std::string& param2, const std::string& param3);   ///< Function-pointer for an argument completion function with three arguments
            ArgumentCompletionList (*function_4_) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4);   ///< Function-pointer for an argument completion function with four arguments
            ArgumentCompletionList (*function_5_) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4, const std::string& param5);   ///< Function-pointer for an argument completion function with five arguments
    };
}

#endif /* _ArgumentCompleter_H__ */
