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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */

#include "ForceField.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/MobileEntity.h"

namespace orxonox
{
    CreateFactory(ForceField);

    ForceField::ForceField(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(ForceField);

        //Standard Values
        this->setDirection(Vector3::ZERO);
        velocity_ = 100;
        diameter_ = 500;
        length_ = 5000;
    }

    ForceField::~ForceField() {}

    void ForceField::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ForceField, XMLPort, xmlelement, mode);

        //For correct xml import use: position, direction, velocity, scale
        XMLPortParam(ForceField, "velocity", setVelocity, getVelocity, xmlelement, mode).defaultValues(100);
        XMLPortParam(ForceField, "diameter", setDiameter, getDiameter, xmlelement, mode).defaultValues(500);
        XMLPortParam(ForceField, "length"  , setLength  , getLength  , xmlelement, mode).defaultValues(2000);
    }

    void ForceField::tick(float dt)
    {
        for (ObjectList<MobileEntity>::iterator it = ObjectList<MobileEntity>::begin(); it != ObjectList<MobileEntity>::end(); ++it)
        {
            //calculate from
            Vector3 directionVec = this->getOrientation() * WorldEntity::FRONT;
            directionVec.normalise();

            Vector3 distanceVec = it->getWorldPosition() - (this->getWorldPosition() + (this->length_ / 2 * directionVec));

            //distance from centervector of the field (
            float distFromCenterVec = ((it->getWorldPosition() - this->getWorldPosition()).crossProduct(directionVec)).length();

            if (distanceVec.length() < this->length_ / 2 && distFromCenterVec < diameter_ / 2)
            {
                //normalize distance from center
                it->applyCentralForce(((diameter_ / 2 - distFromCenterVec) / (diameter_ / 2)) * directionVec * velocity_);
            }
        }
    }
}





