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
        CS_FunctionClass_Or_Shortcut_Or_Keyword,
        CS_Shortcut_Params,
        CS_Shortcut_Finished,
        CS_Function,
        CS_Function_Params,
        CS_Function_Finished,
        CS_ConfigValueClass,
        CS_ConfigValue,
        CS_ConfigValueType,
        CS_ConfigValueFinished,
        CS_KeybindKey,
        CS_KeybindCommand,
        CS_KeybindFinished,
        CS_Error
    };

    enum KeybindMode {}; // temporary

    class _CoreExport CommandEvaluation
    {
        friend class CommandExecutor;

        public:
            CommandEvaluation();

            KeybindMode getKeybindMode();
            bool isValid() const;

            inline void setAdditionalParameter(const std::string& param)
                { this->additionalParameter_ = param; this->bEvaluatedParams_ = false; }
            inline std::string getAdditionalParameter() const
                { return (this->additionalParameter_ != "") ? (" " + this->additionalParameter_) : ""; }

            void setEvaluatedParameter(unsigned int index, MultiTypeMath param);
            MultiTypeMath getEvaluatedParameter(unsigned int index) const;

            void evaluateParams();

            bool hasReturnvalue() const;
            MultiTypeMath getReturnvalue() const;

        private:
            std::string processedCommand_;
            SubString tokens_;
            std::string additionalParameter_;

            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctionClasses_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleShortcuts_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctions_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleConfigValueClasses_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleConfigValues_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleKeys_;

            Identifier* functionclass_;
            Identifier* configvalueclass_;
            ConsoleCommand* shortcut_;
            ConsoleCommand* function_;
            ConfigValueContainer* configvalue_;
            ConfigValueContainer* key_;

            std::string errorMessage_;
            CommandState state_;

            bool bEvaluatedParams_;
            MultiTypeMath param_[5];
            ConsoleCommand* evaluatedExecutor_;
    };
}

#endif /* _CommandEvaluation_H__ */
