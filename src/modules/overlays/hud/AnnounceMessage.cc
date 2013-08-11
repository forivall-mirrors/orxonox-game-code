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

#include "AnnounceMessage.h"

#include "core/CoreIncludes.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    RegisterClass(AnnounceMessage);

    AnnounceMessage::AnnounceMessage(Context* context) : FadeoutText(context)
    {
        RegisterObject(AnnounceMessage);

        this->owner_ = 0;

        this->setDelay(3.0f);
        this->setFadeouttime(1.0f);
    }

    void AnnounceMessage::announcemessage(const GametypeInfo* gtinfo, const std::string& message)
    {
        if (this->owner_ && this->owner_->getGametypeInfo() == gtinfo)
            this->setCaption(message);
    }

    void AnnounceMessage::changedOwner()
    {
        SUPER(AnnounceMessage, changedOwner);

        this->owner_ = orxonox_cast<PlayerInfo*>(this->getOwner());
    }
}
