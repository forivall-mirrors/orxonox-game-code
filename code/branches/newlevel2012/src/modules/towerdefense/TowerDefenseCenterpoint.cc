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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
    @file TowerDefenseCenterpoint.cc
    @brief Implementation of the TowerDefenseCenterpoint class.
*/

#include "TowerDefenseCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "TowerDefense.h"

namespace orxonox
{
    CreateFactory(TowerDefenseCenterpoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually TowerDefense.
    */
    TowerDefenseCenterpoint::TowerDefenseCenterpoint(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(TowerDefenseCenterpoint);

        this->width_ = 10;
        this->height_ = 11;
        this->towerTemplate_ = "";
        
        this->checkGametype();
    }

    /**
    @brief
        Method to create a TowerDefenseCenterpoint through XML.
    */
    void TowerDefenseCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TowerDefenseCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(TowerDefenseCenterpoint, "width", setWidth, getWidth, xmlelement, mode); // die Breite
        XMLPortParam(TowerDefenseCenterpoint, "height", setHeight, setWidth, xmlelement, mode); // die Grösse
        XMLPortParam(TowerDefenseCenterpoint, "towerTemplate", setTowerTemplate, getTowerTemplate, xmlelement, mode);
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void TowerDefenseCenterpoint::changedGametype()
    {
        SUPER(TowerDefenseCenterpoint, changedGametype);

        // Check, whether it's still TowerDefense.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is TowerDefense and if it is, sets its centerpoint.
    */
    void TowerDefenseCenterpoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(TowerDefense)))
        {
            //TowerDefense* TowerDefenseGametype = orxonox_cast<TowerDefense*>(this->getGametype().get());
            //TowerDefenseGametype->setCenterpoint(this);
        }
    }
}
