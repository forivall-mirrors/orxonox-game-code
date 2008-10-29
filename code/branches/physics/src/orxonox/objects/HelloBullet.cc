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
 *      Martin Stypinski
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "HelloBullet.h"

#include <OgreStaticGeometry.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "ogreode/OgreOde_Core.h"
#include "ogreode/OgreOdeGeometry.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/XMLPort.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    CreateFactory(HelloBullet);
    
    HelloBullet::HelloBullet()
    {
 	   RegisterObject(HelloBullet);
  	   COUT(0) << "HelloBullet loaded" << std::endl ;
           int maxProxies = 1024


 	   btVector3 worldAabbMin(-10000,-10000,-10000);
	   btVector3 worldAabbMax(10000,10000,10000);
    	   btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

	    // Set up the collision configuration and dispatcher
            btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
            btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

            // The actual physics solver
            btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

            // The world.
            btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    }

    HelloBullet::~HelloBullet()
    {
     // empty
    }

    void HelloBullet::setConfigValues()
    {

    }

    /**
    @brief
        XML loading and saving.
    @param
        xmlelement The XML-element
    @param	
        loading Loading (true) or saving (false)
    @return
        The XML-element
    */
    void HelloBullet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
      SUPER(HelloBullet, XMLPort, xmlelement, mode);
    }

    void HelloBullet::tick(float dt)
    {
        // only update physics in a certain interval
    }

}
