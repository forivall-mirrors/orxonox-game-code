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
 *
 *   Co-authors:
 *      ...
 */

#include "TowerDefenseHUDController.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"

namespace orxonox
{
    RegisterClass(TowerDefenseHUDController);

    TowerDefenseHUDController::TowerDefenseHUDController(Context* context) : OverlayText(context)
    {
        RegisterObject(TowerDefenseHUDController);
        this->td = 0;
    }

    TowerDefenseHUDController::~TowerDefenseHUDController()
    {

    }

    void TowerDefenseHUDController::tick(float dt)
    {
        SUPER(TowerDefenseHUDController, tick, dt);
        const std::string& lifes = multi_cast<std::string>(this->td->getLifes());
        const std::string& credits = multi_cast<std::string>(this->td->getCredit());
        const std::string& waves = multi_cast<std::string>(this->td->getWaveNumber());

        if(showlives == true)
          this->setCaption(multi_cast<std::string>(lifes));
        else if(showcredits == true)
          this->setCaption(multi_cast<std::string>(credits));
        else if(showwaves == true)
          this->setCaption(multi_cast<std::string>(waves));


    }

    void TowerDefenseHUDController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TowerDefenseHUDController, XMLPort, xmlelement, mode);

        XMLPortParam(TowerDefenseHUDController, "showlives", setShowlives, getShowlives, xmlelement, mode);
        XMLPortParam(TowerDefenseHUDController, "showcredits", setShowcredits, getShowcredits, xmlelement, mode);
        XMLPortParam(TowerDefenseHUDController, "showwaves", setShowwaves, getShowwaves, xmlelement, mode);

    }

    void TowerDefenseHUDController::changedOwner()
        {
            SUPER(TowerDefenseHUDController, changedOwner);

            if (this->getOwner() && this->getOwner()->getGametype())
                    {
                        this->td = orxonox_cast<TowerDefense*>(this->getOwner()->getGametype().get());
                    }
                    else
                    {
                        this->td = 0;
                    }
                }
            /*if (this->getOwner() != NULL && this->getOwner()->getGametype())
            {
                this->owner_ = orxonox_cast<TowerDefense*>(this->getOwner()->getGametype().get());
            }
            else
            {
                this->owner_ = NULL;
            }*/
        }

