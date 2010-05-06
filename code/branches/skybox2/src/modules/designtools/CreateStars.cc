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
 *      Gion-Andri Cantieni
 *   Co-authors:
 *      ...
 *
 */

#include "CreateStars.h"

#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ScopedSingletonManager.h"
#include "core/BaseObject.h"


#include "core/XMLPort.h"

#include "graphics/Billboard.h"

#include <OgreVector3.h>
#include <math.h>
 
namespace orxonox
{

    CreateFactory(CreateStars);

    CreateStars::CreateStars(BaseObject* creator) : BaseObject(creator)

    {
        RegisterObject(CreateStars);
    }

    CreateStars::~CreateStars()
    {
        while( billboards_.size()!=0 ) 
        {
            delete(billboards_.back());
            billboards_.pop_back();

        }

        billboards_.clear();
    }

    void CreateStars::createBillboards()
    {

        for(int i=0; i < numStars_; i++) 
        {
            Billboard* b = new Billboard(this);
            b->setMaterial(material_);

            float alpha = rnd(-90,90);
            float beta = rnd(0,360);
            
            b->setPosition( PolarToCartesian(alpha, beta, radius_) );
            billboards_.push_back(b);
        }
    }

    Vector3 PolarToCartesian(float alpha, float beta, float radius) 
    {
        int x = radius * cos(alpha) * sin(beta);
        int y = radius * sin(alpha) * cos(beta);
        int z = radius * cos(beta);
        return Vector3(x,y,z);
    }

    void CreateStars::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(CreateStars, "numStars", setNumStars, getNumStars, xmlelement, mode);
        XMLPortParam(CreateStars, "material", setMaterial, getMaterial, xmlelement, mode);
        XMLPortParam(CreateStars, "radius", setRadius, getRadius, xmlelement, mode);
	}

}
