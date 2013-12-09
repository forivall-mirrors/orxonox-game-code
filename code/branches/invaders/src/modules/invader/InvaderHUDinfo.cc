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

#include "InvaderHUDinfo.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"
// #include "Invader.h"

namespace orxonox
{
    RegisterClass(InvaderHUDinfo);

    InvaderHUDinfo::InvaderHUDinfo(Context* context) : OverlayText(context)
    {
        RegisterObject(InvaderHUDinfo);

        this->InvaderGame = 0;
        this->bShowLives_ = false;
        this->bShowLevel_ = false;
        this->bShowPoints_ = false;
        this->bShowMultiplier_ = false;
    }

    void InvaderHUDinfo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(InvaderHUDinfo, XMLPort, xmlelement, mode);

        XMLPortParam(InvaderHUDinfo, "showlives",     setShowLives,     getShowLives,     xmlelement, mode).defaultValues(false);
        XMLPortParam(InvaderHUDinfo, "showPoints",     setShowPoints,     getShowPoints,     xmlelement, mode).defaultValues(false);
        XMLPortParam(InvaderHUDinfo, "showLevel",     setShowLevel,     getShowLevel,     xmlelement, mode).defaultValues(false);
        XMLPortParam(InvaderHUDinfo, "showMultiplier",     setShowMultiplier,     getShowMultiplier,     xmlelement, mode).defaultValues(false);
    }

    void InvaderHUDinfo::tick(float dt)
    {
        SUPER(InvaderHUDinfo, tick, dt);

        if (this->InvaderGame)
        {
            if (this->bShowLives_)
            {
                const std::string& lives = multi_cast<std::string>(this->InvaderGame->getLives());
                this->setCaption(lives);
            }
            else if(this->bShowLevel_)
            {
                const std::string& Level = multi_cast<std::string>(this->InvaderGame->getLevel());
                if (this->InvaderGame->lives <= 0)
                {
                    setPosition(Vector2(0.1, 0.65));
                    this->setCaption("Game ends in 30 seconds.\nPress (e)xit / (q)uit to go to the main menu.\nTo restart fly out of the screen!");
                    setTextSize(0.05);
                    this->InvaderGame->bEndGame = true;
                }
                else if (this->InvaderGame->bShowLevel)
                {
                    setTextSize(0.1);
                    setPosition(Vector2(0.3, 0.55));
                    std::stringstream sstm;
                    sstm << "Level " << Level;
                    this->setCaption(sstm.str()); // + level
                }
                else
                {
                    setTextSize(0.04);
                    setPosition(Vector2(0.14, 0.055));
                    this->setCaption(Level);
                }
            }
            else if(this->bShowPoints_)
            {
                const std::string& points = multi_cast<std::string>(this->InvaderGame->getPoints());
                if (this->InvaderGame->lives <= 0)
                {
                    setTextSize(0.2);
                    setPosition(Vector2(0.1, 0.25));
                    this->setCaption("Final score:\n" + points);
                    this->setColour(ColourValue(1, 0, 0, 1));
                }
                else
                {
                    setTextSize(0.04);
                    setPosition(Vector2(0.14, 0.1));
                    this->setColour(ColourValue(1, 1, 1, 1));
                    this->setCaption(points);
                }
            }
            else if(this->bShowMultiplier_)
            {
                int mult = this->InvaderGame->getMultiplier();
                const std::string& Multiplier = "X " + multi_cast<std::string>(mult);
                this->setCaption(Multiplier);
                this->setColour(ColourValue(1, 0, 0, clamp(float(mult * 0.1), 0.0f, 1.0f)));
                this->setTextSize(clamp(float(mult * 0.1), 0.0f, 1.0f) * 0.01 + 0.04);
            }
        }
    }

    void InvaderHUDinfo::changedOwner()
    {
        SUPER(InvaderHUDinfo, changedOwner);

        if (this->getOwner() && this->getOwner()->getGametype())
        {
            this->InvaderGame = orxonox_cast<Invader*>(this->getOwner()->getGametype().get());
        }
        else
        {
            this->InvaderGame = 0;
        }
    }
}
