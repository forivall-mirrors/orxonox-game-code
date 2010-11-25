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

/**
@file ForceField.cc
@brief Implementation of the ForceField class.
*/

#include "ForceField.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/MobileEntity.h"

namespace orxonox
{
    CreateFactory(ForceField);
    
    /*static*/ const std::string modeStringNormal_s = "tube";
    /*static*/ const std::string modeStringSphere_s = "sphere";

    ForceField::ForceField(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(ForceField);

        //Standard Values
        this->setDirection(Vector3::ZERO);
        this->velocity_ = 100;
        this->diameter_ = 500;
        this->length_ = 5000;
        this->mode_ = ForceFieldMode::tube;
    }

    ForceField::~ForceField()
    {
    }

    void ForceField::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ForceField, XMLPort, xmlelement, mode);

        //For correct xml import use: position, direction, velocity, scale
        XMLPortParam(ForceField, "velocity", setVelocity, getVelocity, xmlelement, mode).defaultValues(100);
        XMLPortParam(ForceField, "diameter", setDiameter, getDiameter, xmlelement, mode).defaultValues(500);
        XMLPortParam(ForceField, "length"  , setLength  , getLength  , xmlelement, mode).defaultValues(2000);
        XMLPortParam(ForceField, "mode", setMode, getMode, xmlelement, mode);
    }

    void ForceField::tick(float dt)
    {
        // Iterate over all objects that could possibly be affected by the ForceField.
        for (ObjectList<MobileEntity>::iterator it = ObjectList<MobileEntity>::begin(); it != ObjectList<MobileEntity>::end(); ++it)
        {
            if(this->mode_ == ForceFieldMode::tube)
            {
                // The direction of the orientation of the force field.
                Vector3 direction = this->getOrientation() * WorldEntity::FRONT;
                direction.normalise();

                // Vector from the center of the force field to the object its acting on.
                // TODO: This could probably be simplified.
                Vector3 distanceVector = it->getWorldPosition() - (this->getWorldPosition() + (this->halfLength_ * direction));
                
                // The object is outside of the length of the ForceField.
                if(distanceVector.length() > this->halfLength_)
                    return;

                // The distance of the object form the orientation vector. (Or rather the smallest distance from the orientation vector)
                float distanceFromDirectionVector = ((it->getWorldPosition() - this->getWorldPosition()).crossProduct(direction)).length();
                
                // If the object in a tube of radius diameter/2 around the direction of orientation.
                if(distanceFromDirectionVector >= this->radius_)
                    return;

                // Apply a force to the object in the direction of the orientation.
                // The force is highest when the object is directly on the direction vector, with a linear decrease, finally reaching zero, when distanceFromDirectionVector = radius.
                it->applyCentralForce(((this->radius_ - distanceFromDirectionVector)/(this->radius_)) * this->velocity_ * direction);
            }
            else if(this->mode_ == ForceFieldMode::sphere)
            {
                Vector3 distanceVector = it->getWorldPosition() - this->getWorldPosition();
                float distance = distanceVector.length();
                if (distance < this->radius_)
                {
                    distanceVector.normalise();
                    it->applyCentralForce((this->radius_ - distance)/this->radius_ * this->velocity_ * distanceVector);
                }
            }
        }
    }
    
    void ForceField::setMode(const std::string& mode)
    {
        if(mode == ForceField::modeStringTube_s)
            this->mode_ = ForceFieldMode::tube;
        else if(mode == ForceField::modeStringSphere_s)
            this->mode_ = ForceFieldMode::sphere;
        else
        {
            COUT(2) << "Wrong mode '" << mode << "' in ForceField. Setting to 'tube'." << std::endl;
            this->mode_ = ForceFieldMode::tube;
        }
    }
    
    inline const std::string& ForceField::getMode(void)
    {
        switch(this->mode_)
        {
            case ForceFieldMode::tube:
                return ForceField::modeStringTube_s;
            case ForceFieldMode::sphere:
                return ForceField::modeStringSphere_s;
            default:
                return ForceField::modeStringTube_s;
        }
    }
}





