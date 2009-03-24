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
        
        //COUT(0) << "Map loaded\n" << this->getScene();
        Ogre::SceneManager* sm = this->getScene()->getSceneManager();
        Ogre::SceneNode* sn = new Ogre::SceneNode(sm);
        COUT(0) << "Map loaded\n" << sm << " / " << sn << std::endl;
    }
    
    void Map::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(Map, XMLPort, xmlElement, mode);
    }
    
    void Map::changedVisibility()
    {
        SUPER(Map, changedVisibility);
        
    }
    
    void Map::tick(float dt)
    {
    }
    
 }
