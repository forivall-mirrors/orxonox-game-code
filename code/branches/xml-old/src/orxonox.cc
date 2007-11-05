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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

// TODO: Change this to orxonox.h and include all necessary functions there
#include "ExampleApplication.h"

#include <string>
#include <iostream>

#include "xml/xmlParser.h"
#include "loader/LevelLoader.h"



using namespace std;

// TODO: Put creation of SceneNode and implementation of FrameListener into an extern file
SceneNode *lightNode;

class FrameListener : public ExampleFrameListener
{
  public:
    FrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
  : ExampleFrameListener(win, cam, false, false)
    {
    }

    bool frameStarted(const FrameEvent &evt)
    {
        // add tutorial code here:
        // ...
      lightNode->translate(Vector3(0, -10 * evt.timeSinceLastFrame, 0));

      return ExampleFrameListener::frameStarted(evt);
    }
  private:
};

// TODO: Make Doxygen tags work and create scene AFTER loading in an extern file
//! This is the application class of Orxonox
/**
  Application class. The starting point of Orxonox.
  Loading of ressources should start in here.
  ...
*/
class Orxonox : public ExampleApplication
{
  protected:
  public:
    Orxonox()
    {
    }

    ~Orxonox()
    {
    }
  protected:
    void createCamera(void)
    {
        // create camera
      mCamera = mSceneMgr->createCamera("PlayerCam");
      mCamera->setNearClipDistance(5);
      mCamera->setPosition(Vector3(0,10,500));
      mCamera->lookAt(Vector3(0,0,0));
    }

    void createScene(void)
    {

    	string levelFile = "sp_level_moonstation.oxw";
			loader::LevelLoader* loader = new loader::LevelLoader(levelFile);

/*			
			LightManager* lightman = new LightManager();
			loader->loadLights(lightman);
			CameraManager* cameraman = new CameraManager();
			loader->loadCameras(cameraman);			    
			ScriptManager* scriptman = new ScriptManager();
			loader->loadScripts(scriptman);			    
	*/		    			    
    
    
        // add tutorial code here:
        // ...
        
        /*
      mSceneMgr->setAmbientLight( ColourValue( 1, 1, 0.1 ) );
      Entity* head = mSceneMgr->createEntity("head", "ogrehead.mesh");

      Entity* head2 = mSceneMgr->createEntity("head2", "ogrehead.mesh");

      SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHeadNode", Vector3( 0, 0, 0 ) );
      node->attachObject( head );

      SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHeadNode2", Vector3( 50, 0, 0 ) );
      node2->attachObject( head2 );


      //mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

      Light *light = mSceneMgr->createLight("Light1");
      light->setType(Light::LT_POINT);
      light->setPosition(Vector3(0, 100, 0));
      light->setDiffuseColour(0.5, 0.5, 0.0);
      light->setSpecularColour(0.5, 0.5, 0.0);

      BillboardSet *bbs = mSceneMgr->createBillboardSet("bb", 1);
      bbs->createBillboard(Vector3::ZERO, ColourValue(1.0, 0.0, 0.0));
      //bbs->setMaterialName("Examples/Flare");

      lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LightNode", Vector3(0, 100, 0));
      lightNode->attachObject(bbs);
      lightNode->attachObject(light);
      light->setPosition(0.0, 0.0, 0.0);
      
      */
    }

    void createFrameListener(void)
    {
        // create frame listener
      mFrameListener = new ExampleFrameListener(mWindow, mCamera, mSceneMgr);
      mRoot->addFrameListener(mFrameListener);
    }
};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
  // Create application object
  Orxonox orxonox;

  try 
  {
    orxonox.go();
  } 
  catch( Exception& e ) 
  {
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    	MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
    	fprintf(stderr, "An exception has occurred: %s\n",e.getFullDescription().c_str());
		#endif
  }
  return 0;
}
