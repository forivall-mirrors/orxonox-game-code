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

#ifndef _KeyDetector_H__
#define _KeyDetector_H__

#include "core/CorePrereqs.h"

#include "KeyBinder.h"

namespace orxonox
{
  class _CoreExport KeyDetector : public KeyBinder
  {
  public:
    KeyDetector();
    ~KeyDetector();
    void loadBindings();

  protected:
    void readTrigger(Button& button);
  };
}

#endif /* _KeyDetector_H__ */
