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
 @brief Different definitions of input processing.
 */

#ifndef _InputCommands_H__
#define _InputCommands_H__

#include "core/CorePrereqs.h"
#include "core/CommandEvaluation.h"

namespace orxonox
{
  class _CoreExport BufferedParamCommand
  {
  public:
    BufferedParamCommand() : value_(0.0f), nValuesAdded_(0), paramIndex_(-1) { }
    bool execute();

    float value_;
    unsigned int nValuesAdded_;
    int paramIndex_;
    CommandEvaluation evaluation_;
  };

  class _CoreExport BaseCommand
  {
  public:
    virtual ~BaseCommand() { }
    virtual bool execute(float abs = 1.0f, float rel = 1.0f) = 0;
  };

  class _CoreExport SimpleCommand : public BaseCommand
  {
  public:
    bool execute(float abs = 1.0f, float rel = 1.0f);

    CommandEvaluation evaluation_;
  };

  class _CoreExport ParamCommand : public BaseCommand
  {
  public:
    ParamCommand() : bRelative_(false), paramModifier_(1.0f), paramCommand_(0) { }
    bool execute(float abs = 1.0f, float rel = 1.0f);

    bool bRelative_;
    float paramModifier_;
    BufferedParamCommand* paramCommand_;
  };
}

#endif /* _InputCommands_H__ */
