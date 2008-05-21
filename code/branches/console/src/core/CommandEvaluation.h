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

#ifndef _CommandEvaluation_H__
#define _CommandEvaluation_H__

#include "CorePrereqs.h"

#include <string>
#include <list>

#include "util/SubString.h"
#include "util/MultiTypeMath.h"

namespace orxonox
{
    enum CommandState
    {
        CS_Uninitialized,
        CS_Empty,
        CS_FunctionClass_Or_Shortcut,
        CS_Shortcut_Params,
        CS_Shortcut_Finished,
        CS_Function,
        CS_Function_Params,
        CS_Function_Finished,
        CS_Error
    };

    class _CoreExport CommandEvaluation
    {
        public:
            CommandEvaluation();

            void initialize(const std::string& command);

            void execute() const;
            std::string complete() const;
            std::string hint() const;
            void evaluateParams();

            bool isValid() const;

            inline Identifier* getIdentifier() const
                { return this->functionclass_; }
            inline void setIdentifier(Identifier* identifier)
                { this->functionclass_ = identifier; }
            inline ConsoleCommand* getFunction() const
                { return this->function_; }
            inline void setFunction(ConsoleCommand* command)
                { this->function_ = command; }

            inline const std::string& getOriginalCommand() const
                { return this->originalCommand_; }
            inline const std::string& getCommand() const
                { return this->command_; }
            inline void setCommand(const std::string& command)
                { this->command_ = command; }
            inline const CommandState& getState() const
                { return this->state_; }
            inline void setState(CommandState state)
                { this->state_ = state; }
            inline SubString& getOriginalTokens()
                { return this->originalCommandTokens_; }
            inline SubString& getTokens()
                { return this->commandTokens_; }
            inline void setTokens(const std::string& command)
                { this->commandTokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0'); }
            inline const std::string& getError() const
                { return this->errorMessage_; }
            inline void setError(const std::string& error)
                { this->errorMessage_ = error; }
            inline bool isNewCommand() const
                { return this->bNewCommand_; }
            inline void setNewCommand(bool bNewCommand)
                { this->bNewCommand_ = bNewCommand; }

            inline std::list<std::pair<const std::string*, const std::string*> >& getListOfPossibleFunctionClasses()
                { return this->listOfPossibleFunctionClasses_; }
            inline std::list<std::pair<const std::string*, const std::string*> >& getListOfPossibleFunctions()
                { return this->listOfPossibleFunctions_; }

            inline void setAdditionalParameter(const std::string& param)
                { this->additionalParameter_ = param; this->bEvaluatedParams_ = false; }
            inline std::string getAdditionalParameter() const
                { return (this->additionalParameter_ != "") ? (" " + this->additionalParameter_) : ""; }

            void setEvaluatedParameter(unsigned int index, MultiTypeMath param);
            MultiTypeMath getEvaluatedParameter(unsigned int index) const;

            bool hasReturnvalue() const;
            MultiTypeMath getReturnvalue() const;

        private:
            unsigned int getStartindex() const;
            static std::string getCommonBegin(const std::list<std::pair<const std::string*, const std::string*> >& list);
            static std::string dump(const std::list<std::pair<const std::string*, const std::string*> >& list);
            static std::string dump(const ConsoleCommand* command);


            bool bNewCommand_;

            std::string originalCommand_;
            std::string command_;
            SubString originalCommandTokens_;
            SubString commandTokens_;
            std::string additionalParameter_;

            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctionClasses_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctions_;

            Identifier* functionclass_;
            ConsoleCommand* function_;

            std::string errorMessage_;
            CommandState state_;

            bool bEvaluatedParams_;
            MultiTypeMath param_[5];
    };
}

#endif /* _CommandEvaluation_H__ */
