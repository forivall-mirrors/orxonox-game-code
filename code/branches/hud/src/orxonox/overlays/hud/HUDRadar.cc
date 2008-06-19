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
#include "tools/TextureGenerator.h"
#include "RadarViewable.h"

namespace orxonox
{
    CreateFactory(HUDRadar);
    CreateFactory(RadarShape);

    using namespace Ogre;

    HUDRadar::HUDRadar()
      : background_(0)
      , marker_(0)
      , sensitivity_(1.0f)
    {
        RegisterObject(HUDRadar);
    }

    HUDRadar::~HUDRadar()
    {
        if (this->isInitialized())
        {
            /*if (this->background_)
                OverlayManager::getSingleton().destroyOverlayElement(this->background_);
            while (this->radarDots_.size() > 0)
            {
                OverlayManager::getSingleton().destroyOverlayElement(this->radarDots_[this->radarDots_.size() - 1]);
                this->radarDots_.pop_back();
            }*/
        }
    }

    void HUDRadar::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            this->sensitivity_ = 1.0f;
            this->halfDotSizeDistance_ = 3000.0f;
            this->maximumDotSize_ = 0.1;
        }

        XMLPortParam(HUDRadar, "sensitivity", setRadarSensitivity, getRadarSensitivity, xmlElement, mode);
        XMLPortParam(HUDRadar, "halfDotSizeDistance", setHalfDotSizeDistance, getHalfDotSizeDistance, xmlElement, mode);
        XMLPortParam(HUDRadar, "maximumDotSize", setMaximumDotSize, getMaximumDotSize, xmlElement, mode);

        shapeMaterials_[RadarViewable::Dot]      = "RadarSquare.tga";
        shapeMaterials_[RadarViewable::Triangle] = "RadarSquare.tga";
        shapeMaterials_[RadarViewable::Square]   = "RadarSquare.tga";

        if (mode == XMLPort::LoadObject)
        {
            background_ = (Ogre::PanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_Background");
            background_->setMaterialName("Orxonox/Radar");
            overlay_->add2D(background_);

            marker_ = (Ogre::PanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_Marker");
            marker_->setMaterialName("Orxonox/RadarMarker");
            overlay_->add2D(marker_);
            marker_->hide();
        }
    }

    /*void HUDRadar::addShape(RadarShape* shape)
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
    }*/

    void HUDRadar::displayObject(RadarViewable* object, bool bIsMarked)
    {
        const WorldEntity* wePointer = object->getWorldEntity();

        // Just to be sure that we actually have a WorldEntity
        // We could do a dynamic_cast, but that's a lot slower
        if (!wePointer)
        {
            CCOUT(4) << "Cannot display a non-WorldEntitiy on the radar" << std::endl;
            return;
        }

        /*static int counter = 0;
        if (++counter < 1120 && counter > 120)
        {
            // we have to create a new entry
            Ogre::PanelOverlayElement* panel = static_cast<Ogre::PanelOverlayElement*>(
                Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "RadarDot" + getUniqueNumberStr()));
            // get right material
            panel->setMaterialName("Orxonox/RadarSquare");
            panel->setDimensions(0.03, 0.03);
            panel->setPosition((1.0 + (rand() & 0xFF) / 256.0 - 0.001) * 0.5, (1.0 - (rand() & 0xFF) / 256.0 - 0.001) * 0.5);
            panel->show();
            this->overlay_->add2D(panel);
            this->overlay_->show();
        }*/

        // try to find a panel already created
        Ogre::PanelOverlayElement* panel;
        std::map<RadarViewable*, Ogre::PanelOverlayElement*>::iterator it = this->radarDots_.find(object);
        if (it == this->radarDots_.end())
        {
            // we have to create a new entry
            panel = static_cast<Ogre::PanelOverlayElement*>(
                Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "RadarDot" + getUniqueNumberStr()));
            radarDots_[object] = panel;
            // get right material
            panel->setMaterialName(TextureGenerator::getMaterialName(
                shapeMaterials_[object->getRadarObjectType()], object->getRadarObjectColour()));
            this->overlay_->add2D(panel);
        }
        else
            panel = (*it).second;

        // set size to fit distance...
        float distance = (wePointer->getWorldPosition() - SpaceShip::getLocalShip()->getPosition()).length();
        // calculate the size with 1/distance dependency for simplicity (instead of exp(-distance * lambda)
        float size = maximumDotSize_ * halfDotSizeDistance_ / (halfDotSizeDistance_ + distance);
        panel->setDimensions(size, size);

        // calc position on radar...
        Vector2 coord = get2DViewcoordinates(SpaceShip::getLocalShip()->getPosition(), SpaceShip::getLocalShip()->getDir(), SpaceShip::getLocalShip()->getOrth(), wePointer->getWorldPosition());
        coord *= Ogre::Math::PI / 3.5; // small adjustment to make it fit the texture
        panel->setPosition((1.0 + coord.x - size) * 0.5, (1.0 - coord.y - size) * 0.5);

        if (bIsMarked)
        {
            this->marker_->show();
            this->marker_->setDimensions(size * 1.2, size * 1.2);
            this->marker_->setPosition((1.0 + coord.x - size * 1.2) * 0.5, (1.0 - coord.y - size * 1.2) * 0.5);
        }
    }

    void HUDRadar::radarTick(float dt)
    {
    }

    /*void HUDRadar::tick(float dt)
    {
        // iterate through all RadarObjects
        unsigned int i = 0;
        for (Iterator<RadarViewable> it = ObjectList<RadarViewable>::start(); it; ++it, ++i)
        {
            if ((*it)->isVisibleOnRadar())
            {
            }
        }
    }*/

    /*void HUDRadar::listObjects()
    {
        COUT(3) << "List of RadarObjects:\n";
        // iterate through all Radar Objects
        unsigned int i = 0;
        for (Iterator<RadarViewable> it = ObjectList<RadarViewable>::start(); it; ++it, ++i)
        {
            COUT(3) << i++ << ": " << (*it)->getWorldEntity()->getWorldPosition() << std::endl;
        }
    }*/
}
