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

/**
 @file  orxonox.cc
 @brief Orxonox Main File
 */

#include <Ogre.h>
#include <OIS/OIS.h>
//#include <CEGUI/CEGUI.h>
//#include <OgreCEGUIRenderer.h>

#include <string>
#include <iostream>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#include "windows.h"



// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
  char path[1024];
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  assert(mainBundle);

  CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
  assert(mainBundleURL);

  CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
  assert(cfStringRef);

  CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

  CFRelease(mainBundleURL);
  CFRelease(cfStringRef);

  return std::string(path);
}
#endif

namespace orxonox
{
      Ogre::RenderWindow * mWindow;
      Ogre::SceneManager *mgr;
      int stopper = 1;


  class OrxApplication
  {
    public:
      void go()
      {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        createScene();
        setupScene();
        setupInputSystem();
        setupCEGUI();
        createFrameListener();
        startRenderLoop();
      }

      OrxApplication()
      {
        OrxApplication::pointer_s = this;
        this->x = 0;
      }

      ~OrxApplication()
      {
        mInputManager->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputManager);

        delete mListener;
        delete mRoot;
      }


      void createRoot()
      {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        mRoot = new Ogre::Root(macBundlePath() + "/Contents/Resources/plugins.cfg");
#else
        mRoot = new Ogre::Root();
#endif
      }

      void defineResources()
      {
        Ogre::String secName, typeName, archName;
        Ogre::ConfigFile cf;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
#else
        cf.load("resources.cfg");
#endif

        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
          secName = seci.peekNextKey();
          Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
          Ogre::ConfigFile::SettingsMultiMap::iterator i;
          for (i = settings->begin(); i != settings->end(); ++i)
          {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation( String(macBundlePath() + "/" + archName), typeName, secName);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
#endif
          }
        }
      }

      void setupRenderSystem()
      {
        if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
          throw Ogre::Exception(52, "User canceled the config dialog!", "OrxApplication::setupRenderSystem()");
      }

      void createRenderWindow()
      {
        mWindow = mRoot->initialise(true, "Ogre Render Window");
      }

      void initializeResourceGroups()
      {
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
      }

      void createScene(void)
      {

      }

      void setupScene()
      {
        mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
        cam = mgr->createCamera("Camera");
        cam->setPosition(Ogre::Vector3(0,0,0));
        //cam->lookAt(Ogre::Vector3(0,0,0));
	cam->setFOVy(Ogre::Degree(90));
        Ogre::Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
        mgr->setSkyBox(true, "Examples/SpaceSkyBox");

	// camera pitch here works

	
      }

      void setupInputSystem()
      {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;
        Ogre::RenderWindow *win = mRoot->getAutoCreatedWindow();

        win->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);

        try
        {
          mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
          mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
        }
        catch (const OIS::Exception &e)
        {
          throw new Ogre::Exception(42, e.eText, "OrxApplication::setupInputSystem");
        }
      }

      void setupCEGUI()
      {
        Ogre::SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
        Ogre::RenderWindow *win = mRoot->getAutoCreatedWindow();
      }
      
      void screenShots()
      {

	Ogre::Radian x1 = Ogre::Radian(x);

	unsigned int indice = 1;
	char filename[30];
	char fn[1];
	// generate new names...
	sprintf(filename, "SkyBox%d.png", stopper);
	//sprintf(filename, "screenshot_%d.png", ++indice);
	//sprintf(filename, fn, ++indice);
	

	Ogre::Radian x2 = Ogre::Radian((3.141592653589/2.0)*(stopper+1));


	if(stopper == 1){
		        cam->yaw(Ogre::Degree(90));
	sprintf(filename, "SkyBox%d.png", stopper+1035);
	mWindow->writeContentsToFile(filename);
	
	}
	else if(stopper == 2){
		        cam->yaw(Ogre::Degree(180));
	mWindow->writeContentsToFile(filename);
	
	}
	else if(stopper == 3){
		        cam->yaw(Ogre::Degree(270));	
	mWindow->writeContentsToFile(filename);

	}
	else if(stopper == 4){
		        cam->yaw(Ogre::Degree(0));	
	mWindow->writeContentsToFile(filename);

	}

	else if(stopper == 5){

		        cam->yaw(Ogre::Degree(0));	
	mWindow->writeContentsToFile(filename);

	}
	else if(stopper == 6){
		        cam->yaw(Ogre::Degree(180));	
	mWindow->writeContentsToFile(filename);

	}

	/*else if(stopper == 6){
		        cam->roll(Ogre::Degree(270));	
	}*/

	//cam->pitch(Ogre::Degree(90)*stopper);

	stopper+=1;
	if(stopper >= 7)
	stopper = 1;
      }

  class OrxExitListener : public Ogre::FrameListener
  {
    public:
      OrxExitListener(OIS::Keyboard *keyboard)
    : mKeyboard(keyboard)
      {
      }
      bool frameEnded(const Ogre::FrameEvent& evt)
	{
		if(mKeyboard->isKeyDown(OIS::KC_RETURN) or stopper > 1){
			OrxApplication::pointer_s->screenShots();
		}

                return true;
	}
      
      bool frameStarted(const Ogre::FrameEvent& evt)
      {
        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
      }

	
    private:
      OIS::Keyboard *mKeyboard;
  };

      void createFrameListener()
      {
        mListener = new OrxExitListener(mKeyboard);
        mRoot->addFrameListener(mListener);
      }

      void startRenderLoop()
      {
        mRoot->startRendering();
      }


    private:
      Ogre::Root *mRoot;
      OIS::Keyboard *mKeyboard;
      OIS::Mouse *mMouse;
      OIS::InputManager *mInputManager;
//      CEGUI::OgreCEGUIRenderer *mRenderer;
//      CEGUI::System *mSystem;
      OrxExitListener *mListener;
      Ogre::Camera * cam;
      double x;

    public:
      static OrxApplication* pointer_s;
  };

  OrxApplication* OrxApplication::pointer_s = 0;
}

using namespace Ogre;

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

             INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
             int main(int argc, char **argv)
#endif
{
  try
  {
    orxonox::OrxApplication orxonox;
    orxonox.go();
  }
  catch(Exception& e)
  {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
  }

  return 0;
}

