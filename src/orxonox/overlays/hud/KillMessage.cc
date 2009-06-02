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

#include "KillMessage.h"

#include "core/CoreIncludes.h"
#include "objects/infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(KillMessage);

    KillMessage::KillMessage(BaseObject* creator) : FadeoutText(creator)
    {
        RegisterObject(KillMessage);

        this->owner_ = 0;

        this->setDelay(2.0f);
        this->setFadeouttime(0.5f);
    }

    void KillMessage::killmessage(const GametypeInfo* gtinfo, const std::string& message)
    {
        if (this->owner_ && this->owner_->getGametypeInfo() == gtinfo)
            this->setCaption(message);
    }

    void KillMessage::changedOwner()
    {
        SUPER(KillMessage, changedOwner);

        this->owner_ = dynamic_cast<PlayerInfo*>(this->getOwner());
    }
}
