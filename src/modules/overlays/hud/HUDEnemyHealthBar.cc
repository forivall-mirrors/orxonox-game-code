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
 *      Matthias Spalinger
 *   Co-authors:
 *      ...
 *
 */

#include "HUDEnemyHealthBar.h"

#include <OgreCamera.h>

#include "util/Convert.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "infos/PlayerInfo.h"
#include "overlays/OverlayGroup.h"
#include "CameraManager.h"
#include "graphics/Camera.h"
#include "util/Math.h"
#include "HUDNavigation.h"
#include "core/input/InputManager.h"
#include "controllers/HumanController.h"
#include "core/GraphicsManager.h"
#include "Scene.h"
#include "Radar.h"
#include "controllers/NewHumanController.h"

namespace orxonox
{
    CreateFactory(HUDEnemyHealthBar);

    HUDEnemyHealthBar::HUDEnemyHealthBar(BaseObject* creator) : HUDHealthBar(creator)
    {
        RegisterObject(HUDEnemyHealthBar);

        this->setConfigValues();
        setSensibility(0.1f);
        this->owner_ = 0;
        markerLimit_ = 3; //TODO connect with markerLimit_ from the settings / from HUDNavigation.cc
        currentYaw = 0;
        currentPitch = 0;

        this->getOverlayText()->setCaption("");
    }

    HUDEnemyHealthBar::~HUDEnemyHealthBar()
    {
        sortedObjectList_.clear();
    }

    void HUDEnemyHealthBar::setConfigValues()
    {
        SetConfigValue(useEnemyBar_, true);
    }

    void HUDEnemyHealthBar::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDEnemyHealthBar, XMLPort, xmlelement, mode);

        XMLPortParam ( HUDEnemyHealthBar, "sensibility", setSensibility, getSensibility, xmlelement, mode );
    }

    bool compareDist ( std::pair<RadarViewable*, unsigned int > a, std::pair<RadarViewable*, unsigned int > b )
    {
        return a.second<b.second;
    }

    void HUDEnemyHealthBar::tick(float dt)
    {
        if (!useEnemyBar_){
            this->setValue(0); //TODO hide it instead of setting it to 0
            this->getOverlayText()->setCaption("");
            return;
        }

        Camera* cam = CameraManager::getInstance().getActiveCamera();
        if ( cam == NULL )
            return;
        const Matrix4& camTransform = cam->getOgreCamera()->getProjectionMatrix() * cam->getOgreCamera()->getViewMatrix();

        unsigned int markerCount_ = 0;

        for ( sortedList::iterator listIt = sortedObjectList_.begin(); listIt != sortedObjectList_.end(); ++listIt )
        {
            listIt->second = ( int ) ( ( listIt->first->getRVWorldPosition() - HumanController::getLocalControllerSingleton()->getControllableEntity()->getWorldPosition() ).length() + 0.5f );
        }
    
        sortedObjectList_.sort ( compareDist );

        for ( sortedList::iterator listIt = sortedObjectList_.begin(); listIt != sortedObjectList_.end(); ++markerCount_, ++listIt )
        {
            if ( markerCount_ < markerLimit_ )
            {        
                // Transform to screen coordinates and reverse x-axis
                Vector3 pos = (camTransform * listIt->first->getRVWorldPosition());
                pos.x = -pos.x;                

                // get mouse position
                if(this->getOwner() && dynamic_cast<ControllableEntity*>(this->getOwner())->getController() && dynamic_cast<NewHumanController*>(dynamic_cast<ControllableEntity*>(this->getOwner())->getController()))
                {
                    currentYaw = dynamic_cast<NewHumanController*>(dynamic_cast<ControllableEntity*>(this->getOwner())->getController())->getCurrentYaw();
                    currentPitch = dynamic_cast<NewHumanController*>(dynamic_cast<ControllableEntity*>(this->getOwner())->getController())->getCurrentPitch();
                }
                // Compare cursor position to object position
                if ( fabs(pos.x - currentYaw) < sens_ && fabs(pos.y - currentPitch) < sens_ )
                {
                    this->owner_ = orxonox_cast<Pawn*>(listIt->first);
                    break;
                }
            }
            this->owner_ = 0;
        }



        if (this->owner_)
        {
            this->setValue(this->owner_->getHealth() / this->owner_->getInitialHealth());
            this->getOverlayText()->setCaption(multi_cast<std::string>(static_cast<int>(this->owner_->getHealth())));
        }
        else
        {
            this->setValue(0);      //TODO hide it instead of setting it to zero
            this->getOverlayText()->setCaption("");
        }

        if (this->getTextUseBarColour())
            this->getOverlayText()->setColour(this->getCurrentBarColour());
    }

    void HUDEnemyHealthBar::addObject ( RadarViewable* object )
    {
        if( showObject(object)==false )
            return;
    
        if ( sortedObjectList_.size() >= markerLimit_ )
            if ( object == NULL )
                return;
    
        sortedObjectList_.push_front ( std::make_pair ( object, ( unsigned int ) 0 ) );

        //remove duplicates
        sortedObjectList_.unique();
    }

    bool HUDEnemyHealthBar::showObject(RadarViewable* rv)
    {
        if ( rv == dynamic_cast<RadarViewable*> ( this->getOwner() ) )
            return false;
        assert( rv->getWorldEntity() );
        if ( rv->getWorldEntity()->isVisible()==false || rv->getRadarVisibility()==false )
            return false;
        return true;
    }

    void HUDEnemyHealthBar::removeObject ( RadarViewable* viewable )
    {
        for ( sortedList::iterator listIt = sortedObjectList_.begin(); listIt != sortedObjectList_.end(); ++listIt )
        {
            if ( (listIt->first) == viewable )
            {
                sortedObjectList_.erase ( listIt );
                break;
            }
    
        }

    }

    void HUDEnemyHealthBar::objectChanged(RadarViewable* viewable)
    {
        // TODO: niceification neccessary - and while you're at it: the same function exists in HUDNavigation.cc ;)
        removeObject(viewable);
        addObject(viewable);
    }

    void HUDEnemyHealthBar::changedOwner()
    {
    
        const std::set<RadarViewable*>& respawnObjects = this->getOwner()->getScene()->getRadar()->getRadarObjects();
        for ( std::set<RadarViewable*>::const_iterator it = respawnObjects.begin(); it != respawnObjects.end(); ++it )
        {
            if ( ! ( *it )->isHumanShip_ )
                this->addObject ( *it );
        }
    }

}
