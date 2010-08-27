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

#include "core/CorePrereqs.h"

#include <string>

#include "util/SubString.h"
#include "util/MultiType.h"
#include "ArgumentCompletionListElement.h"
#include "Functor.h"

namespace orxonox
{
    class _CoreExport CommandEvaluation
    {
        friend class CommandExecutor;

        public:
            CommandEvaluation();

            int execute();
            MultiType query(int* error = 0);

            std::string complete();
            std::string hint();

            int evaluateParams(bool bPrintError = false);

            inline bool isValid() const
                { return (this->execCommand_ != 0); }

            inline _ConsoleCommand* getConsoleCommand() const
                { return this->execCommand_; }

            void setEvaluatedParameter(unsigned int index, const MultiType& param);
            MultiType getEvaluatedParameter(unsigned int index) const;

        private:
            void initialize(const std::string& command);

            unsigned int getNumberOfArguments() const;
            const std::string& getLastArgument() const;
            const std::string& getToken(unsigned int i) const;

            void retrievePossibleArguments();

            static void strip(ArgumentCompletionList& list, const std::string& fragment);

            static std::string dump(const ArgumentCompletionList& list);
            static std::string dump(const _ConsoleCommand* command);

            static std::string getCommonBegin(const ArgumentCompletionList& list);

            _ConsoleCommand* execCommand_;
            _ConsoleCommand* hintCommand_;
            SubString tokens_;
            std::string string_;
            unsigned int execArgumentsOffset_;
            unsigned int hintArgumentsOffset_;
            bool bPossibleArgumentsRetrieved_;
            ArgumentCompletionList possibleArguments_;

            bool bEvaluatedParams_;
            bool bTriedToEvaluatedParams_;
            unsigned int numberOfEvaluatedParams_;
            MultiType param_[MAX_FUNCTOR_ARGUMENTS];
    };
}

#endif /* _CommandEvaluation_H__ */
