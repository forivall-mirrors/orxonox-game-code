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
 *      Yuning Chai
 *   Co-authors:
 *      Felix Schulthess
 *
 */

#include "OrxonoxStableHeaders.h"
#include "HUDRadar.h"

#include <assert.h>
#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>

#include "core/ConsoleCommand.h"
#include "objects/SpaceShip.h"
#include "objects/WorldEntity.h"
#include "RadarViewable.h"

namespace orxonox
{
    CreateFactory(HUDRadar);
    CreateFactory(RadarColour);
    CreateFactory(RadarShape);

    HUDRadar* HUDRadar::instance_s = 0;

    using namespace Ogre;

    HUDRadar::HUDRadar()
      : background_(0)
    {
        RegisterObject(HUDRadar);

        assert(instance_s == 0);
        instance_s = this;
    }

    HUDRadar::~HUDRadar()
    {
        if (this->isInitialized())
        {
            if (this->background_)
                OverlayManager::getSingleton().destroyOverlayElement(this->background_);
            while (this->radarDots_.size() > 0)
            {
                OverlayManager::getSingleton().destroyOverlayElement(this->radarDots_[this->radarDots_.size() - 1]);
                this->radarDots_.pop_back();
            }
        }

        instance_s = 0;
    }

    void HUDRadar::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        XMLPortObject(HUDRadar, RadarColour, "shapes", addShape, getShape, xmlElement, mode, false, true);

        if (mode == XMLPort::LoadObject)
        {
            background_ = (Ogre::PanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_Background");
            background_->setMaterialName("Orxonox/Radar");
            overlay_->add2D(background_);

            // create an array of all possible materials
            unsigned int iMaterial = 0;
            for (std::map<unsigned int, RadarShape*>::const_iterator itShape = shapes_.begin(); itShape != shapes_.end(); ++itShape)
            {
                Ogre::MaterialPtr originalMaterial = (Ogre::MaterialPtr)(Ogre::MaterialManager::getSingleton().getByName((*itShape).second->getAttribute()));
                for (std::map<unsigned int, RadarColour*>::const_iterator itColour = colours_.begin(); itColour != colours_.end(); ++itColour)
                {
                    Ogre::MaterialPtr newMaterial = originalMaterial->clone((*itShape).second->getAttribute() + convertToString(iMaterial++));
                    newMaterial->getTechnique(0)->getPass(0)->setAmbient((*itColour).second->getAttribute()); 
                    materialNames_[(*itShape).first + ((*itColour).first << 8)] = newMaterial->getName();
                }
            }

            /*WorldEntity* object;
            object = new WorldEntity();
            object->setPosition(2000.0, 0.0, 0.0);
            addRadarObject(object, ColourValue(0.5, 0, 0, 1));
            object = new WorldEntity();
            object->setPosition(0.0, 2000.0, 0.0);
            addRadarObject(object, ColourValue(0.5, 0, 0, 1));
            object = new WorldEntity();
            object->setPosition(0.0, 0.0, 2000.0);
            addRadarObject(object, ColourValue(0.5, 0, 0, 1));
            object = new WorldEntity();
            object->setPosition(10000.0,16000.0,0.0);
            addRadarObject(object);*/
        }
    }

    void HUDRadar::addColour(RadarColour* colour)
    {
        this->colours_[colour->getIndex()] = colour;
    }

    RadarColour* HUDRadar::getColour(unsigned int index) const
    {
        if (index < this->colours_.size())
        {
            std::map<unsigned int, RadarColour*>::const_iterator it = colours_.begin();
            for (unsigned int i = 0; i != index; ++it, ++i)
                ;
            return (*it).second;
        }
        else
            return 0;
    }

    void HUDRadar::addShape(RadarShape* shape)
    {
        this->shapes_[shape->getIndex()] = shape;
    }

    RadarShape* HUDRadar::getShape(unsigned int index) const
    {
        if (index < this->shapes_.size())
        {
            std::map<unsigned int, RadarShape*>::const_iterator it = shapes_.begin();
            for (unsigned int i = 0; i != index; ++it, ++i)
                ;
            return (*it).second;
        }
        else
            return 0;
    }

    void HUDRadar::tick(float dt)
    {
        // iterate through all RadarObjects
        unsigned int i = 0;
        for (Iterator<RadarViewable> it = ObjectList<RadarViewable>::start(); it; ++it, ++i)
        {
            if ((*it)->isVisibleOnRadar())
            {
                WorldEntity* object = (*it)->getWorldEntity();
                // Just to be sure that we actually have a WorldEntity
                // We could do a dynamic_cast, but that's a lot slower
                assert(object);

                // set size to fit distance...
                float size = 1.0/((object->getWorldPosition() - SpaceShip::getLocalShip()->getPosition()).length());
                size = clamp(size * 100.0f, 0.02f, 0.12f);
                if (i == radarDots_.size())
                {
                    // we have to create a new panel
                    radarDots_.push_back(static_cast<Ogre::PanelOverlayElement*>(
                        Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "RadarDot" + convertToString(i))));
                }
                radarDots_[i]->setDimensions(size, size);

                // calc position on radar...
                Vector2 coord = get2DViewcoordinates(SpaceShip::getLocalShip()->getPosition(), SpaceShip::getLocalShip()->getDir(), SpaceShip::getLocalShip()->getOrth(), object->getWorldPosition());
                coord *= Ogre::Math::PI / 3.5; // small adjustment to make it fit the texture
                radarDots_[i]->setPosition((1.0 + coord.x) * 0.5, (1.0 - coord.y) * 0.5);

                // apply the right material
                RadarPoint description = (*it)->getDescription();
                radarDots_[i]->setMaterialName(materialNames_[description.shape_ + (description.colour_ << 8)]);
            }
        }
    }

    void HUDRadar::listObjects()
    {
        COUT(3) << "List of RadarObjects:\n";
        // iterate through all Radar Objects
        unsigned int i = 0;
        for (Iterator<RadarViewable> it = ObjectList<RadarViewable>::start(); it; ++it, ++i)
        {
            COUT(3) << i++ << ": " << (*it)->getWorldEntity()->getWorldPosition() << std::endl;
        }
    }

    /*static*/ HUDRadar& HUDRadar::getInstance()
    {
        assert(instance_s);
        return *instance_s;
    }
}
