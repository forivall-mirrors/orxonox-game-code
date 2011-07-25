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

#include "ArgumentCompletionListElement.h"
#include "util/SubString.h"
#include "util/MultiType.h"

namespace orxonox
{
    namespace CommandState
    {
        enum Value
        {
            Uninitialized,
            Empty,
            ShortcutOrIdentifier,
            Function,
            ParamPreparation,
            Params,
            Finished,
            Error
        };
    }

    class _CoreExport CommandEvaluation
    {
        friend class CommandExecutor;

        public:
            CommandEvaluation();

            void initialize(const std::string& command);

            bool execute() const;
            const std::string& complete();
            std::string hint() const;
            void evaluateParams();

            bool isValid() const;

            inline ConsoleCommand* getConsoleCommand() const
                { return this->function_; }
            inline const std::string& getOriginalCommand() const
                { return this->originalCommand_; }
            inline const std::string& getCommand() const
                { return this->command_; }

            inline void setAdditionalParameter(const std::string& param)
                { this->additionalParameter_ = param; this->bEvaluatedParams_ = false; }
            inline std::string getAdditionalParameter() const
                { return (!this->additionalParameter_.empty()) ? (' ' + this->additionalParameter_) : ""; }

            void setEvaluatedParameter(unsigned int index, MultiType param);
            MultiType getEvaluatedParameter(unsigned int index) const;

            bool hasReturnvalue() const;
            MultiType getReturnvalue() const;

        private:
            unsigned int getStartindex() const;
            static std::string dump(const std::list<std::pair<const std::string*, const std::string*> >& list);
            static std::string dump(const ArgumentCompletionList& list);
            static std::string dump(const ConsoleCommand* command);


            bool bNewCommand_;
            bool bCommandChanged_;

            std::string originalCommand_;
            std::string command_;
            SubString commandTokens_;
            std::string additionalParameter_;

            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleIdentifiers_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctions_;
            ArgumentCompletionList listOfPossibleArguments_;

            Identifier* functionclass_;
            ConsoleCommand* function_;
            std::string possibleArgument_;
            std::string argument_;

            std::string errorMessage_;
            CommandState::Value state_;

            bool bEvaluatedParams_;
            MultiType param_[5];
    };
}

#endif /* _CommandEvaluation_H__ */
