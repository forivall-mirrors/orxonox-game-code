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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */

#ifndef _SpaceRace_H__
#define _SpaceRace_H__

#include "Gametype.h"
#include "OrxonoxPrereqs.h"
#include "objects/triggers/DistanceTriggerBeacon.h"
#include <boost/concept_check.hpp>

namespace orxonox
{
    class _OrxonoxExport SpaceRace : public Gametype
    {
	public:
	    SpaceRace(BaseObject* creator);
	    virtual ~SpaceRace(){};
	    
	    virtual void tick(float dt);
	    
	    virtual void start();
	    virtual void end();
	    
	protected:
	    inline void newCheckpointReached()
	      { this->checkpointsReached++; }
	    inline bool lastCheckpointReached()
	      { return (this->checkpointsReached == this->numberOfCheckpoints); }
	private:
	    int numberOfCheckpoints_;
	    int checkpointsReached_;
    
    };
}

#endif