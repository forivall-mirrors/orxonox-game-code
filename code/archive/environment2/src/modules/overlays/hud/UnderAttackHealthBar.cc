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
#include "infos/PlayerInfo.h"
#include "gametypes/UnderAttack.h"
#include "worldentities/pawns/Destroyer.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateFactory(UnderAttackHealthBar);

    UnderAttackHealthBar::UnderAttackHealthBar(BaseObject* creator) : HUDHealthBar(creator)
    {
        RegisterObject(UnderAttackHealthBar);

        this->owner_ = 0;

        this->text_ = new OverlayText(this);
        this->text_->setFont("Monofur");
        this->text_->setTextSize(0.04f);
        this->text_->setAlignmentString("center");
        this->text_->setColour(ColourValue::White);
        this->text_->setPickPoint(Vector2(0.5, 0));

        this->inittimer_.setTimer(0.0f, false, createExecutor(createFunctor(&UnderAttackHealthBar::init, this)));
    }

    UnderAttackHealthBar::~UnderAttackHealthBar()
    {
        if (this->isInitialized())
        {
            this->text_->destroy();
            this->text_ = 0;
        }
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

        PlayerInfo* player = orxonox_cast<PlayerInfo*>(this->getOwner());
        if (player)
        {
            this->owner_ = player;

            UnderAttack* ua = orxonox_cast<UnderAttack*>(player->getGametype().get());
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

    void UnderAttackHealthBar::changedOverlayGroup()
    {
        SUPER(UnderAttackHealthBar, changedOverlayGroup);

        this->getOverlayGroup()->addElement(this->text_.get());
    }

    void UnderAttackHealthBar::init()
    {
        this->setOwner(this->owner_);
    }
}
