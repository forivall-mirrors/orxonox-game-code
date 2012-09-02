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
*     Gabriel Nadler, Originalfile: Oli Scheuss
*   Co-authors:
*      ...
*
*/

#include "RocketController.h"
#include "projectiles/SimpleRocket.h"
#include "util/Math.h"
#include "weapons/projectiles/SimpleRocket.h"
#include "util/Debug.h"
#include "weapons/weaponmodes/SimpleRocketFire.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    /**
    @brief
    Constructor.
    */
    RocketController::RocketController(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(RocketController);
        COUT(5)<< "RocketController constructed\n";


        this->rocket_ = new SimpleRocket(this);
        this->rocket_->setController(this);
        this->setControllableEntity(dynamic_cast<ControllableEntity*> (this->rocket_));
    }


    /**
    @brief
    The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
    The duration of the tick.
    */
    void RocketController::tick(float dt)
    {

        if (this->target_ && this->rocket_->hasFuel()) {
            this->setTargetPosition();
            this->moveToTargetPosition();
        }


    }


    RocketController::~RocketController()
    {
        COUT(5)<< "RocketController destroyed\n";
    }

    void RocketController::setTargetPosition()
    {
        this->targetPosition_=this->target_->getWorldPosition(); //don't really note a difference in the rocket behaviour xD
        //this->targetPosition_ = getPredictedPosition(this->getControllableEntity()->getWorldPosition(),this->getControllableEntity()->getVelocity().length() , this->target_->getWorldPosition(), this->target_->getVelocity());
    }
    void RocketController::moveToTargetPosition()
    {
        this->moveToPosition(this->targetPosition_);
    }



    void RocketController::setTarget(WorldEntity* target)
    {
        this->target_ = target;
    }

    void RocketController::moveToPosition(const Vector3& target)
    {
        if (!this->getControllableEntity())
            return;

        Vector2 coord = get2DViewdirection(this->rocket_->getPosition(), this->rocket_->getOrientation() * WorldEntity::FRONT, this->rocket_->getOrientation() * WorldEntity::UP, target);
        float distance = (target - this->rocket_->getWorldPosition()).length();


        if (distance > 1000 && this->rocket_->getVelocity().squaredLength()<160000)
            this->rocket_->setAcceleration(this->rocket_->getOrientation()*Vector3(-20,-20,-20));
        if (distance <1000) this->rocket_->setAcceleration(0,0,0);

        this->rocket_->rotateYaw(-sgn(coord.x)*coord.x*coord.x);
        this->rocket_->rotatePitch(sgn(coord.y)*coord.y*coord.y);
    }




}
