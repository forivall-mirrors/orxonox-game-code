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
 *
 *   Co-authors:
 *      ...
 *
 */

 /**
    @brief
        This subclass of OverlayText is used to display the stats of the player in the HUD

    @ingroup TowerDefense
 */


#ifndef _TowerDefenseHUDController_H__
#define _TowerDefenseHUDController_H__

#include "towerdefense/TowerDefensePrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"
#include "TowerDefense.h"

namespace orxonox
{
    class _TowerDefenseExport TowerDefenseHUDController : public OverlayText, public Tickable
    {
    public:
        TowerDefenseHUDController(Context* context);
        virtual ~TowerDefenseHUDController();


        virtual void tick(float dt);
        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        virtual void changedOwner();
        void setShowlives(bool temp)
            { this->showlives = temp; }
        unsigned int getShowlives(void) const
            { return this->showlives; }

        void setShowcredits(bool temp)
            { this->showcredits = temp; }
        unsigned int getShowcredits(void) const
            { return this->showcredits; }

        void setShowwaves(bool temp)
            { this->showwaves = temp; }
        unsigned int getShowwaves(void) const
            { return this->showwaves; }

    private:
        TowerDefense* td;

        bool showcredits;
        bool showlives;
        bool showwaves;
    };
}

#endif /* _TowerDefenseHUDController_H__ */
