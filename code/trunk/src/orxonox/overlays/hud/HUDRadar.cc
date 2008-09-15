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
 *      Felix Schulthess
 *   Co-authors:
 *      Reto Grieder
 *
 */

#include "OrxonoxStableHeaders.h"
#include "HUDRadar.h"

#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>

#include "util/Math.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/SpaceShip.h"
#include "objects/WorldEntity.h"
#include "tools/TextureGenerator.h"
#include "Radar.h"

namespace orxonox
{
    CreateFactory(HUDRadar);

    HUDRadar::HUDRadar()
        : marker_(0)
    {
        RegisterObject(HUDRadar);
    }

    HUDRadar::~HUDRadar()
    {
        if (this->marker_)
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->marker_);
        for (std::vector<Ogre::PanelOverlayElement*>::iterator it = this->radarDots_.begin();
            it != this->radarDots_.end(); ++it)
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(*it);
        }
    }

    void HUDRadar::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(HUDRadar, XMLPort, xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            marker_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
                .createOverlayElement("Panel", "HUDRadar_marker_" + getUniqueNumberStr()));
            marker_->setMaterialName("Orxonox/RadarMarker");
            overlay_->add2D(marker_);
            marker_->hide();
        }

        XMLPortParam(HUDRadar, "sensitivity", setRadarSensitivity, getRadarSensitivity, xmlElement, mode)
            .defaultValues(1.0f);
        XMLPortParam(HUDRadar, "halfDotSizeDistance", setHalfDotSizeDistance, getHalfDotSizeDistance,
            xmlElement, mode).defaultValues(3000.0f);
        XMLPortParam(HUDRadar, "maximumDotSize", setMaximumDotSize, getMaximumDotSize, xmlElement, mode)
            .defaultValues(0.1f);

        shapeMaterials_[RadarViewable::Dot]      = "RadarSquare.tga";
        shapeMaterials_[RadarViewable::Triangle] = "RadarSquare.tga";
        shapeMaterials_[RadarViewable::Square]   = "RadarSquare.tga";
    }

    void HUDRadar::displayObject(RadarViewable* object, bool bIsMarked)
    {
        const WorldEntity* wePointer = object->getWorldEntity();

        // Just to be sure that we actually have a WorldEntity.
        // We could do a dynamic_cast, but that would be a lot slower.
        if (!wePointer)
        {
            CCOUT(4) << "Cannot display a non-WorldEntitiy on the radar" << std::endl;
            return;
        }

        // try to find a panel already created
        Ogre::PanelOverlayElement* panel;
        //std::map<RadarViewable*, Ogre::PanelOverlayElement*>::iterator it = this->radarDots_.find(object);
        if (itRadarDots_ == this->radarDots_.end())
        {
            // we have to create a new entry
            panel = static_cast<Ogre::PanelOverlayElement*>(
                Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "RadarDot" + getUniqueNumberStr()));
            radarDots_.push_back(panel);
            // get right material
            panel->setMaterialName(TextureGenerator::getMaterialName(
                shapeMaterials_[object->getRadarObjectType()], object->getRadarObjectColour()));
            this->overlay_->add2D(panel);
            this->itRadarDots_ = this->radarDots_.end();
        }
        else
        {
            panel = *itRadarDots_;
            ++itRadarDots_;
            std::string materialName = TextureGenerator::getMaterialName(
                shapeMaterials_[object->getRadarObjectType()], object->getRadarObjectColour());
            if (materialName != panel->getMaterialName())
                panel->setMaterialName(materialName);
        }
        panel->show();

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
            this->marker_->setDimensions(size * 1.5, size * 1.5);
            this->marker_->setPosition((1.0 + coord.x - size * 1.5) * 0.5, (1.0 - coord.y - size * 1.5) * 0.5);
        }
    }

    void HUDRadar::radarTick(float dt)
    {
        for (itRadarDots_ = radarDots_.begin(); itRadarDots_ != radarDots_.end(); ++itRadarDots_)
            (*itRadarDots_)->hide();
        this->itRadarDots_ = this->radarDots_.begin();
        this->marker_->hide();
    }
}
