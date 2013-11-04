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

#include "HUDRadar.h"

#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>

#include "util/Math.h"
#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/TextureGenerator.h"
#include "worldentities/ControllableEntity.h"
#include "Scene.h"
#include "Radar.h"

namespace orxonox
{
    RegisterClass(HUDRadar);

    HUDRadar::HUDRadar(Context* context)
        : OrxonoxOverlay(context)
    {
        RegisterObject(HUDRadar);

        this->marker_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
            .createOverlayElement("Panel", "HUDRadar_marker_" + getUniqueNumberString()));
        this->marker_->setMaterialName("Orxonox/RadarMarker");
        this->overlay_->add2D(this->marker_);
        this->marker_->hide();

        this->setRadarSensitivity(1.0f);
        this->setHalfDotSizeDistance(3000.0f);
        this->setMaximumDotSize(0.1f);

        this->shapeMaterials_[RadarViewable::Dot]      = "RadarDot.png";
        this->shapeMaterials_[RadarViewable::Triangle] = "RadarTriangle.png";
        this->shapeMaterials_[RadarViewable::Square]   = "RadarSquare.png";
        this->setDetectionLimit( 10000.0f );
        this->owner_ = 0;
    }

    HUDRadar::~HUDRadar()
    {
        if (this->isInitialized())
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->marker_);
            for (std::map<RadarViewable*,Ogre::PanelOverlayElement*>::iterator it = this->radarObjects_.begin();
                it != this->radarObjects_.end(); ++it)
            {
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second);
            }
        }
    }

    void HUDRadar::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDRadar, XMLPort, xmlelement, mode);

        XMLPortParam(HUDRadar, "sensitivity", setRadarSensitivity, getRadarSensitivity, xmlelement, mode);
        XMLPortParam(HUDRadar, "halfDotSizeDistance", setHalfDotSizeDistance, getHalfDotSizeDistance, xmlelement, mode);
        XMLPortParam(HUDRadar, "maximumDotSize", setMaximumDotSize, getMaximumDotSize, xmlelement, mode);
    }

    void HUDRadar::addObject(RadarViewable* object)
    {
        if (object == orxonox_cast<RadarViewable*>(this->owner_))
            return;
        if( showObject(object) == false ) //do not show objects that are "invisible" or "radar invisible"
            return;

        // Make sure the object hasn't been added yet
        assert( this->radarObjects_.find(object) == this->radarObjects_.end() );

        // Create everything needed to display the object on the radar and add it to the map
        Ogre::PanelOverlayElement* panel;
        panel = static_cast<Ogre::PanelOverlayElement*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "RadarDot" + getUniqueNumberString()));
        this->overlay_->add2D(panel);
        // get right material
        panel->setMaterialName(TextureGenerator::getMaterialName(
            shapeMaterials_[object->getRadarObjectShape()], object->getRadarObjectColour()));
        panel->hide();
        this->radarObjects_[object] = panel;
    }

    void HUDRadar::removeObject(RadarViewable* object)
    {
        // If object was added at all then remove it
        std::map<RadarViewable*,Ogre::PanelOverlayElement*>::iterator it;
        it = this->radarObjects_.find( object );
        if( it != this->radarObjects_.end() )
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(it->second);
            this->radarObjects_.erase(it);
        }
    }

    void HUDRadar::objectChanged( RadarViewable* rv )
    {// The new implementation behaves more precisely, since inactive RadarViewables are not displayed anymore.
        this->removeObject(rv);
        this->addObject(rv);
    }

    void HUDRadar::gatherObjects()
    {
        const std::set<RadarViewable*>& objectSet = this->getCreator()->getScene()->getRadar()->getRadarObjects();
        std::set<RadarViewable*>::const_iterator it;
        for( it=objectSet.begin(); it!=objectSet.end(); ++it )
            this->addObject(*it);
        this->radarTick(0);
    }

    void HUDRadar::radarTick(float dt)
    {
        // Make sure the owner of the radar was defined
        if( !this->owner_ )
            return;

        this->marker_->hide();      // in case that no object is in focus
        // get the focus object
        Radar* radar = this->getOwner()->getScene()->getRadar();
        const RadarViewable* focusObject = radar->getFocus();

        // update the distances for all objects
        std::map<RadarViewable*,Ogre::PanelOverlayElement*>::iterator it;
        for( it = this->radarObjects_.begin(); it != this->radarObjects_.end(); ++it )
        {
            // Make sure the object really is a WorldEntity
            const WorldEntity* wePointer = it->first->getWorldEntity();
            if( !wePointer )
            {
                orxout(internal_error) << "Cannot display a non-WorldEntitiy on the radar" << endl;
                assert(0);
            }
            bool isFocus = (it->first == focusObject);
            // set size to fit distance...
            float distance = (wePointer->getWorldPosition() - this->owner_->getPosition()).length();
            // calculate the size with 1/distance dependency for simplicity (instead of exp(-distance * lambda)
            float size = maximumDotSize_ * halfDotSizeDistance_ / (halfDotSizeDistance_ + distance) * it->first->getRadarObjectScale();
            it->second->setDimensions(size, size);

            // calc position on radar...
            //Vector2 coord = get2DViewcoordinates(this->owner_->getPosition(), this->owner_->getOrientation() * WorldEntity::FRONT, this->owner_->getOrientation() * WorldEntity::UP, wePointer->getWorldPosition());
            Vector2 coord = get3DProjection(this->owner_->getPosition(), this->owner_->getOrientation() * WorldEntity::FRONT, this->owner_->getOrientation() * WorldEntity::UP, wePointer->getWorldPosition(), 0.6435011, detectionLimit_);
            coord *= math::pi / 3.5f; // small adjustment to make it fit the texture
            it->second->setPosition((1.0f + coord.x - size) * 0.5f, (1.0f - coord.y - size) * 0.5f);
            if( distance < detectionLimit_ || detectionLimit_ < 0 )
                it->second->show();
            else
                it->second->hide();

            // if this object is in focus, then set the focus marker
            if (isFocus)
            {
                this->marker_->setDimensions(size * 1.5f, size * 1.5f);
                this->marker_->setPosition((1.0f + coord.x - size * 1.5f) * 0.5f, (1.0f - coord.y - size * 1.5f) * 0.5f);
                this->marker_->show();
            }
        }
    }

    bool HUDRadar::showObject(RadarViewable* rv)
    {
        if ( rv == orxonox_cast<RadarViewable*> ( this->getOwner() ) )
            return false;
        assert( rv->getWorldEntity() );
        if ( rv->getWorldEntity()->isVisible()==false || rv->getRadarVisibility()==false )
            return false;
        return true;
    }


    void HUDRadar::changedOwner()
    {
        SUPER(HUDRadar, changedOwner);

        this->owner_ = orxonox_cast<ControllableEntity*>(this->getOwner());
        assert(this->radarObjects_.size() == 0);
        this->gatherObjects();
    }
}
