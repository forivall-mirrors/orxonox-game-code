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
 *   GNU General Public License for more detls.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Johannes Ritz
 *   Co-authors:
 *      
 *
 */

#include "Detector.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    const float NOT_UNDER_ATTACK_LIMIT=2.0f;

    CreateFactory(Detector);

    Detector::Detector(BaseObject* creator) : ArtificialController(creator)
    {
        RegisterObject(Detector);
        this->healthLost = 0;
        this->notUnderAttackTime = 2.0f;
        this->bUnderAttack = false;
        this->bot_ = orxonox_cast<Pawn*>(this->getControllableEntity());
        if(bot_)
            this->previousHealth = bot_->getHealth();
        else
            this->previousHealth = 0;
        this->botLevel = 10.0f;
        this->attacker_=0;
    }

    Detector::~Detector()
    {
    }
    
    void Detector::tick(float dt) //TODO: set a checking interval depending on ai-level
    {
        SUPER(Detector, tick, dt);
        
        notUnderAttackTime += dt;
        if(bot_)
        {
            this->checkHealth();
        }
        else
        {
            bot_ = orxonox_cast<Pawn*>(this->getControllableEntity());
        }
        bUnderAttack = this->checkUnderAttack();
    }
    
    void Detector::checkHealth() //finds out if bot lost life
    {
        float actualHealth = this->bot_->getHealth();
        healthLost = previousHealth - actualHealth;
        if (healthLost>0)
        {
            notUnderAttackTime=0;
            this->analyseAttack();
        }
        previousHealth = actualHealth;
    }
    
    bool Detector::checkUnderAttack()
    {
        return (NOT_UNDER_ATTACK_LIMIT > notUnderAttackTime);
    }
    
    void Detector::analyseAttack()
    {
        if (attacker_)//attacker is already known
            return;
        else
        {
        ;
        }       
        ;//?? from where came the attack;
        //Kombination aus isLooking at und object list und ???
        //idea: if the attacker is viewable he is known. Else the attacking direction is know.
        //callForHelp-Function to friendly pawns
    }
}
