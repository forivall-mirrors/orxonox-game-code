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
 *     Mauro Salomon
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceRace.h"

#include "core/CoreIncludes.h"
#include "network/Host.h"
#include <util/Clock.h>
#include <util/Math.h>
#include "util/Convert.h"

namespace orxonox
{
    CreateUnloadableFactory(SpaceRace);
    
    SpaceRace::SpaceRace(BaseObject* creator) : Gametype(creator)
    {
	RegisterObject(SpaceRace);
	this->checkpointsReached_ = 0;
	this->bTimeIsUp_ = false;
	this->numberOfBots_ = 0;
    }
    
    void SpaceRace::tick(float dt)
    {
	Gametype::tick(dt);
    }
    
    void SpaceRace::end()
    {
      	Gametype::end();
	this->stopTimer();
	if (this->bTimeIsUp_) {
	    COUT(0) << "Time is up" << std::endl;
	    const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage("Time is up");
	}
	else {
	    this->clock_->capture();
	    int s = this->clock_->getSeconds();
	    int ms = this->clock_->getMilliseconds()-1000*s;
	    const std::string& message = "You have reached the last check point after "+ multi_cast<std::string>(s) +
					  "." + multi_cast<std::string>(ms) + " seconds.";
	    COUT(0) << message << std::endl;
	    const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
	    Host::Broadcast(message);
	    float time = this->clock_->getSecondsPrecise();
	    this->scores_.insert(time);
	    std::set<float>::iterator it;
	    for (it=this->scores_.begin(); it!=this->scores_.end(); it++)
	    COUT(0) <<  multi_cast<std::string>(*it) << std::endl;
	}
    }

    void SpaceRace::start()
    {
	Gametype::start();
	
	this->startTimer();
	clock_= new Clock();
	std::string message("The match has started! Reach the check points as quickly as possible!");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }
    
    void SpaceRace::newCheckpointReached()
    {
      	this->checkpointsReached_++;
	this->clock_->capture();
	int s = this->clock_->getSeconds();
	int ms = this->clock_->getMilliseconds()-1000*s;
	const std::string& message = "Checkpoint " + multi_cast<std::string>(this->getCheckpointsReached()) 
				      + " reached after " + multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms)
				      + " seconds.";
	COUT(0) << message << std::endl;
	const_cast<GametypeInfo*>(this->getGametypeInfo())->sendAnnounceMessage(message);
	Host::Broadcast(message);
    }

}