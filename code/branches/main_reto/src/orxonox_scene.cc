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

#include "orxonox_scene.h"


OrxonoxScene::OrxonoxScene(SceneManager *mSceneMgr) : mSceneMgr(mSceneMgr)
{
}


OrxonoxScene::~OrxonoxScene()
{
}


bool OrxonoxScene::initialise()
{
	// Load resources
	loadResources();

	distance = 0;
	radius = 100;

	createScene();

	return true;
}


// method where you can perform resource group loading
// Must at least do
// ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
void OrxonoxScene::loadResources(void)
{
	// Initialise, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


// Currently just a test scene with an ogre head an a surrounding light
void OrxonoxScene::createScene(void)
{
	mSceneMgr->setAmbientLight(ColourValue(0.3,0.3,0.3));

	//create first entity
	Entity *head = mSceneMgr->createEntity("head", "ogrehead.mesh");

	//create a scene node to attach the head to
	SceneNode *node = mSceneMgr->getRootSceneNode()
        ->createChildSceneNode("OgreHeadNode", Vector3(0,0,0));
	//attach the ogre head
	node->attachObject(head);

	// set up skybox
	mSceneMgr->setSkyBox(true, "Examples/SceneSkyBox2");

	// set up one mLight source
	mLight = mSceneMgr->createLight("Light1");
	mLight->setType(Light::LT_POINT);
	mLight->setPosition(Vector3(0, 0, 0));
	mLight->setDiffuseColour(1.0, 1.0, 1.0);
	mLight->setSpecularColour(1.0, 1.0, 1.0);

	//create billboard
	bbs = mSceneMgr->createBillboardSet("bb", 1);
	bbs->createBillboard(Vector3::ZERO, ColourValue(1.0, 1.0, 1.0));
	bbs->setMaterialName("Examples/Flare");

	lightNode = mSceneMgr->getRootSceneNode()
        ->createChildSceneNode("LightNode", Vector3(0, 100, 0));

	lightNode->attachObject(bbs);
	lightNode->attachObject(mLight);
}


// compute something between frames if neccessary
void OrxonoxScene::tick(unsigned long time, float deltaTime)
{
	float t = time/1000.0;

	lightNode->setPosition(radius*sin(5*t), radius*cos(5*t), sin(1*t)*distance);
	
	mLight->setDiffuseColour(sin(1*t), sin(1*t + 2.09), sin(1*t + 2.09*2));
	mLight->setSpecularColour(sin(1*t), sin(1*t + 2.09), sin(1*t + 2.09*2));

	bbs->getBillboard(0)->setColour(ColourValue(sin(1*t),
        sin(1*t + 2.09), sin(1*t + 2.09*2)));
}
