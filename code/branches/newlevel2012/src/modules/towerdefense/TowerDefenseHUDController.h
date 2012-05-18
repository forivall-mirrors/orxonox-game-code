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
	 
	 @ingroup TowerDefense
 */
	 

#ifndef _TowerDefenseHUDController_H__
#define _TowerDefenseHUDController_H__

#include "towerdefense/TowerDefensePrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"


namespace orxonox
{
    class _TowerDefenseExport TowerDefenseHUDController : public OverlayText, public Tickable
    {
	public:
		TowerDefenseHUDController(BaseObject* creator);
        virtual ~TowerDefenseHUDController();

        virtual void tick(float dt);
        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
		virtual void changedOwner();
		
		inline int getCredit()
			{ return credit_; }
		
		inline void setCredit(int credit)
			{ credit_ = credit; }
		
		inline int getWaveNumber()
			{ return waveNumber_; }
		
		inline void didLoadNextWave()
			{ waveNumber_++; }
		
		
		
	private:
		int credit_;
		int waveNumber_;
		//int baseHealth_;
    };
}

#endif /* _TowerDefenseHUDController_H__ */
