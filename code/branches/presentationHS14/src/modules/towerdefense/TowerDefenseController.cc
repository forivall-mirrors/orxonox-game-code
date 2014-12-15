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

#include "TowerDefenseController.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
#include "controllers/WaypointController.h"

namespace orxonox
{
    RegisterClass(TowerDefenseController);

    TowerDefenseController::TowerDefenseController(Context* context) : WaypointController(context)
    {
        RegisterObject(TowerDefenseController);

        this->alertnessradius_ = 500;

        this->patrolTimer_.setTimer(rnd(), true, createExecutor(createFunctor(&TowerDefenseController::searchEnemy, this)));
    }

    void TowerDefenseController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TowerDefenseController, XMLPort, xmlelement, mode);

        XMLPortParam(TowerDefenseController, "alertnessradius", setAlertnessRadius, getAlertnessRadius, xmlelement, mode).defaultValues(500.0f);
    }


    void TowerDefenseController::tick(float dt)
    {
        if (!this->isActive())
            return;

        if (this->target_)
        {
            this->aimAtTarget();

            if (this->bHasTargetPosition_)
                this->moveToTargetPosition();

            if (this->getControllableEntity() && this->isCloseAtTarget(1000) && this->isLookingAtTarget(math::pi / 20.0f))
                this->getControllableEntity()->fire(0);
        }
        else
        {
            SUPER(TowerDefenseController, tick, dt);
        }
    }


    bool sameTeam(ControllableEntity* entity1, ControllableEntity* entity2, Gametype* gametype)
    {
        if (entity1 == entity2)
            return true;

        int team1 = -1;
        int team2 = -1;

        Controller* controller = 0;
        if (entity1->getController())
            controller = entity1->getController();
        else
            controller = entity1->getXMLController();
        if (controller)
        {
            FormationController* ac = orxonox_cast<FormationController*>(controller);
            if (ac)
                team1 = ac->getTeam();
        }

        if (entity2->getController())
            controller = entity2->getController();
        else
            controller = entity2->getXMLController();
        if (controller)
        {
            FormationController* ac = orxonox_cast<FormationController*>(controller);
            if (ac)
                team2 = ac->getTeam();
        }



        return (team1 == team2 && team1 != -1);
    }





    void TowerDefenseController::searchEnemy()
    {
        this->patrolTimer_.setInterval(rnd());

        if (!this->getControllableEntity())
            return;

        Vector3 myposition = this->getControllableEntity()->getPosition();
        float shortestsqdistance = (float)static_cast<unsigned int>(-1);

        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
        {
            if (ArtificialController::sameTeam(this->getControllableEntity(), static_cast<ControllableEntity*>(*it), this->getGametype()))
                continue;

            float sqdistance = it->getPosition().squaredDistance(myposition);
            if (sqdistance < shortestsqdistance)
            {
                shortestsqdistance = sqdistance;
                this->target_ = (*it);
            }
        }

        if (shortestsqdistance > (this->alertnessradius_ * this->alertnessradius_))
            this->target_ = 0;
    }
}
