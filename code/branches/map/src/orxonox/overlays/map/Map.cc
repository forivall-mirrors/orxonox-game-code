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
 *      Si Sun
 *
 */
#include "OrxonoxStableHeaders.h"
#include "Map.h"
 
#include <string>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "objects/Scene.h"
 
 namespace orxonox
 {
    CreateFactory(Map);
     SetConsoleCommand(Map, openMap, true);
    
    Map::Map(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        RegisterObject(Map);
        
        //Getting Scene Manager (Hack)
        ObjectList<Scene>::iterator it = ObjectList<Scene>::begin();
        this->sManager_ = it->getSceneManager();
        
        this->sNode_ = new Ogre::SceneNode(sManager_);
        oManager_ = Ogre::OverlayManager::getSingletonPtr();
        overlay_ = oManager_->create("Map");
        //overlay_ is member of OrxonoxOverlay
        
        //Not Showing the map as default
        this->isVisible_=false;
        overlay_->hide();
        
        //TestEntity
        Ogre::Entity * ent = sManager_->createEntity("ent", "drone.mesh");
        sNode_->attachObject( ent );
        sNode_->setPosition(0,0,-50);
        overlay_->add3D(sNode_);
    }
    
    void Map::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(Map, XMLPort, xmlElement, mode);
    }

    void Map::toggleVisibility()
    {
        if (!(this->isVisible_))
        {
            this->overlay_->show();
            this->isVisible_=1;
        }
        else
        {
            this->overlay_->hide();
            this->isVisible_=0;
        }
    }
    
    //Static function to toggle visibility of the map
    void Map::openMap()
    {
        for(ObjectList<orxonox::Map>::iterator it = ObjectList<orxonox::Map>::begin();
            it!=ObjectList<orxonox::Map>::end();
            it++)
        {
        //Map * m = it->getMap();
        COUT(0) << it->isVisible_ << std::endl;
        it->toggleVisibility();
        }
    }
    
    void Map::tick(float dt)
    {
        sNode_->lookAt(Vector3::NEGATIVE_UNIT_Z, Ogre::Node::TS_WORLD, Vector3::NEGATIVE_UNIT_Z);
        
    }
    
 }
