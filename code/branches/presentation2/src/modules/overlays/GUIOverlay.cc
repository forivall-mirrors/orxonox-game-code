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
#include <sstream>

#include "core/input/InputManager.h"
#include "core/CoreIncludes.h"
#include "core/GUIManager.h"
#include "core/XMLPort.h"
#include "infos/PlayerInfo.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    CreateFactory(GUIOverlay);

    GUIOverlay::GUIOverlay(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        RegisterObject(GUIOverlay);
    }

    GUIOverlay::~GUIOverlay()
    {
    }

    void GUIOverlay::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(GUIOverlay, XMLPort, xmlElement, mode);

        XMLPortParam(GUIOverlay, "guiname", setGUIName, getGUIName, xmlElement, mode);
    }

    void GUIOverlay::changedVisibility()
    {
        SUPER(GUIOverlay, changedVisibility);

        //Setting player now.
        if( this->getOwner() )
            GUIManager::getInstance().setPlayer(this->guiName_, (orxonox_cast<ControllableEntity*>(this->getOwner()))->getPlayer());

        if (this->isVisible())
        {
            std::ostringstream out;
            out << reinterpret_cast<long>(this);
            const std::string& str = out.str();
            COUT(1) << "GUIManager ptr: " << str << std::endl;
            GUIManager::getInstance().showGUIExtra(this->guiName_, str);

            COUT(3) << "Showing GUI " << this->guiName_ << std::endl;
        }
        else
        {
            GUIManager::hideGUI(this->guiName_);
            COUT(3) << "Hiding GUI " << this->guiName_ << std::endl;
        }
    }

    void GUIOverlay::setGUIName(const std::string& name)
    {
        this->guiName_ = name;
        GUIManager::getInstance().setPlayer(name, orxonox_cast<PlayerInfo*>(this->getOwner())); //Set Player is going to be NULL, so it needs to be set in changedVisibility() as well.
    }
}
