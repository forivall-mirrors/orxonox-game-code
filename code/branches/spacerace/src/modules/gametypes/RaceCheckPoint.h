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
 *      Mauro Salomon
 *   Co-authors:
 *      ...
 *
 */

#ifndef _RaceCheckPoint_H__
#define _RaceCheckPoint_H__

#include "objects/ObjectsPrereqs.h"

#include "objects/triggers/DistanceTrigger.h"
#include "interfaces/RadarViewable.h"
#include <boost/concept_check.hpp>

namespace orxonox
{
    class _ObjectsExport RaceCheckPoint : public DistanceTrigger, public RadarViewable
    {
    public:
	RaceCheckPoint(BaseObject* creator);
	virtual ~RaceCheckPoint();
	
	virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	virtual void tick(float dt);
	
    protected:
	virtual void triggered(bool bIsTriggered);
	inline void setLast(bool isLast)
	    { this->bIsLast_ = isLast; }
	inline bool getLast()
	    { return this->bIsLast_; }
	inline void setCheckpointIndex(int checkpointIndex)
	    { this->bCheckpointIndex_ = checkpointIndex; }
	inline int getCheckpointIndex()
	    { return this->bCheckpointIndex_; }
	inline const WorldEntity* getWorldEntity() const
            { return this; }
	
    private:
	int bCheckpointIndex_;
	bool bIsLast_;
	
      
    };
}

#endif