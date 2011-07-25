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

#include "RadarViewable.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "tools/DynamicLines.h"
#include "worldentities/WorldEntity.h"
#include "Radar.h"
#include "Scene.h"
#include "overlays/Map.h"

namespace orxonox
{
    /**
        @brief Constructor.
    */
    RadarViewable::RadarViewable()
        : MapNode_(NULL)
        , MapEntity_(NULL)
        , line_(NULL)
        , LineNode_(NULL)
        , isHumanShip_(false)
        , bVisibility_(true)
        , radarObjectCamouflage_(0.0f)
        , radarObjectShape_(Dot)
        , radarObjectDescription_("staticObject")
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
        if (this->isHumanShip_ && MapNode_)
            MapNode_->removeAllChildren();

        if (MapNode_)
            delete MapNode_;

        if (MapEntity_)
            delete MapEntity_;

        if (line_)
            delete line_;

        if (LineNode_)
            delete LineNode_;
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
            this->line_ = new Ogre::DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
            this->line_->addPoint( Vector3(0,0,0) );
            this->line_->addPoint( Vector3(0,0,0) );

            this->MapNode_->attachObject( this->MapEntity_ );

            this->LineNode_ = this->MapNode_->createChildSceneNode();
            this->LineNode_->attachObject( this->line_ );
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
            this->MapNode_->translate( this->getRVOrientedVelocity(), static_cast<Ogre::Node::TransformSpace>(3) );
            this->MapNode_->setOrientation( this->getWorldEntity()->getOrientation() );
//Vector3 v = this->getRVWorldPosition();
            //this->line_->setPoint(1, Vector3(0,v.y,0) );
            this->line_->setPoint(1, Vector3( 0, static_cast<float>(static_cast<int>( -Map::getSingletonPtr()->movablePlane_->getDistance( this->getRVWorldPosition() ) ) ) ,0 ));
            this->line_->update();
            if( Map::getSingletonPtr()->playerShipNode_ )
                this->LineNode_->setDirection( Map::getSingletonPtr()->playerShipNode_->getLocalAxes().GetColumn(1) ,Ogre::Node::TS_WORLD,Vector3::UNIT_Y);
        }
    }

    void RadarViewable::setRadarObjectDescription(const std::string& str)
    {
        Radar* radar = this->getWorldEntity()->getScene()->getRadar();
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

    void RadarViewable::validate(const WorldEntity* object) const
    {
        if (!object)
        {
            COUT(1) << "Assertion: Every RadarViewable has to be assigned a WorldEntity pointer!" << std::endl;
            assert(0);
        }
    }
}
