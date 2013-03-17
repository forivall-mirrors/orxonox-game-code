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
 *      Marian Runo
 *   Co-authors:
 *      ...
 *
 */

#include "Turret.h"
#include "core/CoreIncludes.h"
#include "OgreQuaternion.h"
#include "core/XMLPort.h"
#include "controllers/WaypointPatrolController.h"

namespace orxonox
{
    CreateFactory(Turret);

    /**
     * @brief Constructor
     */
    Turret::Turret(BaseObject* creator) : SpaceShip(creator)
    {
        RegisterObject(Turret);
        this->controller_ = new WaypointPatrolController(this);
    }

    /**
     * @brief Destructor
     */
    Turret::~Turret()
    {

    }


    void Turret::rotatePitch(const Vector2& value)
    {
    	orxout()<< "Turret rotate Pitch"<< endl;

    	const Quaternion& orient = this->getOrientation();
    	Radian pitch = orient.getPitch();

    	if((value.x > 0 && pitch < Radian(180)) || (value.x < 0 && pitch > Radian(0)))
    		SpaceShip::rotatePitch(value);
    }


    void Turret::setAlertnessRadius(float value)
    {
    	this->controller_->setAlertnessRadius(value);
    }
    float Turret::getAlertnessRadius()
    {
    	return this->controller_->getAlertnessRadius();
    }

    void Turret::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Turret, XMLPort, xmlelement, mode);
        XMLPortParam(Turret, "alertnessRadius", setAlertnessRadius, getAlertnessRadius, xmlelement, mode).defaultValues("400");
    }


}
