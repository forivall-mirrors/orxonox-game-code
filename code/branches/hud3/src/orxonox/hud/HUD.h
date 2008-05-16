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
*      Yuning Chai
*   Co-authors:
*      ...
*
*/


#ifndef _HUD_H__
#define _HUD_H__

#include <string.h>
#include <OgreOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePrerequisites.h>

#include "OrxonoxPrereqs.h"

#include "core/Tickable.h"
#include "BarOverlayElement.h"
#include "RadarOverlayElement.h"


namespace orxonox
{
  class _OrxonoxExport HUD : public Tickable
  {
  private:

  public:
    HUD(int zoom);
    ~HUD();

    virtual void tick(float);

    SmartBarOverlayElement* energyCounter;

    RadarOverlayElement* radar;
  };
}

#endif
