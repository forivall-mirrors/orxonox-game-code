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

#include "OrxonoxStableHeaders.h"
#include "LinearEntity.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Executor.h"
#include "tools/Timer.h"

namespace orxonox
{
    static const float MAX_RESYNCHRONIZE_TIME = 3.0f;

    CreateFactory(LinearEntity);

    LinearEntity::LinearEntity(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(LinearEntity);

        this->overwrite_position_    = Vector3::ZERO;
        this->overwrite_orientation_ = Quaternion::IDENTITY;

        this->registerVariables();
    }

    LinearEntity::~LinearEntity()
    {
    }

    void LinearEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LinearEntity, XMLPort, xmlelement, mode);
    }

    void LinearEntity::registerVariables()
    {
        REGISTERDATA(this->linearVelocity_,        network::direction::toclient, new network::NetworkCallback<LinearEntity>(this, &LinearEntity::processLinearVelocity));
        REGISTERDATA(this->angularVelocity_,       network::direction::toclient, new network::NetworkCallback<LinearEntity>(this, &LinearEntity::processAngularVelocity));

        REGISTERDATA(this->overwrite_position_,    network::direction::toclient, new network::NetworkCallback<LinearEntity>(this, &LinearEntity::overwritePosition));
        REGISTERDATA(this->overwrite_orientation_, network::direction::toclient, new network::NetworkCallback<LinearEntity>(this, &LinearEntity::overwriteOrientation));
    }

    void LinearEntity::tick(float dt)
    {
        MovableEntity::tick(dt);

        if (this->isActive())
        {
        }
    }

    void LinearEntity::overwritePosition()
    {
        this->setPosition(this->overwrite_position_);
    }

    void LinearEntity::overwriteOrientation()
    {
        this->setOrientation(this->overwrite_orientation_);
    }

    void LinearEntity::clientConnected(unsigned int clientID)
    {
        resynchronize();
        new Timer<LinearEntity>(rnd() * MAX_RESYNCHRONIZE_TIME, true, this, createExecutor(createFunctor(&LinearEntity::resynchronize)), false);
    }

    void LinearEntity::clientDisconnected(unsigned int clientID)
    {
    }

    void LinearEntity::resynchronize()
    {
        positionChanged(false);
        orientationChanged(false);
    }

    void LinearEntity::positionChanged(bool bContinuous)
    {
        if (!bContinuous)
            this->overwrite_position_ = this->getPosition();
    }

    void LinearEntity::orientationChanged(bool bContinuous)
    {
        if (!bContinuous)
            this->overwrite_orientation_ = this->getOrientation();
    }
}
