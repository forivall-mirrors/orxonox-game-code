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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "GUIOverlay.h"

#include <string>

#include "util/Convert.h"
#include "core/input/InputManager.h"
#include "core/CoreIncludes.h"
#include "core/GUIManager.h"
#include "core/XMLPort.h"
#include "infos/PlayerInfo.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    CreateFactory(GUIOverlay);

    GUIOverlay::GUIOverlay(Context* context) : OrxonoxOverlay(context)
    {
        RegisterObject(GUIOverlay);
    }

    GUIOverlay::~GUIOverlay()
    {
    }

    void GUIOverlay::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GUIOverlay, XMLPort, xmlelement, mode);

        XMLPortParam(GUIOverlay, "guiname", setGUIName, getGUIName, xmlelement, mode);
    }

    void GUIOverlay::changedVisibility()
    {
        SUPER(GUIOverlay, changedVisibility);

        //Setting player now.
        if (this->getOwner())
        {
            ControllableEntity* entity = orxonox_cast<ControllableEntity*>(this->getOwner());
            if (entity)
                GUIManager::getInstance().setPlayer(this->guiName_, entity->getPlayer());
        }

        if (this->isVisible())
        {
            const std::string& str = multi_cast<std::string>(reinterpret_cast<long>(this));
            GUIManager::getInstance().showGUIExtra(this->guiName_, str);

            orxout(verbose, context::misc::gui) << "Showing GUI " << this->guiName_ << endl;
        }
        else
        {
            GUIManager::hideGUI(this->guiName_);
            orxout(verbose, context::misc::gui) << "Hiding GUI " << this->guiName_ << endl;
        }
    }

    void GUIOverlay::setGUIName(const std::string& name)
    {
        this->guiName_ = name;
        if (this->getOwner())
        {
            ControllableEntity* entity = orxonox_cast<ControllableEntity*>(this->getOwner());
            if (entity)
                GUIManager::getInstance().setPlayer(name, entity->getPlayer()); //Set Player is going to be NULL, so it needs to be set in changedVisibility() as well.
        }
    }

    void GUIOverlay::changedOwner()
    {
        SUPER(GUIOverlay, changedOwner);
        if (!this->getGUIName().empty())
            this->setGUIName(this->getGUIName());
    }
}
