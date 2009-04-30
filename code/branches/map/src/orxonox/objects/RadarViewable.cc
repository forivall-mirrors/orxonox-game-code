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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "RadarViewable.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "objects/worldentities/WorldEntity.h"
#include "objects/Radar.h"
#include "util/String.h"
#include <OgreManualObject.h>
#include "overlays/map/Map.h"
#include "orxonox/tools/DynamicLines.h"

namespace orxonox
{
    /**
        @brief Constructor.
    */
    RadarViewable::RadarViewable()
        : radarObjectCamouflage_(0.0f)
        , radarObjectShape_(Dot)
        , radarObjectDescription_("staticObject")
        , MapNode_(NULL)
        , MapEntity_(NULL)
        , line_(NULL)
    {
        RegisterRootObject(RadarViewable);

        this->uniqueId_=getUniqueNumberString();
/*
        if(Map::getSingletonPtr() && Map::getSingletonPtr()->getMapSceneManagerPtr())
        {
            this->addEntity();
        }

        */
    }


    RadarViewable::~RadarViewable()
    {
        delete MapNode_;
        delete MapEntity_;
    }

    void RadarViewable::addMapEntity()
    { //TODO Check shape and add accordantly
        if( this->MapNode_ && !this->MapEntity_ && Map::getSingletonPtr() && Map::getSingletonPtr()->getMapSceneManagerPtr() )
        {
            COUT(0) << "Adding " << this->uniqueId_ << " to Map.\n";
            this->MapEntity_ = Map::getSingletonPtr()->getMapSceneManagerPtr()->createEntity( this->uniqueId_, "drone.mesh");
            /*this->line_ =  Map::getSingletonPtr()->getMapSceneManagerPtr()->createManualObject(this->uniqueId_ + "_l"); 
            this->line_->begin("Map/line_", Ogre::RenderOperation::OT_LINE_STRIP); 
            //line_->position(0, -it->getRVWorldPosition().y, 0); 
            //this->line_->position(0, -20, 0); 
            this->line_->position(0, 0, -10); //Front Arrow
            this->line_->position(0, 0, 0);

            this->line_->end(); */
            this->line_ = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
            this->line_->addPoint( Vector3(0,0,0) );
            this->line_->addPoint( Vector3(0,0,0) );

            this->MapNode_->attachObject( this->MapEntity_ );
            this->MapNode_->attachObject( this->line_ );
        }
        else
        {
            COUT(0) << "Unable to load " << this->uniqueId_ << " to Map.\n";
        }
    }

    void RadarViewable::updateMapPosition()
    {
        if( this->MapNode_ )
        {
            this->MapNode_->setPosition( this->getRVWorldPosition() );
            this->MapNode_->translate( this->getRVOrientedVelocity(), (Ogre::Node::TransformSpace)3 );
            this->MapNode_->setOrientation( this->getWorldEntity()->getOrientation() );
Vector3 v = this->getRVWorldPosition();
            this->line_->setPoint(1, Vector3(0,v.y,0) );
        }
    }

    void RadarViewable::setRadarObjectDescription(const std::string& str)
    {
        Radar* radar = Radar::getInstancePtr();
        if (radar)
            this->radarObjectShape_ = radar->addObjectDescription(str);
        else
        {
            CCOUT(2) << "Attempting to access the radar, but the radar is non existent." << std::endl;
        }
        this->radarObjectDescription_ = str;
    }

    const Vector3& RadarViewable::getRVWorldPosition() const
    {
        const WorldEntity* object = this->getWorldEntity();
        validate(object);
        return object->getWorldPosition();
    }

    Vector3 RadarViewable::getRVOrientedVelocity() const
    {
        const WorldEntity* object = this->getWorldEntity();
        validate(object);
        return object->getWorldOrientation() * object->getVelocity();
    }
}
