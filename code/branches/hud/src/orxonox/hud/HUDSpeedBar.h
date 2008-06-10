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

#ifndef _HUDSpeedBar_H__
#define _HUDSpeedBar_H__

#include "OrxonoxPrereqs.h"

#include "HUDBar.h"

namespace orxonox
{
  class _OrxonoxExport HUDSpeedBar : public HUDBar
  {
    public:
      HUDSpeedBar();
      virtual ~HUDSpeedBar();

      //virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

      virtual void tick(float dt);

    private:

    };
}
#endif /* _HUDSpeedBar_H__ */
