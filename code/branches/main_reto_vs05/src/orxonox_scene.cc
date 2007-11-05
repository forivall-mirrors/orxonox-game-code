/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreLight.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"
#include "OgreVector3.h"

#include "orxonox_scene.h"

namespace orxonox {
  using namespace Ogre;

  /**
  * The orxonox scene includes everything running in the background like terrain,
  * static figures, dangling lamp, etc.
  */


  /**
  * Empty Consructor except the initialiser list.
  * @param sceneMgr The Scene Manager.
  */
  OrxonoxScene::OrxonoxScene(SceneManager *sceneMgr) : sceneMgr_(sceneMgr)
  {
  }

  /**
  * Empty Destructor.
  */
  OrxonoxScene::~OrxonoxScene()
  {
  }

  /**
  * Ogre initialisation method.
  * This function is called by the Run Manager to load the neccessary recources
  * and to create the scene.
  * @return False if failed.
  */
  bool OrxonoxScene::initialise()
  {
	  // Load resources
	  loadResources();

	  distance_ = 0;
	  radius_ = 100;

	  createScene();

	  return true;
  }


  /**
  * Resource loader.
  * Currently, this method loads everything! TODO: If done this ugly, it should
  * at least be in the Run Manager.
  */
  void OrxonoxScene::loadResources()
  {
	  // Initialise, parse scripts etc
	  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }


  /**
  * Scene creation.
  * Currently just a test scene with an ogre head an a surrounding light.
  */
  void OrxonoxScene::createScene()
  {
	  sceneMgr_->setAmbientLight(ColourValue(0.3,0.3,0.3));

	  //create first entity
	  Entity *head = sceneMgr_->createEntity("head", "ogrehead.mesh");

	  //create a scene node to attach the head to
	  SceneNode *node = sceneMgr_->getRootSceneNode()
          ->createChildSceneNode("OgreHeadNode", Vector3(0,0,0));
	  //attach the ogre head
	  node->attachObject(head);

	  // set up skybox
	  sceneMgr_->setSkyBox(true, "Examples/SceneSkyBox2");

	  // set up one light_ source
	  light_ = sceneMgr_->createLight("Light1");
	  light_->setType(Light::LT_POINT);
	  light_->setPosition(Vector3(0, 0, 0));
	  light_->setDiffuseColour(1.0, 1.0, 1.0);
	  light_->setSpecularColour(1.0, 1.0, 1.0);

	  //create billboard
	  bbs_ = sceneMgr_->createBillboardSet("bb", 1);
	  bbs_->createBillboard(Vector3::ZERO, ColourValue(1.0, 1.0, 1.0));
	  bbs_->setMaterialName("Examples/Flare");

	  lightNode_ = sceneMgr_->getRootSceneNode()
          ->createChildSceneNode("lightNode_", Vector3(0, 100, 0));

	  lightNode_->attachObject(bbs_);
	  lightNode_->attachObject(light_);
  }


  /**
  * Compute something between frames if neccessary.
  * @param time Absolute time.
  * @param deltaTime Relative time.
  * @return Return true to continue rendering.
  */
  bool OrxonoxScene::tick(unsigned long time, Real deltaTime)
  {
	  Real t = time/1000.0;

	  lightNode_->setPosition(radius_*sin(5*t), radius_*cos(5*t), sin(1*t)*distance_);
  	
	  light_->setDiffuseColour(sin(1*t), sin(1*t + 2.09), sin(1*t + 2.09*2));
	  light_->setSpecularColour(sin(1*t), sin(1*t + 2.09), sin(1*t + 2.09*2));

	  bbs_->getBillboard(0)->setColour(ColourValue(sin(1*t),
          sin(1*t + 2.09), sin(1*t + 2.09*2)));
    
    return true;
  }

}
