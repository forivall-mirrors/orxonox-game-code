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

#include "InputCommands.h"
#include "util/Math.h"

namespace orxonox
{
    // ###############################
    // ###  BufferedParamCommand   ###
    // ###############################

    /**
    @brief
        Executes a buffered command. This is used for commands with additional
        parameters.
    @return
        True if command execution was successful or value was zero.
    */
    bool BufferedParamCommand::execute()
    {
        if (this->abs_ != 0.0f || this->rel_ != 0.0f)
        {
            evaluation_.setEvaluatedArgument(paramIndex_, Vector2(abs_, rel_));
            // reset
            rel_ = 0.0;
            abs_ = 0.0;
            return evaluation_.execute();
        }
        else
            return true;
    }

    // ###############################
    // #####    ParamCommand     #####
    // ###############################

    /**
    @brief
        Executes a parameter command. The commmand string is not directly executed,
        but instead stored in a buffer list so that values can be combined.
    @return
        Always true.
    */
    bool ParamCommand::execute(float abs, float rel)
    {
        BufferedParamCommand& cmd = *paramCommand_;
        // command has an additional parameter
        if (rel != 0.0f)
        {
            // calculate relative movement.
            // scale_ says how much one keystroke is
            cmd.rel_ += scale_ * rel;
        }

        if (abs != 0.0f)
        {
            cmd.abs_ += scale_ * abs;
            if (cmd.abs_ > 1.0)
                cmd.abs_ = 1.0;
            if (cmd.abs_ < -1.0)
                cmd.abs_ = -1.0;
        }
        return true;
    }
}
