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

#ifndef _SpaceRace_H__
#define _SpaceRace_H__

#include "gametypes/Gametype.h"
#include "gametypes/GametypesPrereqs.h"
#include "RaceCheckPoint.h"
#include <boost/concept_check.hpp>
#include <util/Clock.h>
#include <string.h>
#include <set>

namespace orxonox
{
/*    class PlayerScore {
	public:
	    PlayerScore() {
		this->name = "";
		this->time =0;
	    }
	    PlayerScore(std::string name, float time) {
		this->name_ = name;
		this->time_ = time;
	    }
	    PlayerScore(float time) {
		this->name_ = "Player";
		this->time_ = time;
	    }
	
	private:
	    std::string name_;
	    float time_;
    };*/
	
    class _OrxonoxExport SpaceRace : public Gametype
    {
	public:
	    SpaceRace(BaseObject* creator);
	    virtual ~SpaceRace() {}
	    
	    virtual void tick(float dt);
	    
	    virtual void start();
            virtual void end();
	    
	    virtual void newCheckpointReached();
	    
	    inline void setCheckpointsReached(int n)
		{ this->checkpointsReached_ = n;}
	    inline int getCheckpointsReached()
		{ return this->checkpointsReached_; }
	    inline void timeIsUp()
		{ this->bTimeIsUp_ = true;}
	    
	protected:
	    
	private:
	    int checkpointsReached_;
	    std::set<float> scores_;
	    Clock *clock_;
	    bool bTimeIsUp_;
    };
}

#endif
