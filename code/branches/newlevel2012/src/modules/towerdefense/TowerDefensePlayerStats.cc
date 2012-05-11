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
 */
 
#include "TowerDefensePlayerStats.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"


namespace orxonox
{
	CreateFactory(TowerDefensePlayerStats);
	
	const int kDefaultCredit = 200;
	
    TowerDefensePlayerStats::TowerDefensePlayerStats(BaseObject* creator) : OverlayText(creator)
	{
		RegisterObject(TowerDefensePlayerStats);
		
		this->credit_ = 200;
		this->waveNumber_ = 0;
	}
	
	TowerDefensePlayerStats::~TowerDefensePlayerStats()
	{
		
	}

    void TowerDefensePlayerStats::tick(float dt)
	{
		SUPER(TowerDefensePlayerStats, tick, dt);
	}
	
    void TowerDefensePlayerStats::XMLPort(Element& xmlelement, XMLPort::Mode mode)
	{
		SUPER(TowerDefensePlayerStats, XMLPort, xmlelement, mode);
	}
	
    void TowerDefensePlayerStats::changedOwner()
    {
        SUPER(TowerDefensePlayerStats, changedOwner);
		/*
        if (this->getOwner() != NULL && this->getOwner()->getGametype())
            this->owner_ = orxonox_cast<Pong*>(this->getOwner()->getGametype().get());
        else
            this->owner_ = 0;
			*/
    }
	
}
