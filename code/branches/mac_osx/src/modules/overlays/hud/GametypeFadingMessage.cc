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

#include "GametypeFadingMessage.h"

#include "core/CoreIncludes.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(GametypeFadingMessage);

    GametypeFadingMessage::GametypeFadingMessage(BaseObject* creator) : FadeoutText(creator)
    {
        RegisterObject(GametypeFadingMessage);

        this->owner_ = 0;
        this->setDelay(2.0f);
        this->setFadeouttime(0.5f);
    }

    GametypeFadingMessage::~GametypeFadingMessage()
    {
    }

    void GametypeFadingMessage::fadingmessage(const GametypeInfo* gtinfo, const std::string& message)
    {
        if (this->owner_ && this->owner_->getGametypeInfo() == gtinfo)
            this->setCaption(message);
    }

    void GametypeFadingMessage::changedOwner()
    {
        SUPER(GametypeFadingMessage, changedOwner);
        this->owner_ = orxonox_cast<PlayerInfo*>(this->getOwner());
    }
}
