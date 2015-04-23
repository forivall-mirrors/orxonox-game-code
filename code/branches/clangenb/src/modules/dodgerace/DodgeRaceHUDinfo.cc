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
 *      Florian Zinggeler
 *
 */

#include "DodgeRaceHUDinfo.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"
//#include "DodgeRace.h"

namespace orxonox
{
    RegisterClass(DodgeRaceHUDinfo);

    DodgeRaceHUDinfo::DodgeRaceHUDinfo(Context* context) : OverlayText(context)
    {
        RegisterObject(DodgeRaceHUDinfo);

        this->DodgeRaceGame = 0;
        this->bShowPoints_ = true;
    }

    void DodgeRaceHUDinfo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(DodgeRaceHUDinfo, XMLPort, xmlelement, mode);

        XMLPortParam(DodgeRaceHUDinfo,"showPoints", setShowPoints, getShowPoints, xmlelement, mode).defaultValues(false);
    }

    void DodgeRaceHUDinfo::tick(float dt)
    {
        SUPER(DodgeRaceHUDinfo, tick, dt);


        if(this->bShowPoints_)
        {
            const std::string& points = multi_cast<std::string>(this->DodgeRaceGame->getPoints());
            if (this->DodgeRaceGame->lives <= 0)
            {
                setTextSize(0.2);
                setPosition(Vector2(0.1, 0.02));
                this->setCaption("Final score:\n" + points);
                this->setColour(ColourValue(1, 0, 0, 1));
            }
            else
            {
                setTextSize(0.04);
                setPosition(Vector2(0.14, 0.02));
                this->setColour(ColourValue(1, 1, 1, 1));
                this->setCaption(points);
            }
        }
    }

    void DodgeRaceHUDinfo::changedOwner()
    {
        SUPER(DodgeRaceHUDinfo, changedOwner);

        if (this->getOwner() && this->getOwner()->getGametype())
        {
            this->DodgeRaceGame = orxonox_cast<DodgeRace*>(this->getOwner()->getGametype().get());
        }
        else
        {
            this->DodgeRaceGame = 0;
        }
    }
}
