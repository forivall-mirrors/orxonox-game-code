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
    RegisterClass(Turret);



    /**
     * @brief Constructor
     */
    Turret::Turret(Context* context) : SpaceShip(context)
    {
        RegisterObject(Turret);
        this->controller_ = new WaypointPatrolController(this->getContext());
        gotOrient_ = false;
    }

    /**
     * @brief Destructor
     */
    Turret::~Turret()
    {

    }


    void Turret::rotatePitch(const Vector2& value)
    {
        Radian currentPitch = this->getOrientation().getPitch();
        Radian startPitch = startOrient_.getPitch();
        Radian limit = Radian((Degree)45);
        Radian upperBoundary = startPitch+limit;
        if(upperBoundary > Radian(Degree(360)))
        {
            upperBoundary -= Radian(Degree(360));
        }
        Radian lowerBoundary = startPitch-limit;
        if(lowerBoundary < Radian(Degree(0)))
        {
            lowerBoundary += Radian(Degree(360));
        }
        //orxout() << "Pitch:\t" << currentPitch << "\t" << startPitch << endl;
        
        if(currentPitch > upperBoundary && value.x > 0 ||
           currentPitch < lowerBoundary && value.x < 0)
        {
            return;
        }
        
        //SpaceShip::rotatePitch(value);
    }

    void Turret::rotateYaw(const Vector2& value)
    {

        Radian currentYaw = this->getOrientation().getYaw();
        Radian startYaw = startOrient_.getYaw();
        Radian limit = Radian(Degree(45));
        Radian upperBoundary = startYaw + limit;
        Radian lowerBoundary = startYaw - limit;
        orxout() << currentYaw << " " << this->getOrientation().getRoll() << endl;
        /*if(upperBoundary >= Radian(Degree(180)));
        {
            upperBoundary -= Radian(Degree(180));
            lowerBoundary -= Radian(Degree(180));
            currentYaw -= Radian(Degree(180));
        }
        if(lowerBoundary <= Radian(Degree(-180)))
        {
            lowerBoundary += Radian(Degree(180));
            upperBoundary += Radian(Degree(180));
            currentYaw += Radian(Degree(180));
        }*/
        //orxout() << "Yaw:\t" << (Degree)currentYaw << "\t" << (Degree)upperBoundary << "\t" << (Degree)lowerBoundary << endl;
        //if((currentYaw > upperBoundary && value.x > 0) ||
        //   (currentYaw < lowerBoundary && value.x < 0))
        if((currentYaw < Radian(1) && value.x < 0) || (currentYaw > Radian(3) && value.x>0))
        {
            //return;
        }
        SpaceShip::rotateYaw(value);
    }

    void Turret::rotateRoll(const Vector2& value)
    {
        return;
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

    void Turret::tick(float dt)
    {
        if(!gotOrient_)
        {
            startOrient_ = this->getOrientation();
            gotOrient_ = true;
        }
        SUPER(Turret, tick, dt);
    }

}
