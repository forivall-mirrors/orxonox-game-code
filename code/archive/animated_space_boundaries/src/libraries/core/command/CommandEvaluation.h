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
    @defgroup CommandExecEval Command execution and evaluation
    @ingroup Command
*/

/**
    @file
    @ingroup Command CommandExecEval
    @brief Declaration of the orxonox::CommandEvaluation class which is returned by orxonox::CommandExecutor::evaluate().

    The orxonox::CommandEvaluation class gathers all information about a command-string. It is used
    internally by orxonox::CommandExecutor and can also be returned by it to provide more information
    about a command. It's also possible to evaluate the arguments of a command to execute it faster.

    @see See @ref CommandExecutorExample "this description" for an example.
*/

#ifndef _CommandEvaluation_H__
#define _CommandEvaluation_H__

#include "core/CorePrereqs.h"

#include <string>

#include "util/SubString.h"
#include "util/MultiType.h"
#include "ArgumentCompletionListElement.h"
#include "Functor.h"

namespace orxonox
{
    /**
        @brief CommandEvaluation is used to gather information about a command and to evaluate its arguments

        This class provides several information about a command-string, for example the
        evaluated ConsoleCommand, but also other information like hints if the command
        is not yet finished.

        @note You don't have to call evaluateArguments() manually, it's also done automatically
        if you call the command the first time. However you can of course do it at any earlier
        time, for example to return an error message if it doesn't work.

        @remarks execCommand_ and hintCommand_ can be different in this case: There are multiple
        commands avaliable, let's say "tcl" and "TclThreadManager". The user enters "tcl", which
        is already a valid command. Now execCommand_ points to the "tcl"-command, but hintCommand_
        still points to the autocompletion command of CommandExecutor, because the auto-completion
        list must still return the two possible commands, "tcl TclThreadManager" because the user
        may want to write "TclThreadManager ..." and needs auto-completion.

        @see See @ref CommandExecutorExample "this description" for an example.
    */
    class _CoreExport CommandEvaluation
    {
        friend class CommandExecutor;

        public:
            CommandEvaluation();

            int execute();
            MultiType query(int* error = 0);

            std::string complete();
            std::string hint();

            std::string getCommandSuggestion() const;

            int evaluateArguments(bool bPrintError = false);

            /// Returns true if the command evaluation contains a valid command that can be executed.
            inline bool isValid() const
                { return (this->execCommand_ != 0); }

            /// Returns the console command that was evaluated by this object.
            inline const ConsoleCommand* getConsoleCommand() const
                { return this->execCommand_; }

            void setEvaluatedArgument(unsigned int index, const MultiType& arg);
            MultiType getEvaluatedArgument(unsigned int index) const;

            /**
                @brief Returns a list of possible arguments for the given command.

                Note that the command's arguments may not be complete yet, so in this case
                this list returns the possibilities. They can then be used to display a list
                of the possible arguments or to apply auto-completion.
            */
            const ArgumentCompletionList& getPossibleArguments() const
                { return this->possibleArguments_; }

            /// Returns the number of possible arguments. Empty ("") arguments are not counted.
            size_t getPossibleArgumentsSize() const
                { return CommandEvaluation::getSize(this->possibleArguments_); }

        private:
            void initialize(const std::string& command);

            unsigned int getNumberOfArguments() const;
            const std::string& getLastArgument() const;
            const std::string& getToken(unsigned int i) const;

            void retrievePossibleArguments();

            static void strip(ArgumentCompletionList& list, const std::string& fragment);
            static size_t getSize(const ArgumentCompletionList& list);

            static std::string dump(const ArgumentCompletionList& list);
            static std::string dump(const ConsoleCommand* command);

            static std::string getCommonBegin(const ArgumentCompletionList& list);

            const ConsoleCommand* execCommand_;             ///< The command that will be executed (can be NULL if the command is not valid)
            const ConsoleCommand* hintCommand_;             ///< The command that is used to display hints and argument lists (can be different to execCommand_ in some cases)
            SubString tokens_;                              ///< The single words of the command string, split into tokens
            std::string string_;                            ///< The original command string, entered by the user in the shell
            unsigned int execArgumentsOffset_;              ///< The index of the first argument in tokens_ used to execute the command
            unsigned int hintArgumentsOffset_;              ///< The index of the first argument in tokens_ used to display hints and argument lists
            bool bPossibleArgumentsRetrieved_;              ///< True if the list of possible arguments was already retrieved
            ArgumentCompletionList possibleArguments_;      ///< List of possible arguments for the command in the current state

            bool bEvaluatedArguments_;                      ///< True if the arguments of the command have been evaluated (and stored in arguments_)
            bool bTriedToEvaluatedArguments_;               ///< True if an attempt was started to evaluate the arguments (but not necessarily successful)
            unsigned int numberOfEvaluatedArguments_;       ///< The number of evaluated arguments (ranges from 0 to MAX_FUNCTOR_ARGUMENTS)
            MultiType arguments_[MAX_FUNCTOR_ARGUMENTS];    ///< The evaluated arguments (the arguments from string_ or tokens_ converted to the right type)
    };
}

#endif /* _CommandEvaluation_H__ */
