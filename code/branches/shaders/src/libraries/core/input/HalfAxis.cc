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
    Implementation of the different input handlers.
*/

#include "HalfAxis.h"
#include "core/command/CommandEvaluation.h"

namespace orxonox
{
    void HalfAxis::clear()
    {
        Button::clear();
        if (nParamCommands_)
        {
            // delete all commands and the command pointer array
            for (unsigned int i = 0; i < nParamCommands_; i++)
                delete paramCommands_[i];
            delete[] paramCommands_;
            nParamCommands_ = 0;
        }
    }

    bool HalfAxis::addParamCommand(ParamCommand* command)
    {
        ParamCommand** cmds = paramCommands_;
        paramCommands_ = new ParamCommand*[++nParamCommands_];
        unsigned int i;
        for (i = 0; i < nParamCommands_ - 1; i++)
            paramCommands_[i] = cmds[i];
        paramCommands_[i] = command;
        if (nParamCommands_ > 1)
            delete[] cmds;
        return true;
    }
}
