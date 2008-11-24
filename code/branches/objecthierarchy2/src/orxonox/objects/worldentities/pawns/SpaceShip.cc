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
#include "SpaceShip.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "util/Math.h"
#include "objects/items/Engine.h"

namespace orxonox
{
    CreateFactory(SpaceShip);

    SpaceShip::SpaceShip(BaseObject* creator) : Pawn(creator)
    {
        RegisterObject(SpaceShip);

        this->zeroDegree_ = 0;

        this->bBoost_ = false;
        this->steering_ = Vector3::ZERO;
        this->engine_ = 0;

        this->maxRotation_ = 0;
        this->rotationAcceleration_ = 0;
        this->translationDamping_ = 0;

        this->yawRotation_ = 0;
        this->pitchRotation_ = 0;
        this->rollRotation_ = 0;

        this->bInvertYAxis_ = false;

        this->setDestroyWhenPlayerLeft(true);

        this->setConfigValues();
        this->registerVariables();
    }

    SpaceShip::~SpaceShip()
    {
        if (this->isInitialized() && this->engine_)
            delete this->engine_;
    }

    void SpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceShip, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceShip, "engine",            setEngineTemplate,    getEngineTemplate,    xmlelement, mode);
        XMLPortParam(SpaceShip, "maxrotation",       setMaxRotation,       getMaxRotation,       xmlelement, mode);
        XMLPortParam(SpaceShip, "rotacc",            setRotAcc,            getRotAcc,            xmlelement, mode);
        XMLPortParam(SpaceShip, "transdamp",         setTransDamp,         getTransDamp,         xmlelement, mode);
    }

    void SpaceShip::registerVariables()
    {
        REGISTERDATA(this->maxRotation_,             direction::toclient);
        REGISTERDATA(this->rotationAcceleration_,    direction::toclient);
        REGISTERDATA(this->translationDamping_,      direction::toclient);
    }

    void SpaceShip::setConfigValues()
    {
        SetConfigValue(bInvertYAxis_, false).description("Set this to true for joystick-like mouse behaviour (mouse up = ship down).");
    }

    void SpaceShip::tick(float dt)
    {
        if (this->hasLocalController())
        {
            // #####################################
            // ############# STEERING ##############
            // #####################################

            Vector3 velocity = this->getVelocity();

            // normalize velocity and acceleration
            for (size_t dimension = 0; dimension < 3; ++dimension)
            {
                if (this->acceleration_[dimension] == 0)
                {
                    if (velocity[dimension] > 0)
                    {
                        velocity[dimension] -= (this->translationDamping_ * dt);
                        if (velocity[dimension] < 0)
                            velocity[dimension] = 0;
                    }
                    else if (velocity[dimension] < 0)
                    {
                        velocity[dimension] += (this->translationDamping_ * dt);
                        if (velocity[dimension] > 0)
                            velocity[dimension] = 0;
                    }
                }
            }

            this->setVelocity(velocity);
        }


        SUPER(SpaceShip, tick, dt);


        if (this->hasLocalController())
        {
            this->yaw(this->yawRotation_ * dt);
            if (this->bInvertYAxis_)
                this->pitch(Degree(-this->pitchRotation_ * dt));
            else
                this->pitch(Degree( this->pitchRotation_ * dt));
            this->roll(this->rollRotation_ * dt);

            this->yawRotation_   = this->zeroDegree_;
            this->pitchRotation_ = this->zeroDegree_;
            this->rollRotation_  = this->zeroDegree_;
        }
    }

    void SpaceShip::moveFrontBack(const Vector2& value)
    {
        this->steering_.z = -value.x;
    }

    void SpaceShip::moveRightLeft(const Vector2& value)
    {
        this->steering_.x = value.x;
    }

    void SpaceShip::moveUpDown(const Vector2& value)
    {
        this->steering_.y = value.x;
    }

    void SpaceShip::rotateYaw(const Vector2& value)
    {
        Degree temp = value.x * value.x * sgn(value.x) * this->rotationAcceleration_;
        if (temp > this->maxRotation_)
            temp = this->maxRotation_;
        if (temp < -this->maxRotation_)
            temp = -this->maxRotation_;
        this->yawRotation_ = Degree(temp);
    }

    void SpaceShip::rotatePitch(const Vector2& value)
    {
        Degree temp = value.x * value.x * sgn(value.x) * this->rotationAcceleration_;
        if (temp > this->maxRotation_)
            temp = this->maxRotation_;
        if (temp < -this->maxRotation_)
            temp = -this->maxRotation_;
        this->pitchRotation_ = Degree(temp);
    }

    void SpaceShip::rotateRoll(const Vector2& value)
    {
        Degree temp = value.x * value.x * sgn(value.x) * this->rotationAcceleration_;
        if (temp > this->maxRotation_)
            temp = this->maxRotation_;
        if (temp < -this->maxRotation_)
            temp = -this->maxRotation_;
        this->rollRotation_ = Degree(temp);
    }

    void SpaceShip::fire()
    {
    }

    void SpaceShip::boost()
    {
        this->bBoost_ = true;
    }

    void SpaceShip::loadEngineTemplate()
    {
        if (this->enginetemplate_ != "")
        {
            Template* temp = Template::getTemplate(this->enginetemplate_);

            if (temp)
            {
                Identifier* identifier = temp->getBaseclassIdentifier();

                if (identifier)
                {
                    BaseObject* object = identifier->fabricate(this);
                    this->engine_ = dynamic_cast<Engine*>(object);

                    if (this->engine_)
                    {
                        this->engine_->addTemplate(temp);
                        this->engine_->addToSpaceShip(this);
                    }
                    else
                    {
                        delete object;
                    }
                }
            }
        }
    }
}
