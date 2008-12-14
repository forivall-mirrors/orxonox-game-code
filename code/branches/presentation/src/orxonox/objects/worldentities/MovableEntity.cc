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

#include "OrxonoxStableHeaders.h"
#include "MovableEntity.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Executor.h"
#include "tools/Timer.h"

namespace orxonox
{
    static const float MAX_RESYNCHRONIZE_TIME = 3.0f;
    static const float CONTINUOUS_SYNCHRONIZATION_TIME = 10.0f;

    CreateFactory(MovableEntity);

    MovableEntity::MovableEntity(BaseObject* creator) : MobileEntity(creator)
    {
        RegisterObject(MovableEntity);

        this->overwrite_position_    = Vector3::ZERO;
        this->overwrite_orientation_ = Quaternion::IDENTITY;
        
        this->setPriority( priority::low );

        // Resynchronise every few seconds because we only work with velocities (no positions)
        continuousResynchroTimer_ = new Timer<MovableEntity>(CONTINUOUS_SYNCHRONIZATION_TIME + rnd(-1, 1),
                true, this, createExecutor(createFunctor(&MovableEntity::resynchronize)), false);

        this->registerVariables();
    }

    MovableEntity::~MovableEntity()
    {
        if (this->isInitialized())
            delete this->continuousResynchroTimer_;
    }

    void MovableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MovableEntity, XMLPort, xmlelement, mode);
    }

    void MovableEntity::registerVariables()
    {
        registerVariable(this->linearVelocity_,        variableDirection::toclient, new NetworkCallback<MovableEntity>(this, &MovableEntity::processLinearVelocity));
        registerVariable(this->angularVelocity_,       variableDirection::toclient, new NetworkCallback<MovableEntity>(this, &MovableEntity::processAngularVelocity));

        registerVariable(this->overwrite_position_,    variableDirection::toclient, new NetworkCallback<MovableEntity>(this, &MovableEntity::overwritePosition));
        registerVariable(this->overwrite_orientation_, variableDirection::toclient, new NetworkCallback<MovableEntity>(this, &MovableEntity::overwriteOrientation));
    }

    void MovableEntity::clientConnected(unsigned int clientID)
    {
        new Timer<MovableEntity>(rnd() * MAX_RESYNCHRONIZE_TIME, false, this, createExecutor(createFunctor(&MovableEntity::resynchronize)), true);
    }

    void MovableEntity::clientDisconnected(unsigned int clientID)
    {
    }

    void MovableEntity::resynchronize()
    {
        this->overwrite_position_ = this->getPosition();
        this->overwrite_orientation_ = this->getOrientation();
    }
}
