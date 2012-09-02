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

#ifndef _HalfAxis_H__
#define _HalfAxis_H__

#include "InputPrereqs.h"

#include "Button.h"
#include "InputCommands.h"

namespace orxonox
{
    class _CoreExport HalfAxis : public Button
    {
    public:
        HalfAxis()
            : relVal_(0.0f)
            , absVal_(0.0f)
            , paramCommands_(0)
            , nParamCommands_(0)
            , pressed_(false)
            , hasChanged_(false)
        { }
        using Button::execute;
        bool execute();
        bool addParamCommand(ParamCommand* command);
        void clear();
        void reset();

        // axis related
        float relVal_;
        float absVal_;
        ParamCommand** paramCommands_;
        unsigned int nParamCommands_;

        // button related
        bool pressed_;
        bool hasChanged_;
    };

    inline bool HalfAxis::execute()
    {
        bool success = true;
        for (unsigned int i = 0; i < nParamCommands_; i++)
            success = paramCommands_[i]->execute(absVal_, relVal_) && success;
        return success;
    }

    inline void HalfAxis::reset()
    {
        this->relVal_ = 0.0f;
        this->absVal_ = 0.0f;
        this->hasChanged_ = true;
    }
}

#endif /* _HalfAxis_H__ */
