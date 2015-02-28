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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Different definitions of input processing.
*/

#ifndef _InputCommands_H__
#define _InputCommands_H__

#include "InputPrereqs.h"
#include "core/command/CommandEvaluation.h"

namespace orxonox
{
    class _CoreExport BufferedParamCommand
    {
    public:
        BufferedParamCommand() : abs_(0.0f), rel_(0.0), paramIndex_(-1) { }
        bool execute();

        float abs_;
        float rel_;
        int paramIndex_;
        CommandEvaluation evaluation_;
    };

    class _CoreExport BaseCommand
    {
    public:
        BaseCommand() : bFixedKeybindMode_(false) {}
        virtual ~BaseCommand() { }

        virtual bool execute(float abs = 1.0f, float rel = 1.0f) = 0;
        virtual CommandEvaluation* getEvaluation() = 0;

        inline void setFixedKeybindMode(bool fixed)
            { this->bFixedKeybindMode_ = fixed; }
        inline bool hasFixedKeybindMode() const
            { return this->bFixedKeybindMode_; }

        virtual BaseCommand* clone() = 0;

    private:
        bool bFixedKeybindMode_;
    };

    class _CoreExport SimpleCommand : public BaseCommand
    {
    public:
        bool execute(float abs = 1.0f, float rel = 1.0f);
        CommandEvaluation* getEvaluation();
        virtual SimpleCommand* clone() { return new SimpleCommand(*this); }

        CommandEvaluation evaluation_;
    };

    /**
    @brief
        Executes a simple command with no additional paramters.
    @return
        True if command execution was successful, false otherwise.
    */
    inline bool SimpleCommand::execute(float abs, float rel)
    {
        return evaluation_.execute();
    }

    /// Returns a pointer to the encapsuled evaluation.
    inline CommandEvaluation* SimpleCommand::getEvaluation()
    {
        return &this->evaluation_;
    }

    class _CoreExport ParamCommand : public BaseCommand
    {
    public:
        ParamCommand() : scale_(1.0f), paramCommand_(0) { }
        bool execute(float abs = 1.0f, float rel = 1.0f);
        CommandEvaluation* getEvaluation();
        virtual ParamCommand* clone() { return new ParamCommand(*this); }

        float scale_;
        BufferedParamCommand* paramCommand_;
    };

    /// Returns a pointer to the encapsuled evaluation.
    inline CommandEvaluation* ParamCommand::getEvaluation()
    {
        if (this->paramCommand_)
            return &this->paramCommand_->evaluation_;
        else
            return 0;
    }
}

#endif /* _InputCommands_H__ */
