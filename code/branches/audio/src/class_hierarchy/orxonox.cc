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

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

#include "BaseObject.h"
#include "Test.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

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
  class OrxExitListener : public Ogre::FrameListener
  {
    public:
      OrxExitListener(OIS::Keyboard *keyboard)
    : mKeyboard(keyboard)
      {
      }

      bool frameStarted(const Ogre::FrameEvent& evt)
      {
        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
      }

    private:
      OIS::Keyboard *mKeyboard;
  };

  class OrxApplication
  {
    public:
      void go()
      {

        #define testandcout(code) \
          std::cout << #code << " " << code << "\n"


        std::cout << "Test 8\n";

        std::cout << "1\n";
        Test1* test8_01 = new Test1;
        Test3* test8_03 = new Test3;
        test8_03->usefullClassesIsATest(test8_01);

        std::cout << "2\n";
        Test2* test8_02 = new Test2;
        test8_03->usefullClassesIsATest(test8_02);

        std::cout << "3\n";
        test8_01->setUsefullClass1(Class(Test1));
        test8_01->setUsefullClass1(test8_02->getIdentifier());
        test8_01->setUsefullClass2(Class(Test2));
        test8_01->setUsefullClassOfTypeTest3(Class(Test3));
        test8_01->setUsefullClassOfTypeTest3(test8_03->getIdentifier());


        testandcout(test8_01->isA(Class(Test1)));
        testandcout(test8_01->isA(Class(Test2)));
        testandcout(test8_01->isA(Class(Test3)));

        Test2* test8_04 = new Test2;
        testandcout(test8_02->isA(Class(Test1)));
        testandcout(test8_02->isA(Class(Test2)));
        testandcout(test8_02->isA(Class(Test3)));

        Test3* test8_05 = new Test3;
        testandcout(test8_03->isA(Class(Test1)));
        testandcout(test8_03->isA(Class(Test2)));
        testandcout(test8_03->isA(Class(Test3)));

        delete test8_01;
        delete test8_02;
        delete test8_03;


      }

      ~OrxApplication()
      {
        mInputManager->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputManager);

        delete mRenderer;
        delete mSystem;

        delete mListener;
        delete mRoot;
      }

    private:
      Ogre::Root *mRoot;
      OIS::Keyboard *mKeyboard;
      OIS::Mouse *mMouse;
      OIS::InputManager *mInputManager;
      CEGUI::OgreCEGUIRenderer *mRenderer;
      CEGUI::System *mSystem;
      OrxExitListener *mListener;

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
        mRoot->initialise(true, "Ogre Render Window");
      }

      void initializeResourceGroups()
      {
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
      }

      void setupScene()
      {
        Ogre::SceneManager *mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
        Ogre::Camera *cam = mgr->createCamera("Camera");
        Ogre::Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
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

        // CEGUI setup
        mRenderer = new CEGUI::OgreCEGUIRenderer(win, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mgr);
        mSystem = new CEGUI::System(mRenderer);

        // Other CEGUI setup here.
      }

      void createFrameListener()
      {
        mListener = new OrxExitListener(mKeyboard);
        mRoot->addFrameListener(mListener);
      }

      void startRenderLoop()
      {
        mRoot->startRendering();
      }
  };
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
