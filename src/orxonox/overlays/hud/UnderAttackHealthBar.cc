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

#include "UnderAttackHealthBar.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/gametypes/UnderAttack.h"
#include "objects/worldentities/pawns/Destroyer.h"

namespace orxonox
{
    CreateFactory(UnderAttackHealthBar);

    UnderAttackHealthBar::UnderAttackHealthBar(BaseObject* creator) : HUDHealthBar(creator)
    {
        RegisterObject(UnderAttackHealthBar);

        this->owner_ = 0;

        this->text_ = new OverlayText(this);
        this->text_->setFont("Monofur");
        this->text_->setTextSize(0.04);
        this->text_->setAlignmentString("center");
        this->text_->setColour(ColourValue::White);
        this->text_->setPickPoint(Vector2(0.5, 0));

        this->inittimer_.setTimer(0.0f, false, this, createExecutor(createFunctor(&UnderAttackHealthBar::init)));
    }

    UnderAttackHealthBar::~UnderAttackHealthBar()
    {
        if (this->isInitialized())
            delete this->text_;
    }

    void UnderAttackHealthBar::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(UnderAttackHealthBar, XMLPort, xmlelement, mode);

        XMLPortParam(UnderAttackHealthBar, "descriptionpickpoint", setDescriptionPickPoint, getDescriptionPickPoint, xmlelement, mode);
        XMLPortParam(UnderAttackHealthBar, "descriptionoffset", setDescriptionOffset, getDescriptionOffset, xmlelement, mode);
    }

    void UnderAttackHealthBar::changedOwner()
    {
        SUPER(UnderAttackHealthBar, changedOwner);

        PlayerInfo* player = dynamic_cast<PlayerInfo*>(this->getOwner());
        if (player)
        {
            this->owner_ = player;

            UnderAttack* ua = dynamic_cast<UnderAttack*>(player->getGametype());
            if (ua)
            {
                this->setOwner(ua->getDestroyer());

                if (ua->getTeam(player) == 0)
                    this->text_->setCaption("Attack the Transporter!");
                else
                    this->text_->setCaption("Defend the Transporter!");
            }
        }
    }

    void UnderAttackHealthBar::init()
    {
        this->setOwner(this->owner_);
    }
}
