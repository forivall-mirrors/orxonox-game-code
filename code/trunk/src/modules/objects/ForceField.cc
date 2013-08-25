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
 *      Kevin Young
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
    RegisterClass(ForceField);

    /*static*/ const std::string ForceField::modeTube_s = "tube";
    /*static*/ const std::string ForceField::modeSphere_s = "sphere";
    /*static*/ const std::string ForceField::modeInvertedSphere_s = "invertedSphere";
    /*static*/ const std::string ForceField::modeNewtonianGravity_s = "newtonianGravity";
    /*static*/ const float ForceField::gravConstant_ = 6.673e-11;
    /*static*/ const float ForceField::attenFactor_ = 1;

    /**
    @brief
        Constructor. Registers the object and initializes some values.
    */
    ForceField::ForceField(Context* context) : StaticEntity(context)
    {
        RegisterObject(ForceField);

        //Standard Values
        this->setDirection(Vector3::ZERO);
        this->setVelocity(100);
        this->setDiameter(500);
        this->setMassDiameter(0);   //! We allow point-masses
        this->setLength(2000);
        this->mode_ = forceFieldMode::tube;
        
        this->registerVariables();
    }

    /**
    @brief
        Destructor.
    */
    ForceField::~ForceField()
    {
    }

    /**
    @brief
        Creates a ForceField object through XML.
    */
    void ForceField::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ForceField, XMLPort, xmlelement, mode);

        XMLPortParam(ForceField, "velocity", setVelocity, getVelocity, xmlelement, mode).defaultValues(100);
        XMLPortParam(ForceField, "diameter", setDiameter, getDiameter, xmlelement, mode).defaultValues(500);
        XMLPortParam(ForceField, "massDiameter", setMassDiameter, getMassDiameter, xmlelement, mode).defaultValues(0);
        XMLPortParam(ForceField, "length", setLength  , getLength  , xmlelement, mode).defaultValues(2000);
        XMLPortParam(ForceField, "mode", setMode, getMode, xmlelement, mode);
    }
    
    void ForceField::registerVariables()
    {
        registerVariable(this->velocity_, VariableDirection::ToClient);
        registerVariable(this->radius_, VariableDirection::ToClient);
        registerVariable(this->massRadius_, VariableDirection::ToClient);
        registerVariable(this->halfLength_, VariableDirection::ToClient);
        registerVariable(this->mode_, VariableDirection::ToClient);
    }


    /**
    @brief
        A method that is called every tick.
        Implements the behavior of the ForceField.
    @param dt
        The amount of time that elapsed since the last tick.
    */
    void ForceField::tick(float dt)
    {
        if(this->mode_ == forceFieldMode::tube)
        {
            // Iterate over all objects that could possibly be affected by the ForceField.
            for (ObjectList<MobileEntity>::iterator it = ObjectList<MobileEntity>::begin(); it != ObjectList<MobileEntity>::end(); ++it)
            {
                // The direction of the orientation of the force field.
                Vector3 direction = this->getOrientation() * WorldEntity::FRONT;
                direction.normalise();

                // Vector from the center of the force field to the object its acting on.
                Vector3 distanceVector = it->getWorldPosition() - (this->getWorldPosition() + (this->halfLength_ * direction));

                // The object is outside a ball around the center with radius length/2 of the ForceField.
                if(distanceVector.length() > this->halfLength_)
                    continue;

                // The distance of the object form the orientation vector. (Or rather the smallest distance from the orientation vector)
                float distanceFromDirectionVector = ((it->getWorldPosition() - this->getWorldPosition()).crossProduct(direction)).length();

                // If the object in a tube of radius 'radius' around the direction of orientation.
                if(distanceFromDirectionVector >= this->radius_)
                    continue;

                // Apply a force to the object in the direction of the orientation.
                // The force is highest when the object is directly on the direction vector, with a linear decrease, finally reaching zero, when distanceFromDirectionVector = radius.
                it->applyCentralForce((this->radius_ - distanceFromDirectionVector)/this->radius_ * this->velocity_ * direction);
            }
        }
        else if(this->mode_ == forceFieldMode::sphere)
        {
            // Iterate over all objects that could possibly be affected by the ForceField.
            for (ObjectList<MobileEntity>::iterator it = ObjectList<MobileEntity>::begin(); it != ObjectList<MobileEntity>::end(); ++it)
            {
                Vector3 distanceVector = it->getWorldPosition() - this->getWorldPosition();
                float distance = distanceVector.length();
                // If the object is within 'radius' distance.
                if (distance < this->radius_)
                {
                    distanceVector.normalise();
                    // Apply a force proportional to the velocity, with highest force at the origin of the sphere, linear decreasing until reaching a distance of 'radius' from the origin, where the force reaches zero. 
                    it->applyCentralForce((this->radius_ - distance)/this->radius_ * this->velocity_ * distanceVector);
                }
            }
        }
        else if(this->mode_ == forceFieldMode::invertedSphere)
        {
            // Iterate over all objects that could possibly be affected by the ForceField.
            for (ObjectList<MobileEntity>::iterator it = ObjectList<MobileEntity>::begin(); it != ObjectList<MobileEntity>::end(); ++it)
            {
                Vector3 distanceVector = this->getWorldPosition() - it->getWorldPosition();
                float distance = distanceVector.length();
                // If the object is within 'radius' distance and no more than 'length' away from the boundary of the sphere.
                float range = this->radius_ - this->halfLength_*2;
                if (distance < this->radius_ && distance > range)
                {
                    distanceVector.normalise();
                    // Apply a force proportional to the velocity, with highest force at the boundary of the sphere, linear decreasing until reaching a distance of 'radius-length' from the origin, where the force reaches zero.
                    it->applyCentralForce((distance-range)/range * this->velocity_ * distanceVector);
                }
            }
        }
        else if(this->mode_ == forceFieldMode::newtonianGravity)
        {
            // Iterate over all objects that could possibly be affected by the ForceField.
            for (ObjectList<MobileEntity>::iterator it = ObjectList<MobileEntity>::begin(); it != ObjectList<MobileEntity>::end(); ++it)
            {
                Vector3 distanceVector = it->getWorldPosition() - this->getWorldPosition();
                float distance = distanceVector.length();
                // If the object is within 'radius' distance and especially further away than massRadius_
                if (distance < this->radius_ && distance > this->massRadius_)
                {
                    distanceVector.normalise();
                    /* Apply a central force that follows the newtownian law of gravity, ie.:
                     * F = G * (M*m) / D^2,
                     * while M is the mass of the stellar body and m is the mass of the affected object. 
                     * D is the distance from the center of mass of both bodies
                     * and it should be noted that massRadius_ denotes the radius of the stellar body,
                     * at which point the force vanishes (you can use this to dictate the size of the body).
                     * attenFactor_ weakens the field by a constant factor. The -1 is needed for an attractive force.
                     */
                    
                    // Note: this so called force is actually an acceleration!
                    it->applyCentralForce((-1) * (ForceField::attenFactor_ * ForceField::gravConstant_ * this->getMass()) / (distance * distance) * distanceVector);
                }
            }
        }
    }

    /**
    @brief
        Set the mode of the ForceField.
    @param mode
        The mode as a string.
    */
    void ForceField::setMode(const std::string& mode)
    {
        if(mode == ForceField::modeTube_s)
            this->mode_ = forceFieldMode::tube;
        else if(mode == ForceField::modeSphere_s)
            this->mode_ = forceFieldMode::sphere;
        else if(mode == ForceField::modeInvertedSphere_s)
            this->mode_ = forceFieldMode::invertedSphere;
        else if(mode == ForceField::modeNewtonianGravity_s)
            this->mode_ = forceFieldMode::newtonianGravity;
        else
        {
            orxout(internal_warning) << "Wrong mode '" << mode << "' in ForceField. Setting to 'tube'." << endl;
            this->mode_ = forceFieldMode::tube;
        }
    }

    /**
    @brief
        Get the mode of the ForceField.
    @return
        Returns the mode of the ForceField as a string.
    */
    const std::string& ForceField::getMode(void)
    {
        switch(this->mode_)
        {
            case forceFieldMode::tube:
                return ForceField::modeTube_s;
            case forceFieldMode::sphere:
                return ForceField::modeSphere_s;
            case forceFieldMode::invertedSphere:
                return ForceField::modeInvertedSphere_s;
            case forceFieldMode::newtonianGravity:
                return ForceField::modeNewtonianGravity_s;
            default:
                return ForceField::modeTube_s;
        }
    }

}
