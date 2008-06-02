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
 @brief Implementation of the different input handlers.
 */

#include "InputCommands.h"
#include "core/CommandExecutor.h"

namespace orxonox
{
  // ###############################
  // ###  BufferedParamCommand   ###
  // ###############################

  /**
  * Executes a buffered command. This is used for commands with additional
  * parameters.
  * @return True if command execution was successful or value was zero.
  */
  bool BufferedParamCommand::execute()
  {
    if (nValuesAdded_)
    {
      BufferedParamCommand& cmd = *this;
      cmd.evaluation_.setEvaluatedParameter(cmd.paramIndex_, cmd.value_);
      // reset
      cmd.nValuesAdded_ = 0;
      cmd.value_ = 0;
      return cmd.evaluation_.execute();
    }
    else
      return true;
  }

  // ###############################
  // #####    SimpleCommand    #####
  // ###############################

  /**
  * Executes a simple command with no additional paramters.
  * @return True if command execution was successful, false otherwise.
  */
  bool SimpleCommand::execute(float abs, float rel)
  {
    return evaluation_.execute();
  }

  // ###############################
  // #####    ParamCommand     #####
  // ###############################

  /**
  * Executes a parameter command. The commmand string is not directly executed,
  * but instead stored in a buffer list so that values can be combined.
  * @return Always true.
  */
  bool ParamCommand::execute(float abs, float rel)
  {
    BufferedParamCommand& cmd = *paramCommand_;
    // command has an additional parameter
    if (bRelative_)
    {
      if (rel != 0.0f)
      {
        // we have to calculate a relative movement.
        // paramModifier_ says how much one keystroke is
        cmd.value_ += paramModifier_ * rel;
      }
    }
    else if (abs != 0.0f)
    {
      // Usually, joy sticks create 'noise' (they return values if they're in 0 position)
      // and normally this is caught in tickInput(), but that threshold cannot be to high
      // in order to preserve accuracy. Instead, we have to catch the problem here. An example:
      // Someone only uses buttons with an active joystick. The joy stick value could then
      // be 0.05 for instance and the the key value 1. Without handling the problem, the final
      // value would be computed to (1+0.05)/2=0.5025 which is not what the user expects.
      float absQ = abs * abs;
      float valueQ = cmd.value_ * cmd.value_;
      if (absQ > 50.0f * valueQ) // ease up comparison by using quadratics
      {
        cmd.value_ = abs * paramModifier_;
        cmd.nValuesAdded_ = 1;
      }
      else if (absQ * 50.0f < valueQ)
      {
        // abs is too small, we just don't do anything
      }
      else
      {
        // we have to calculate the absolute position of the axis.
        // Since there might be another axis that is affected, we have to wait and
        // store the result in a temporary place
        cmd.value_ = (cmd.value_ * cmd.nValuesAdded_ + paramModifier_ * abs) / ++cmd.nValuesAdded_;
      }
    }
    return true;
  }
}
