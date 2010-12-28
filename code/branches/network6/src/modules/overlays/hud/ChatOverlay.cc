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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "ChatOverlay.h"

#include <string>
#include <OgreTextAreaOverlayElement.h>

#include "util/Convert.h"
#include "util/DisplayStringConversions.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/command/Executor.h"

#include "tools/Timer.h"
#include "infos/PlayerInfo.h"
#include "PlayerManager.h"

namespace orxonox
{
    CreateFactory(ChatOverlay);

    ChatOverlay::ChatOverlay(BaseObject* creator)
        : OverlayText(creator)
    {
        RegisterObject(ChatOverlay);

        this->displayTime_ = 0.0;

        this->setConfigValues();
    }

    ChatOverlay::~ChatOverlay()
    {
        for (std::set<Timer*>::iterator it = this->timers_.begin(); it != this->timers_.end(); ++it)
            delete (*it);
    }

    void ChatOverlay::setConfigValues()
    {
        SetConfigValue(displayTime_, 6.0f);
    }

    void ChatOverlay::incomingChat(const std::string& message, unsigned int senderID)
    {
        std::string text;

        if (senderID != NETWORK_PEER_ID_UNKNOWN)
        {
            std::string name = "unknown";

            PlayerInfo* player = PlayerManager::getInstance().getClient(senderID);
            if (player)
                name = player->getName();

            text = name + ": " + message;
        }
        else
        {
            text = message;
        }

        this->messages_.push_back(multi_cast<Ogre::DisplayString>(text));
        COUT(0) << "Chat: " << text << std::endl;

        Timer* timer = new Timer();
        this->timers_.insert(timer); // store the timer in a set to destroy it in the destructor
        const ExecutorPtr& executor = createExecutor(createFunctor(&ChatOverlay::dropMessage, this));
        executor->setDefaultValues(timer);
        timer->setTimer(this->displayTime_, false, executor, true);

        this->updateOverlayText();
    }

    void ChatOverlay::dropMessage(Timer* timer)
    {
        if (this->messages_.size() > 0)
            this->messages_.pop_front();
        this->updateOverlayText();
        this->timers_.erase(timer); // the timer destroys itself, but we have to remove it from the set
    }

    void ChatOverlay::updateOverlayText()
    {
        this->text_->setCaption("");

        for (std::list<Ogre::DisplayString>::reverse_iterator it = this->messages_.rbegin(); it != this->messages_.rend(); ++it)
        {
            this->text_->setCaption(this->text_->getCaption() + "\n" + (*it));
        }
    }
}
