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

#include "core/ConsoleCommand.h"
#include "util/Math.h"
#include "objects/SpaceShip.h"
#include "objects/WorldEntity.h"
#include "RadarObject.h"

namespace orxonox
{
    CreateFactory(HUDRadar);

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
        }

        instance_s = 0;
    }

    void HUDRadar::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            background_ = (Ogre::PanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_Background");
            background_->setMaterialName("Orxonox/Radar");
            overlay_->add2D(background_);

            WorldEntity* object;
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
            addRadarObject(object);
        }

    }

    void HUDRadar::tick(float dt)
    {
        // iterate through all RadarObjects
        for(std::list<RadarObject*>::iterator it = getRadarObjects().begin(); it!= getRadarObjects().end(); it++)
        {
            // calc position on radar...
            // set size to fit distance...
            float size = 1.0/(((*it)->getPosition() - SpaceShip::getLocalShip()->getPosition()).length());
            size = clamp(size * 100.0f, 0.02f, 0.12f);
            (*it)->getPanel()->setDimensions(size, size);

            Vector2 coord = get2DViewcoordinates(SpaceShip::getLocalShip()->getPosition(), SpaceShip::getLocalShip()->getDir(), SpaceShip::getLocalShip()->getOrth(), (*it)->getPosition());
            coord *= Ogre::Math::PI / 3.5; // small adjustment to make it fit the texture
            (*it)->getPanel()->setPosition((1.0 + coord.x) * 0.5, (1.0 - coord.y) * 0.5);
        }
    }

    void HUDRadar::listObjects()
    {
        int i = 0;
        COUT(3) << "List of RadarObjects:\n";
        // iterate through all Radar Objects
        for(std::list<RadarObject*>::const_iterator it = getRadarObjects().begin(); it != getRadarObjects().end(); ++it)
        {
            COUT(3) << i++ << ": " << (*it)->getPosition() << std::endl;
        }
    }

    void HUDRadar::addRadarObject(WorldEntity* object, const ColourValue& colour)
    {
        RadarObject* obj = new RadarObject(overlay_, object, colour);
        roSet_.insert(roSet_.end(), obj);
    }

    void HUDRadar::removeRadarObject(WorldEntity* object)
    {
        for(std::list<RadarObject*>::iterator it=roSet_.begin(); it!=roSet_.end(); ++it)
        {
            if ((*it)->getObject() == object)
            {
                /*if (this->nav_ && this->nav_->getFocus() == (*it))
                    this->nav_->releaseFocus();*/

                delete (*it);
                roSet_.erase(it);
                return;
            }
        }
    }

    /*static*/ HUDRadar& HUDRadar::getInstance()
    {
        assert(instance_s);
        return *instance_s;
    }
}
