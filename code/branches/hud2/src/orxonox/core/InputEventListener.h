/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 @brief InputEventListener Interface declaration
 */

#ifndef _InputEventListener_H__
#define _InputEventListener_H__

#include "CorePrereqs.h"

#include "core/OrxonoxClass.h"
#include "InputEvent.h"

namespace orxonox
{
  /**
    @brief Interface that any class can use to get key independent input.
  */
  class _CoreExport InputEventListener : virtual public OrxonoxClass
  {
    friend class InputManager;
  public:
    InputEventListener();
    virtual ~InputEventListener();
    /** Method is called every time an Input event occurs && bActive */
    virtual void eventOccured(InputEvent &evt) = 0;

  private:
    bool bActive_;
  };
}

#endif /* _InputEventListener_H__ */
