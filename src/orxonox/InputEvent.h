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
 @brief InputEvent Structure declaration
 */

#ifndef _InputEvent_H__
#define _InputEvent_H__

#include "OrxonoxPrereqs.h"

namespace orxonox
{
  /**
    @brief Interface that any class can use to get key independent input.
  */
  struct InputEvent
  {
    unsigned char id;      //!< Event ID
    bool bStarted;         //!< Tells whether the Event started or ended (e.g. firing started)
    int data1, data2;      //!< Two additional integer datafields, for instance for the mouse moved event
    void* dataRef;         //!< Additional pointer to any further data
  };
}

#endif /* _InputEvent_H__ */
