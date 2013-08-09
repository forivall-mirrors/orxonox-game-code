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

#include <OgreVector3.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "graphics/Billboard.h"

namespace orxonox
{
    CreateFactory(CreateStars);

    CreateStars::CreateStars(Context* context) : BaseObject(context)
    {
        RegisterObject(CreateStars);
        this->material_ = "Examples/Flare";
        this->alpha_ = 0.7f;
        this->alphaDiff_ = 0.5f;
        this->radiusDiff_ = 0.9f;
        this->colour_.r = 1;
        this->colour_.g = 1;
        this->colour_.b = 1;
        this->colourDiff_ = 0.1f;
    }

    CreateStars::~CreateStars()
    {
        while( billboards_.size()!=0 )
        {
            billboards_.back()->destroy();
            billboards_.pop_back();

        }

        billboards_.clear();
    }

    void CreateStars::createBillboards()
    {

        for(int i=0; i < numStars_; i++)
        {
            Billboard* bb = new Billboard(this->getContext());

            float r = rnd(-colourDiff_,colourDiff_);
            float g = rnd(-colourDiff_,colourDiff_);
            float b = rnd(-colourDiff_,colourDiff_);
            orxonox::ColourValue thisColour = colour_;
            float alpha = alpha_+rnd(-alphaDiff_,alphaDiff_);
            thisColour.r=clamp((thisColour.r+r)*alpha, 0.0f, 1.0f);
            thisColour.g=clamp((thisColour.g+g)*alpha, 0.0f, 1.0f);
            thisColour.b=clamp((thisColour.b+b)*alpha, 0.0f, 1.0f);

            bb->setMaterial(material_);
            bb->setColour(thisColour);

            float phi;
            float teta;

            while(1)
            {
                phi = rnd(math::twoPi);
                teta = rnd(math::pi);
                float random = rnd(1);
                if(sin(teta)>random) break;
            }
            float radius = rnd(radiusDiff_,1)*radius_;
            bb->setPosition( PolarToCartesian(phi, teta, radius) );
            billboards_.push_back(bb);
        }
    }

    Vector3 CreateStars::PolarToCartesian(float phi, float teta, float radius)
    {
        float x = radius * cos(phi) * sin(teta);
        float y = radius * sin(phi) * sin(teta);
        float z = radius * cos(teta);
        return Vector3(x,y,z);
    }

    void CreateStars::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CreateStars, XMLPort, xmlelement, mode);

        XMLPortParam(CreateStars, "numStars", setNumStars, getNumStars, xmlelement, mode);
        XMLPortParam(CreateStars, "material", setMaterial, getMaterial, xmlelement, mode);
        XMLPortParam(CreateStars, "colour", setColour, getColour, xmlelement, mode);
        XMLPortParam(CreateStars, "alpha", setAlpha, getAlpha, xmlelement, mode);
        XMLPortParam(CreateStars, "colourDiff", setColourDiff, getColourDiff, xmlelement, mode);
        XMLPortParam(CreateStars, "alphaDiff", setAlphaDiff, getAlphaDiff, xmlelement, mode);
        XMLPortParam(CreateStars, "radiusDiff", setRadiusDiff, getRadiusDiff, xmlelement, mode);
        XMLPortParam(CreateStars, "radius", setRadius, getRadius, xmlelement, mode);
    }

}
