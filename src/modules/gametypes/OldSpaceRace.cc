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

#include "OldSpaceRace.h"

#include "core/CoreIncludes.h"
#include "chat/ChatManager.h"
#include "util/Convert.h"
#include "util/Math.h"

namespace orxonox
{
    CreateUnloadableFactory(OldSpaceRace);

    OldSpaceRace::OldSpaceRace(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(OldSpaceRace);
        this->checkpointsReached_ = 0;
        this->bTimeIsUp_ = false;
        this->numberOfBots_ = 0;
    }

    void OldSpaceRace::end()
    {
        this->Gametype::end();

        if (this->bTimeIsUp_)
        {
            this->clock_.capture();
            int s = this->clock_.getSeconds();
            int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
            const std::string& message = multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms) + " seconds !!\n"
                        + "You didn't reach the check point " + multi_cast<std::string>(this->checkpointsReached_+1)
                        + " before the time limit. You lose!";
            this->getGametypeInfo()->sendAnnounceMessage(message);
            ChatManager::message(message);
        }
        else
        {
            this->clock_.capture();
            int s = this->clock_.getSeconds();
            int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
            const std::string& message = "You win!! You have reached the last check point after "+ multi_cast<std::string>(s)
                        + "." + multi_cast<std::string>(ms) + " seconds.";
            this->getGametypeInfo()->sendAnnounceMessage(message);
            ChatManager::message(message);
/*
            float time = this->clock_.getSecondsPrecise();
            this->scores_.insert(time);
            std::set<float>::iterator it;
            for (it=this->scores_.begin(); it!=this->scores_.end(); it++)
                orxout(level::message) << multi_cast<std::string>(*it) << endl;
*/
        }
    }

    void OldSpaceRace::start()
    {
        Gametype::start();

        std::string message("The match has started! Reach the check points as quickly as possible!");
        this->getGametypeInfo()->sendAnnounceMessage(message);
        ChatManager::message(message);
    }

    void OldSpaceRace::newCheckpointReached()
    {
        this->checkpointsReached_++;
        this->clock_.capture();
        int s = this->clock_.getSeconds();
        int ms = static_cast<int>(this->clock_.getMilliseconds()-1000*s);
        const std::string& message = "Checkpoint " + multi_cast<std::string>(this->getCheckpointsReached())
                        + " reached after " + multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms)
                        + " seconds.";
        this->getGametypeInfo()->sendAnnounceMessage(message);
        ChatManager::message(message);
    }

}
