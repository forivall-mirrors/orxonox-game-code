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

#include "OrxonoxStableHeaders.h"
#include "Controller.h"

#include "core/CoreIncludes.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateUnloadableFactory(Controller);

    Controller::Controller(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Controller);

        this->player_ = 0;
        this->controllableEntity_ = 0;
        this->hud_ = 0;
        this->bUpdateHUD_ = false;
    }

    Controller::~Controller()
    {
        if (this->isInitialized() && this->hud_)
            delete this->hud_;
    }

    void Controller::changedControllableEntity()
    {
        if (this->bUpdateHUD_)
        {
            this->updateHUD();
            this->bUpdateHUD_ = false;
        }

        if (this->hud_)
            this->hud_->setOwner(this->getControllableEntity());
    }

    void Controller::updateHUD()
    {
        if (this->hud_)
        {
            delete this->hud_;
            this->hud_ = 0;
        }

        if (this->hudtemplate_ != "")
        {
            this->hud_ = new OverlayGroup(this);
            this->hud_->addTemplate(this->hudtemplate_);
            this->hud_->setOwner(this->getControllableEntity());
        }
    }
}
