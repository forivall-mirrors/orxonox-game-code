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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "MovableEntity.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "core/command/Executor.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    static const float MAX_RESYNCHRONIZE_TIME = 3.0f;
    static const float CONTINUOUS_SYNCHRONIZATION_TIME = 10.0f;

    CreateFactory(MovableEntity);

    MovableEntity::MovableEntity(Context* context) : MobileEntity(context)
    {
        RegisterObject(MovableEntity);

        this->overwrite_position_    = Vector3::ZERO;
        this->overwrite_orientation_ = Quaternion::IDENTITY;

        this->continuousResynchroTimer_ = 0;

        this->setPriority(Priority::Low);

        this->registerVariables();
    }

    MovableEntity::~MovableEntity()
    {
        if (this->isInitialized())
            if (this->continuousResynchroTimer_)
                this->continuousResynchroTimer_->destroy();
    }

    void MovableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MovableEntity, XMLPort, xmlelement, mode);

        XMLPortParam(MovableEntity, "enablecollisiondamage", setEnableCollisionDamage, getEnableCollisionDamage, xmlelement, mode).defaultValues(false);
        XMLPortParam(MovableEntity, "collisiondamage", setCollisionDamage, getCollisionDamage, xmlelement, mode).defaultValues(1);
    }

    bool MovableEntity::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        if (GameMode::isMaster() && enableCollisionDamage_)
        {
            Pawn* victim = orxonox_cast<Pawn*>(otherObject);
            if (victim)
            {
                float damage = this->collisionDamage_ * (victim->getVelocity() - this->getVelocity()).length();
                victim->hit(0, contactPoint, damage);
            }
        }

        return false;
    }


    void MovableEntity::registerVariables()
    {
        registerVariable(this->linearVelocity_,        VariableDirection::ToClient, new NetworkCallback<MovableEntity>(this, &MovableEntity::processLinearVelocity));
        registerVariable(this->angularVelocity_,       VariableDirection::ToClient, new NetworkCallback<MovableEntity>(this, &MovableEntity::processAngularVelocity));

        registerVariable(this->overwrite_position_,    VariableDirection::ToClient, new NetworkCallback<MovableEntity>(this, &MovableEntity::overwritePosition));
        registerVariable(this->overwrite_orientation_, VariableDirection::ToClient, new NetworkCallback<MovableEntity>(this, &MovableEntity::overwriteOrientation));
    }

    void MovableEntity::clientConnected(unsigned int clientID)
    {
        this->resynchronizeTimer_.setTimer(rnd() * MAX_RESYNCHRONIZE_TIME, false, createExecutor(createFunctor(&MovableEntity::resynchronize, this)));
    }

    void MovableEntity::clientDisconnected(unsigned int clientID)
    {
    }

    void MovableEntity::resynchronize()
    {
        if (GameMode::isMaster() && !this->continuousResynchroTimer_)
        {
            // Resynchronise every few seconds because we only work with velocities (no positions)
            continuousResynchroTimer_ = new Timer(CONTINUOUS_SYNCHRONIZATION_TIME + rnd(-1, 1),
                true, createExecutor(createFunctor(&MovableEntity::resynchronize, this)), false);
        }

        this->overwrite_position_ = this->getPosition();
        this->overwrite_orientation_ = this->getOrientation();
    }
}
