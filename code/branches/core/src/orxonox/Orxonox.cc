/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  Orxonox.cc
 @brief Orxonox Main Class
 */

// Precompiled Headers
#include "OrxonoxStableHeaders.h"

//****** OGRE ******
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreFrameListener.h>
#include <OgreRenderWindow.h>
#include <OgreTextureManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreConfigFile.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

//****** OIS *******
#include <OIS/OIS.h>

//****** STD *******
#include <iostream>
#include <exception>

//***** ORXONOX ****
//misc
#include "util/Sleep.h"

// loader and audio
//#include "loader/LevelLoader.h"
//#include "audio/AudioManager.h"

// network
//#include "network/Server.h"
//#include "network/Client.h"
//#include "network/NetworkFrameListener.h"

// objects
#include "objects/Tickable.h"
#include "tools/Timer.h"
//#include "objects/NPC.h"
#include "core/ArgReader.h"
#include "core/Factory.h"
#include "core/Debug.h"
//#include "hud/HUD.h"
//#include "objects/weapon/BulletManager.h"
#include "GraphicsEngine.h"

#include "core/ClassTreeMask.h"
#include "objects/WorldEntity.h"
#include "core/DebugLevel.h"
#include "core/BaseObject.h"
#include "objects/Test.h"
#include "objects/test1.h"
#include "objects/test2.h"
#include "objects/test3.h"
#include "core/Loader.h"

#include "Orxonox.h"

namespace orxonox
{
   // put this in a seperate Class or solve the problem in another fashion
  class OrxListener : public Ogre::FrameListener
  {
    public:
      OrxListener(OIS::Keyboard *keyboard/*, audio::AudioManager*  auMan*/, gameMode mode)
      {
        mKeyboard = keyboard;
        mode_=mode;
//        auMan_ = auMan;
      }

      bool frameStarted(const Ogre::FrameEvent& evt)
      {
//        auMan_->update();
        updateAI();

/*        if(mode_ == PRESENTATION)
          server_g->tick(evt.timeSinceLastFrame);
        else if(mode_ == CLIENT)
          client_g->tick(evt.timeSinceLastFrame);
*/
        usleep(10);

        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
      }

      void updateAI()
      {/*
        for(Iterator<NPC> it = ObjectList<NPC>::start(); it; ++it)
        {
          it->update();
        }*/
      }

    private:
      gameMode mode_;
      OIS::Keyboard *mKeyboard;
//      audio::AudioManager*  auMan_;
  };

  // init static singleton reference of Orxonox
  Orxonox* Orxonox::singletonRef_ = NULL;

  /**
   * create a new instance of Orxonox
   */
  Orxonox::Orxonox()
  {
    this->ogre_ = new GraphicsEngine();
    this->dataPath_ = "";
//    this->loader_ = 0;
//    this->auMan_ = 0;
    this->singletonRef_ = 0;
    this->keyboard_ = 0;
    this->mouse_ = 0;
    this->inputManager_ = 0;
    this->frameListener_ = 0;
    this->root_ = 0;
  }

  /**
   * destruct Orxonox
   */
  Orxonox::~Orxonox()
  {
    // nothing to delete as for now
  }

  /**
   * initialization of Orxonox object
   * @param argc argument counter
   * @param argv list of arguments
   * @param path path to config (in home dir or something)
   */
  void Orxonox::init(int argc, char **argv, std::string path)
  {
    //TODO: find config file (assuming executable directory)
    //TODO: read config file
    //TODO: give config file to Ogre
    std::string mode;
//     if(argc>=2)
//       mode = std::string(argv[1]);
//     else
//       mode = "";
    ArgReader ar = ArgReader(argc, argv);
    ar.checkArgument("mode", mode, false);
    ar.checkArgument("data", this->dataPath_, false);
    ar.checkArgument("ip", serverIp_, false);
    //mode = "presentation";
    if(ar.errorHandling()) die();
    if(mode == std::string("server"))
    {
      serverInit(path);
      mode_ = SERVER;
    }
    else if(mode == std::string("client"))
    {
      clientInit(path);
      mode_ = CLIENT;
    }
    else if(mode == std::string("presentation"))
    {
      serverInit(path);
      mode_ = PRESENTATION;
    }
    else{
      standaloneInit(path);
      mode_ = STANDALONE;
    }
  }

  /**
   * start modules
   */
  void Orxonox::start()
  {
    //TODO: start modules
//    ogre_->startRender();
    //TODO: run engine
    Factory::createClassHierarchy();
//    createScene();
//    setupScene();
//    setupInputSystem();
    if(mode_!=CLIENT){ // remove this in future ---- presentation hack
    }
    else
      std::cout << "client here" << std::endl;
    createFrameListener();
    switch(mode_){
    case PRESENTATION:
      //ogre_->getRoot()->addFrameListener(new network::ServerFrameListener());
      //std::cout << "could not add framelistener" << std::endl;
//      server_g->open();
      break;
    case CLIENT:
//      client_g->establishConnection();
      break;
    case SERVER:
    case STANDALONE:
    default:
      break;
    }
        #define testandcout(code) \
          std::cout << #code << " " << code << "\n"
/*
        std::cout << "Test 1\n";
        BaseObject* test1;
        test1 = new BaseObject();
        test1 = new BaseObject();
        test1 = new BaseObject();

        std::cout << "Test 2\n";
        A1* test2;
        test2 = new A1();
        test2 = new A1();
        test2 = new A1();

        std::cout << "Test 3\n";
        BaseObject* test3;
        test3 = new BaseObject();
        test3 = new BaseObject();
        test3 = new BaseObject();

        std::cout << "Test 4\n";
        A3* test4;
        test4 = new A3();
        test4 = new A3();
        test4 = new A3();


        std::cout << "Test 5\n";
        A1* test5_01 = new A1();
        A2* test5_02 = new A2();
        A3* test5_03 = new A3();
        A1B1* test5_04 = new A1B1();
        A1B2* test5_05 = new A1B2();
        A2B1* test5_06 = new A2B1();
        A2B2* test5_07 = new A2B2();
        A3B1* test5_08 = new A3B1();
        A3B2* test5_09 = new A3B2();
        A1B1C1* test5_10 = new A1B1C1();
        A1B1C2* test5_11 = new A1B1C2();
        A1B2C1* test5_12 = new A1B2C1();
        A2B1C1* test5_13 = new A2B1C1();
        A2B2C1* test5_14 = new A2B2C1();
        A3B1C1* test5_15 = new A3B1C1();
        A3B1C2* test5_16 = new A3B1C2();
        A3B2C1* test5_17 = new A3B2C1();
        A3B2C2* test5_18 = new A3B2C2();
*/
/*
        OrxonoxClass* test5 = 0;
        for (int i = 0; i <= 18; i++)
        {
          if (i == 0) test5 = new BaseObject;
          if (i == 1) test5 = test5_01;
          if (i == 2) test5 = test5_02;
          if (i == 3) test5 = test5_03;
          if (i == 4) test5 = test5_04;
          if (i == 5) test5 = test5_05;
          if (i == 6) test5 = test5_06;
          if (i == 7) test5 = test5_07;
          if (i == 8) test5 = test5_08;
          if (i == 9) test5 = test5_09;
          if (i == 10) test5 = test5_10;
          if (i == 11) test5 = test5_11;
          if (i == 12) test5 = test5_12;
          if (i == 13) test5 = test5_13;
          if (i == 14) test5 = test5_14;
          if (i == 15) test5 = test5_15;
          if (i == 16) test5 = test5_16;
          if (i == 17) test5 = test5_17;
          if (i == 18) test5 = test5_18;

          std::cout << "\n";
          std::cout << test5->getIdentifier()->getName() << " (" << test5->getIdentifier()->getNetworkID() << "): parents:         " << test5->getIdentifier()->getParents() << "\n";
          std::cout << test5->getIdentifier()->getName() << " (" << test5->getIdentifier()->getNetworkID() << "): children:        " << test5->getIdentifier()->getChildren() << "\n";
          std::cout << test5->getIdentifier()->getName() << " (" << test5->getIdentifier()->getNetworkID() << "): direct parents:  " << test5->getIdentifier()->getDirectParents() << "\n";
          std::cout << test5->getIdentifier()->getName() << " (" << test5->getIdentifier()->getNetworkID() << "): direct children: " << test5->getIdentifier()->getDirectChildren() << "\n";
        }
*/
/*
        for (std::map<std::string, Identifier*>::const_iterator it = Factory::getFactoryBegin(); it != Factory::getFactoryEnd(); ++it)
            std::cout << "Class with ID " << (*it).second->getNetworkID() << ": " << ID((*it).second->getNetworkID()) << " / " << ID((*it).second->getNetworkID())->getName() << std::endl;

        std::cout << "Class with ID 100: " << ID(100) << "\n";
        std::cout << "ID of BaseObject: " << Class(BaseObject)->getNetworkID() << "\n";

        std::cout << "\nChange ID of BaseObject to 100\n";
        Class(BaseObject)->setNetworkID(100);
        std::cout << "Class with ID 100: " << ID(100) << "\n";
        std::cout << "Class with ID 1: " << ID(1) << "\n";
        std::cout << "ID of BaseObject: " << Class(BaseObject)->getNetworkID() << "\n";

        std::cout << "\nChange ID of BaseObject to 3\n";
        Class(BaseObject)->setNetworkID(3);
        std::cout << "Class with ID 100: " << ID(100) << "\n";
        std::cout << "Class with ID 1: " << ID(1) << "\n";
        std::cout << "Class with ID 3: " << ID(3) << "\n";
        std::cout << "ID of BaseObject: " << Class(BaseObject)->getNetworkID() << "\n";
        std::cout << "ID of Test1: " << Class(Test1)->getNetworkID() << "\n";
*/
/*
        std::cout << "\n";
        std::cout << "isA(XYZ)-Test:\n";
        std::cout << "test5_01 = A1, Erwartet: 1 0 0 1 0\n";
        testandcout(test5_01->isA(Class(A1)));
        testandcout(test5_01->isA(Class(A2)));
        testandcout(test5_01->isA(Class(A1B1)));
        testandcout(test5_01->isA(Class(BaseObject)));
        testandcout(test5_01->isA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_02 = A2, Erwartet: 0 1 0 1 0\n";
        testandcout(test5_02->isA(Class(A1)));
        testandcout(test5_02->isA(Class(A2)));
        testandcout(test5_02->isA(Class(A1B1)));
        testandcout(test5_02->isA(Class(BaseObject)));
        testandcout(test5_02->isA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_01 = A3, Erwartet: 0 0 0 1 1\n";
        testandcout(test5_03->isA(Class(A1)));
        testandcout(test5_03->isA(Class(A2)));
        testandcout(test5_03->isA(Class(A1B1)));
        testandcout(test5_03->isA(Class(BaseObject)));
        testandcout(test5_03->isA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "isDirectA(XYZ)-Test:\n";
        std::cout << "test5_01 = A1, Erwartet: 1 0 0 0 0\n";
        testandcout(test5_01->isExactlyA(Class(A1)));
        testandcout(test5_01->isExactlyA(Class(A2)));
        testandcout(test5_01->isExactlyA(Class(A1B1)));
        testandcout(test5_01->isExactlyA(Class(BaseObject)));
        testandcout(test5_01->isExactlyA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_02 = A2, Erwartet: 0 1 0 0 0\n";
        testandcout(test5_02->isExactlyA(Class(A1)));
        testandcout(test5_02->isExactlyA(Class(A2)));
        testandcout(test5_02->isExactlyA(Class(A1B1)));
        testandcout(test5_02->isExactlyA(Class(BaseObject)));
        testandcout(test5_02->isExactlyA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_03 = A3, Erwartet: 0 0 0 0 0\n";
        testandcout(test5_03->isExactlyA(Class(A1)));
        testandcout(test5_03->isExactlyA(Class(A2)));
        testandcout(test5_03->isExactlyA(Class(A1B1)));
        testandcout(test5_03->isExactlyA(Class(BaseObject)));
        testandcout(test5_03->isExactlyA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "isChildOf(XYZ)-Test:\n";
        std::cout << "test5_04 = A1B1, Erwartet: 1 0 1 0 0 0 0\n";
        testandcout(test5_04->isChildOf(Class(A1)));
        testandcout(test5_04->isChildOf(Class(A2)));
        testandcout(test5_04->isChildOf(Class(BaseObject)));
        testandcout(test5_04->isChildOf(Class(Interface1)));
        testandcout(test5_04->isChildOf(Class(Interface2)));
        testandcout(test5_04->isChildOf(Class(A1B1C2)));
        testandcout(test5_04->isChildOf(Class(A1B1)));

        std::cout << "\n";
        std::cout << "test5_06 = A2B1, Erwartet: 0 1 1 0 0 0 0\n";
        testandcout(test5_06->isChildOf(Class(A1)));
        testandcout(test5_06->isChildOf(Class(A2)));
        testandcout(test5_06->isChildOf(Class(BaseObject)));
        testandcout(test5_06->isChildOf(Class(Interface1)));
        testandcout(test5_06->isChildOf(Class(Interface2)));
        testandcout(test5_06->isChildOf(Class(A1B1C2)));
        testandcout(test5_06->isChildOf(Class(A1B1)));

        std::cout << "\n";
        std::cout << "test5_07 = A2B2, Erwartet: 0 1 1 1 0 0\n";
        testandcout(test5_07->isChildOf(Class(A1)));
        testandcout(test5_07->isChildOf(Class(A2)));
        testandcout(test5_07->isChildOf(Class(BaseObject)));
        testandcout(test5_07->isChildOf(Class(Interface1)));
        testandcout(test5_07->isChildOf(Class(Interface2)));
        testandcout(test5_07->isChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "test5_08 = A3B1, Erwartet: 0 0 1 1 0 0\n";
        testandcout(test5_08->isChildOf(Class(A1)));
        testandcout(test5_08->isChildOf(Class(A2)));
        testandcout(test5_08->isChildOf(Class(BaseObject)));
        testandcout(test5_08->isChildOf(Class(Interface1)));
        testandcout(test5_08->isChildOf(Class(Interface2)));
        testandcout(test5_08->isChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "test5_09 = A3B2, Erwartet: 0 0 1 1 1 0\n";
        testandcout(test5_09->isChildOf(Class(A1)));
        testandcout(test5_09->isChildOf(Class(A2)));
        testandcout(test5_09->isChildOf(Class(BaseObject)));
        testandcout(test5_09->isChildOf(Class(Interface1)));
        testandcout(test5_09->isChildOf(Class(Interface2)));
        testandcout(test5_09->isChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "isDirectChildOf(XYZ)-Test:\n";
        std::cout << "test5_04 = A1B1, Erwartet: 1 0 0 0 0 0 0\n";
        testandcout(test5_04->isDirectChildOf(Class(A1)));
        testandcout(test5_04->isDirectChildOf(Class(A2)));
        testandcout(test5_04->isDirectChildOf(Class(BaseObject)));
        testandcout(test5_04->isDirectChildOf(Class(Interface1)));
        testandcout(test5_04->isDirectChildOf(Class(Interface2)));
        testandcout(test5_04->isDirectChildOf(Class(A1B1C2)));
        testandcout(test5_04->isDirectChildOf(Class(A1B1)));

        std::cout << "\n";
        std::cout << "test5_06 = A2B1, Erwartet: 0 1 0 0 0 0 0\n";
        testandcout(test5_06->isDirectChildOf(Class(A1)));
        testandcout(test5_06->isDirectChildOf(Class(A2)));
        testandcout(test5_06->isDirectChildOf(Class(BaseObject)));
        testandcout(test5_06->isDirectChildOf(Class(Interface1)));
        testandcout(test5_06->isDirectChildOf(Class(Interface2)));
        testandcout(test5_06->isDirectChildOf(Class(A1B1C2)));
        testandcout(test5_06->isDirectChildOf(Class(A1B1)));

        std::cout << "\n";
        std::cout << "test5_07 = A2B2, Erwartet: 0 1 0 1 0 0\n";
        testandcout(test5_07->isDirectChildOf(Class(A1)));
        testandcout(test5_07->isDirectChildOf(Class(A2)));
        testandcout(test5_07->isDirectChildOf(Class(BaseObject)));
        testandcout(test5_07->isDirectChildOf(Class(Interface1)));
        testandcout(test5_07->isDirectChildOf(Class(Interface2)));
        testandcout(test5_07->isDirectChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "test5_08 = A3B1, Erwartet: 0 0 0 0 0 0\n";
        testandcout(test5_08->isDirectChildOf(Class(A1)));
        testandcout(test5_08->isDirectChildOf(Class(A2)));
        testandcout(test5_08->isDirectChildOf(Class(BaseObject)));
        testandcout(test5_08->isDirectChildOf(Class(Interface1)));
        testandcout(test5_08->isDirectChildOf(Class(Interface2)));
        testandcout(test5_08->isDirectChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "test5_09 = A3B2, Erwartet: 0 0 0 0 1 0\n";
        testandcout(test5_09->isDirectChildOf(Class(A1)));
        testandcout(test5_09->isDirectChildOf(Class(A2)));
        testandcout(test5_09->isDirectChildOf(Class(BaseObject)));
        testandcout(test5_09->isDirectChildOf(Class(Interface1)));
        testandcout(test5_09->isDirectChildOf(Class(Interface2)));
        testandcout(test5_09->isDirectChildOf(Class(A1B1C2)));

        std::cout << "\n";

        std::cout << "isParentOf(XYZ)-Test:\n";
        std::cout << "test1 = BaseObject, Erwartet: 0 0 1 1 1 1 1\n";
        testandcout(test1->isParentOf(Class(BaseObject)));
        testandcout(test1->isParentOf(Class(Interface1)));
        testandcout(test1->isParentOf(Class(A1)));
        testandcout(test1->isParentOf(Class(A2)));
        testandcout(test1->isParentOf(Class(A1B1)));
        testandcout(test1->isParentOf(Class(A2B2)));
        testandcout(test1->isParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "test5_01 = A1, Erwartet: 0 0 0 0 1 0 0\n";
        testandcout(test5_01->isParentOf(Class(BaseObject)));
        testandcout(test5_01->isParentOf(Class(Interface1)));
        testandcout(test5_01->isParentOf(Class(A1)));
        testandcout(test5_01->isParentOf(Class(A2)));
        testandcout(test5_01->isParentOf(Class(A1B1)));
        testandcout(test5_01->isParentOf(Class(A2B2)));
        testandcout(test5_01->isParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "Interface1, Erwartet: 0 0 0 0 0 1 1\n";
        testandcout(Class(Interface1)->isParentOf(Class(BaseObject)));
        testandcout(Class(Interface1)->isParentOf(Class(Interface1)));
        testandcout(Class(Interface1)->isParentOf(Class(A1)));
        testandcout(Class(Interface1)->isParentOf(Class(A2)));
        testandcout(Class(Interface1)->isParentOf(Class(A1B1)));
        testandcout(Class(Interface1)->isParentOf(Class(A2B2)));
        testandcout(Class(Interface1)->isParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "isDirectParentOf(XYZ)-Test:\n";
        std::cout << "test1 = BaseObject, Erwartet: 0 0 1 1 0 0 0\n";
        testandcout(test1->isDirectParentOf(Class(BaseObject)));
        testandcout(test1->isDirectParentOf(Class(Interface1)));
        testandcout(test1->isDirectParentOf(Class(A1)));
        testandcout(test1->isDirectParentOf(Class(A2)));
        testandcout(test1->isDirectParentOf(Class(A1B1)));
        testandcout(test1->isDirectParentOf(Class(A2B2)));
        testandcout(test1->isDirectParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "test5_01 = A1, Erwartet: 0 0 0 0 1 0 0\n";
        testandcout(test5_01->isDirectParentOf(Class(BaseObject)));
        testandcout(test5_01->isDirectParentOf(Class(Interface1)));
        testandcout(test5_01->isDirectParentOf(Class(A1)));
        testandcout(test5_01->isDirectParentOf(Class(A2)));
        testandcout(test5_01->isDirectParentOf(Class(A1B1)));
        testandcout(test5_01->isDirectParentOf(Class(A2B2)));
        testandcout(test5_01->isDirectParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "Interface1, Erwartet: 0 0 0 0 0 1 0\n";
        testandcout(Class(Interface1)->isDirectParentOf(Class(BaseObject)));
        testandcout(Class(Interface1)->isDirectParentOf(Class(Interface1)));
        testandcout(Class(Interface1)->isDirectParentOf(Class(A1)));
        testandcout(Class(Interface1)->isDirectParentOf(Class(A2)));
        testandcout(Class(Interface1)->isDirectParentOf(Class(A1B1)));
        testandcout(Class(Interface1)->isDirectParentOf(Class(A2B2)));
        testandcout(Class(Interface1)->isDirectParentOf(Class(A3B1C2)));
*/
/*
        std::cout << "Test 6\n";
        std::cout << "1\n";
        Identifier* test6_01 = Class(A1B1);
        std::cout << "2\n";
        Identifier* test6_02 = Class(A1B1);
        std::cout << "3\n";
        Identifier* test6_03 = Class(A1);
        std::cout << "4\n";
        Identifier* test6_04 = Class(A1B1C1);
        std::cout << "5\n";
        Identifier* test6_05 = Class(A1B1);
        std::cout << "6\n";
        Identifier* test6_06 = Class(A1B1C1);

        std::cout << "7\n";

        std::cout << "\n";
        std::cout << "BaseObject: parents:         " << Class(BaseObject)->getParents() << "\n";
        std::cout << "BaseObject: children:        " << Class(BaseObject)->getChildren() << "\n";
        std::cout << "BaseObject: direct parents:  " << Class(BaseObject)->getDirectParents() << "\n";
        std::cout << "BaseObject: direct children: " << Class(BaseObject)->getDirectChildren() << "\n";

        std::cout << "\n";
        std::cout << "A1: parents:                 " << Class(A1)->getParents() << "\n";
        std::cout << "A1: children:                " << Class(A1)->getChildren() << "\n";
        std::cout << "A1: direct parents:          " << Class(A1)->getDirectParents() << "\n";
        std::cout << "A1: direct children:         " << Class(A1)->getDirectChildren() << "\n";

        std::cout << "\n";
        std::cout << "A1B1: parents:               " << Class(A1B1)->getParents() << "\n";
        std::cout << "A1B1: children:              " << Class(A1B1)->getChildren() << "\n";
        std::cout << "A1B1: direct parents:        " << Class(A1B1)->getDirectParents() << "\n";
        std::cout << "A1B1: direct children:       " << Class(A1B1)->getDirectChildren() << "\n";

        std::cout << "\n";
        std::cout << "A1B1C1: parents:             " << Class(A1B1C1)->getParents() << "\n";
        std::cout << "A1B1C1: children:            " << Class(A1B1C1)->getChildren() << "\n";
        std::cout << "A1B1C1: direct parents:      " << Class(A1B1C1)->getDirectParents() << "\n";
        std::cout << "A1B1C1: direct children:     " << Class(A1B1C1)->getDirectChildren() << "\n";

        std::cout << "\n";
        std::cout << "A3B1C1 child of A3:      " << Class(A3B1C1)->isChildOf(Class(A3)) << "\n";
        std::cout << "\n";
        std::cout << "A2 parent of A2B1C1:     " << Class(A2)->isParentOf(Class(A2B1C1)) << "\n";

        std::cout << "8\n";
*/
/*
        std::cout << "Test 7\n";
        std::cout << "1\n";
        SubclassIdentifier<A1B1> test7_01;
        test7_01 = Class(A1B1C1);

        SubclassIdentifier<A1B1> test7_02;
        test7_02 = Class(A1B1);

        std::cout << test7_01->getName() << "\n";
        std::cout << test7_02->getName() << "\n";
*/
/*
// ## WARNING - CRASH! ##
        std::cout << "2\n";

        SubclassIdentifier<A1B1> test7_03;
        test7_03 = Class(A1);

        SubclassIdentifier<A1B1> test7_04;
        test7_04 = Class(A1B2);

        SubclassIdentifier<A1B1> test7_05;
        test7_05 = Class(A2);
// ## END WARNING - CRASH! ##
*/
/*
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
*/
/*
        std::cout << "Test 9\n";
        std::cout << "1\n";
        Identifier* test9_01 = Class(A3);
        BaseObject* test9_02 = test9_01->fabricate();
        std::cout << test9_02->getIdentifier()->getName() << "\n";

        std::cout << "\n2\n";
        BaseObject* test9_03 = Class(A1B2)->fabricate();
        std::cout << test9_03->getIdentifier()->getName() << "\n";

        std::cout << "\n3\n";
        SubclassIdentifier<A1> test9_04;
        test9_04 = Class(A1B1C1);
        A1* test9_05 = test9_04.fabricate();
        std::cout << test9_05->getIdentifier()->getName() << "\n";

        std::cout << "\n4\n";
        BaseObject* test9_06 = ID("A2B2")->fabricate();
        std::cout << test9_06->getIdentifier()->getName() << "\n";

        std::cout << "\n5\n";
        delete test9_02;
        delete test9_03;
        delete test9_05;
        delete test9_06;
*/
/*
        std::cout << "Test 10\n";
        Identifier* test10_01 = Class(A1B2);
        Identifier* test10_02 = Class(A2);
        Identifier* test10_03 = Class(A3B1C1);

        BaseObject* test10_04 = test10_01->fabricate();
        BaseObject* test10_05 = test10_02->fabricate();
        BaseObject* test10_06 = test10_03->fabricate();

        BaseObject* test10_07;
        for (int i = 0; i < 10; i++)
            test10_07 = ID("A1B1C1")->fabricate();

        std::cout << "1\n";
        int count;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "2\n";
        BaseObject* test10_08;
        BaseObject* test10_09 = 0;
        BaseObject* test10_10 = 0;
        for (int i = 0; i < 10; i++)
        {
            test10_08 = ID("A2B1C1")->fabricate();
            std::string objName = "A2B1C1#";
            objName += '0' + i;
            test10_08->setName(objName);

            if (i == 0)
                test10_09 = test10_08;

            if (i == 5)
                test10_10 = test10_08;
        }

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "3\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "4\n";
        delete test10_08;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "5\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "6\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "7\n";
        delete test10_09;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::end(); it; --it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::end(); it; --it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::end(); it; --it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::end(); it; --it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::end(); it; --it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "8\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "9\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "10\n";
        delete test10_10;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "11\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "12\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->getName() << "\n";

        std::cout << "13\n";
*/
/*
        std::cout << "Test 11\n";
        std::cout << "1\n";
        int count2 = 0; for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it) { count2++; }
        std::cout << "AnzahlTickable: " << count2 << "\n";

        Test1* test11_1;
        for (int i = 0; i < 3; i++)
            test11_1 = new Test1;

        count2 = 0; for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it) { count2++; }
        std::cout << "AnzahlTickable: " << count2 << "\n";

        for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
            it->tick(0);

        std::cout << "2\n";
        Test2* test11_2 = new Test2;


        std::cout << "3\n";
        Test3* test11_3 = new Test3;
        test11_3->configOutput();

        std::cout << "4\n";
*/
/*
        std::cout << "Test 12\n";
        std::cout << "1\n";

        WorldEntity* test12_1 = new WorldEntity;

        std::cout << sizeof(WorldEntity) << std::endl;

        std::cout << "2\n";
*/
/*
        std::cout << "Test 13\n";

        #define BoolToYesNo(bool) \
            if (bool) \
                std::cout << "yes "; \
            else \
                std::cout << "no  "

        #define TestClassTreeMask(mask) \
            std::cout << "========== ClassTreeMask-Test ===========" << std::endl; \
            std::cout << mask << std::endl << std::endl; \
            std::cout << "                 "; \
            BoolToYesNo(mask.isIncluded(Class(BaseObject))); \
            std::cout << std::endl; \
            \
            std::cout << "     "; \
            BoolToYesNo(mask.isIncluded(Class(A1))); \
            std::cout << "        "; \
            BoolToYesNo(mask.isIncluded(Class(A2))); \
            std::cout << "           "; \
            BoolToYesNo(mask.isIncluded(Class(A3))); \
            std::cout << std::endl; \
            \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A1B1))); \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A1B2))); \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A2B1))); \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A2B2))); \
            std::cout << "    "; \
            BoolToYesNo(mask.isIncluded(Class(A3B1))); \
            std::cout << "    "; \
            BoolToYesNo(mask.isIncluded(Class(A3B2))); \
            std::cout << std::endl; \
            \
            BoolToYesNo(mask.isIncluded(Class(A1B1C1))); \
            BoolToYesNo(mask.isIncluded(Class(A1B1C2))); \
            BoolToYesNo(mask.isIncluded(Class(A1B2C1))); \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A2B1C1))); \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A2B2C1))); \
            std::cout << "  "; \
            BoolToYesNo(mask.isIncluded(Class(A3B1C1))); \
            BoolToYesNo(mask.isIncluded(Class(A3B1C2))); \
            BoolToYesNo(mask.isIncluded(Class(A3B2C1))); \
            BoolToYesNo(mask.isIncluded(Class(A3B2C2))); \
            std::cout << std::endl; \
            std::cout << "=========================================" << std::endl

        std::cout << "1\n";

        ClassTreeMask test13_1;
        test13_1.exclude(Class(A1B1));
        test13_1.exclude(Class(A2));
        test13_1.include(Class(A2B2));
        test13_1.exclude(Class(A2B2C1));
        test13_1.exclude(Class(A3B1));
        test13_1.include(Class(A3B1C2));
        test13_1.exclude(Class(A3B2C1));
        test13_1.exclude(Class(A3B2C2));
        std::cout << "Mask 1:\n";
        TestClassTreeMask(test13_1);

        ClassTreeMask test13_2;
        test13_2.exclude(Class(BaseObject));
        test13_2.include(Class(A1));
        test13_2.exclude(Class(A1B2));
        test13_2.exclude(Class(A1B1C2));
        test13_2.include(Class(A2));
        test13_2.exclude(Class(A2B2));
        test13_2.include(Class(A3B1));
        test13_2.include(Class(A3B2));
        test13_2.exclude(Class(A3B2C2));
        std::cout << std::endl;
        std::cout << "Mask 2:\n";
        TestClassTreeMask(test13_2);

        std::cout << "2\n";

        ClassTreeMask test13_3;
        test13_3.include(Class(A1), true, false);
        test13_3.exclude(Class(A1B2), true, false);
        test13_3.exclude(Class(A1B1C2), true, false);
        test13_3.include(Class(A2), true, false);
        test13_3.exclude(Class(A2B2), true, false);
        test13_3.include(Class(A3B1), true, false);
        test13_3.include(Class(A3B2), true, false);
        test13_3.exclude(Class(A3B2C2), true, false);
        std::cout << std::endl;
        std::cout << "Mask 2 without excluded BaseObject:\n";
        TestClassTreeMask(test13_3);
        test13_3.exclude(Class(BaseObject), false, false);
        std::cout << std::endl;
        std::cout << "Mask 2 with BaseObject excluded afterwards without overwrite:\n";
        TestClassTreeMask(test13_3);
        test13_3.exclude(Class(BaseObject), true);
        std::cout << std::endl;
        std::cout << "Mask 2 with BaseObject excluded afterwards with overwrite:\n";
        TestClassTreeMask(test13_3);

        ClassTreeMask test13_4;
        test13_4.include(Class(A3B2));
        test13_4.exclude(Class(A1B1C2));
        test13_4.include(Class(A3B1));
        test13_4.exclude(Class(A3B2C2));
        test13_4.exclude(Class(BaseObject));
        test13_4.include(Class(A2));
        test13_4.exclude(Class(A1B2));
        test13_4.exclude(Class(A2B2));
        test13_4.include(Class(A1));
        std::cout << std::endl;
        std::cout << "Mask 2 created in random order with overwrite and clean:\n";
        TestClassTreeMask(test13_4);

        ClassTreeMask test13_4_2;
        test13_4_2.include(Class(A3B2), false, false);
        test13_4_2.exclude(Class(A1B1C2), false, false);
        test13_4_2.include(Class(A3B1), false, false);
        test13_4_2.exclude(Class(A3B2C2), false, false);
        test13_4_2.exclude(Class(BaseObject), false, false);
        test13_4_2.include(Class(A2), false, false);
        test13_4_2.exclude(Class(A1B2), false, false);
        test13_4_2.exclude(Class(A2B2), false, false);
        test13_4_2.include(Class(A1), false, false);
        std::cout << std::endl;
        std::cout << "Mask 2 created in random order without overwrite and without clean:\n";
        TestClassTreeMask(test13_4_2);

        std::cout << "3\n";

        ClassTreeMask test13_6 = !test13_1;
        std::cout << std::endl;
        std::cout << "Mask 1 inverted:\n";
        TestClassTreeMask(test13_6);

        std::cout << "4\n";

        ClassTreeMask test13_7 = test13_1 + test13_2;
        std::cout << std::endl;
        std::cout << "Mask 1 + Mask 2:\n";
        TestClassTreeMask(test13_7);

        std::cout << "5\n";

        ClassTreeMask test13_8 = test13_1 * test13_2;
        std::cout << std::endl;
        std::cout << "Mask 1 * Mask 2:\n";
        TestClassTreeMask(test13_8);

        std::cout << "6\n";

        ClassTreeMask test13_9 = test13_1 - test13_2;
        std::cout << std::endl;
        std::cout << "Mask 1 - Mask 2:\n";
        TestClassTreeMask(test13_9);

        std::cout << "7\n";

        ClassTreeMask test13_10 = test13_1 ^ test13_2;
        std::cout << std::endl;
        std::cout << "Mask 1 ^ Mask 2:\n";
        TestClassTreeMask(test13_10);

        std::cout << "8\n";

        ClassTreeMask test13_12(!test13_1);
        std::cout << std::endl;
        std::cout << "Mask 1 inverted assigned with copyconstructor + the original from before:\n";
        TestClassTreeMask(test13_12);
        TestClassTreeMask(test13_1);


        ClassTreeMask test13_13 = test13_1 + test13_2;
        std::cout << std::endl;
        std::cout << "Mask 1 + Mask 2 assigned with copyconstructor + originals from before:\n";
        TestClassTreeMask(test13_13);
        TestClassTreeMask(test13_1);
        TestClassTreeMask(test13_2);

        ClassTreeMask test13_14 = test13_1;
        test13_14 += test13_2;
        std::cout << std::endl;
        std::cout << "Mask 1 + Mask 2 with += operator + original of mask 2 from before:\n";
        TestClassTreeMask(test13_14);
        TestClassTreeMask(test13_2);

        test13_1 = test13_1;
        std::cout << std::endl;
        std::cout << "Mask 1 assigned with = operator to itself:\n";
        TestClassTreeMask(test13_1);

        std::cout << "9\n";

        ClassTreeMask test13_15;
        test13_15.exclude(Class(Interface1));
        std::cout << std::endl;
        std::cout << "Mask with excluded Interface 1:\n";
        TestClassTreeMask(test13_15);

        ClassTreeMask test13_16 = test13_1;
        test13_16.exclude(Class(Interface1));
        std::cout << std::endl;
        std::cout << "Mask 1 with excluded Interface 1 (overwrite):\n";
        TestClassTreeMask(test13_16);

        ClassTreeMask test13_17 = test13_1;
        test13_17.exclude(Class(Interface1), false);
        std::cout << std::endl;
        std::cout << "Mask 1 with excluded Interface 1 (without overwrite):\n";
        TestClassTreeMask(test13_17);

        ClassTreeMask test13_18 = test13_2;
        test13_18.exclude(Class(Interface1), false);
        std::cout << std::endl;
        std::cout << "Mask 2 with excluded Interface 1 (without overwrite):\n";
        TestClassTreeMask(test13_18);

        std::cout << "10\n";

        ClassTreeMask test13_19;
        test13_19.exclude(Class(Test1));
        std::cout << std::endl;
        std::cout << "Mask with excluded Test1:\n";
        TestClassTreeMask(test13_19);

        std::cout << "11\n";

        ClassTreeMask test13_20;
        test13_20.exclude(Class(DebugLevel));
        std::cout << std::endl;
        std::cout << "Mask with excluded DebugLevel:\n";
        TestClassTreeMask(test13_20);

        std::cout << "12\n";

        ClassTreeMask test13_21;
        test13_21.excludeSingle(Class(A2));
        std::cout << std::endl;
        std::cout << "Mask with single-excluded A2:\n";
        TestClassTreeMask(test13_21);

        test13_21.exclude(Class(A2));
        std::cout << std::endl;
        std::cout << "Mask with excluded A2:\n";
        TestClassTreeMask(test13_21);

        test13_21.includeSingle(Class(A2));
        std::cout << std::endl;
        std::cout << "Previous mask with single-included A2:\n";
        TestClassTreeMask(test13_21);

        test13_21.includeSingle(Class(A2), false);
        std::cout << std::endl;
        std::cout << "Previous mask with single-included A2 without clean:\n";
        TestClassTreeMask(test13_21);

        std::cout << "13\n";
*/
/*
        std::cout << "Test 14\n";
        std::cout << "1\n";

        SubclassIdentifier<A1> test14_1;
        test14_1 = Class(A1B1C1);
        std::cout << test14_1.getIdentifier()->getName() << std::endl;

        SubclassIdentifier<A1> test14_2 = Class(A1B1C2);
        std::cout << test14_2.getIdentifier()->getName() << std::endl;

        SubclassIdentifier<Interface1> test14_3;
        test14_3 = Class(A2B2C1);
        std::cout << test14_3.getIdentifier()->getName() << std::endl;

        SubclassIdentifier<A1B2> test14_4;
        test14_4 = Class(A1B2C1);
        std::cout << test14_4.getIdentifier()->getName() << std::endl;

        SubclassIdentifier<BaseObject> test14_5 = Class(Test1);
        std::cout << test14_5.getIdentifier()->getName() << std::endl;

        std::cout << "2\n";
*/

        std::cout << "Test 15\n";
        std::cout << "1\n";

        Level* test15_1 = new Level("levels/sample.oxw");
        Loader::open(test15_1);

        std::cout << "2\n";

//    startRenderLoop();
  }

  /**
   * @return singleton object
   */
  Orxonox* Orxonox::getSingleton()
  {
    if (!singletonRef_)
      singletonRef_ = new Orxonox();
    return singletonRef_;
  }

  /**
   * error kills orxonox
   */
  void Orxonox::die(/* some error code */)
  {
    //TODO: destroy and destruct everything and print nice error msg
    delete this;
  }

  void Orxonox::standaloneInit(std::string path)
  {
    ogre_->setConfigPath(path);
    ogre_->setup();
    root_ = ogre_->getRoot();
    if(!ogre_->load()) die(/* unable to load */);

    //defineResources();
    //setupRenderSystem();
    //createRenderWindow();
    //initializeResourceGroups();
    /*createScene();
    setupScene();
    setupInputSystem();
    createFrameListener();
    Factory::createClassHierarchy();
    startRenderLoop();*/
  }

  void Orxonox::playableServer(std::string path)
  {
    ogre_->setConfigPath(path);
    ogre_->setup();
    root_ = ogre_->getRoot();
    defineResources();
    setupRenderSystem();
    createRenderWindow();
    initializeResourceGroups();
    setupInputSystem();
    Factory::createClassHierarchy();
    createScene();
    setupScene();
    createFrameListener();
    try{
//      server_g = new network::Server(); //!< add port and bindadress
//      server_g->open(); //!< open server and create listener thread
//      if(ogre_ && ogre_->getRoot())
//        ogre_->getRoot()->addFrameListener(new network::ServerFrameListener()); // adds a framelistener for the server
      COUT(3) << "Info: network framelistener added" << std::endl;
    }
    catch(...)
    {
      COUT(1) << "Error: There was a problem initialising the server :(" << std::endl;
    }
    startRenderLoop();
  }

  void Orxonox::standalone(){



  }

  void Orxonox::serverInit(std::string path)
  {
    COUT(2) << "initialising server" << std::endl;
    ogre_->setConfigPath(path);
    ogre_->setup();
//    server_g = new network::Server(); // FIXME add some settings if wanted
    if(!ogre_->load()) die(/* unable to load */);
    // FIXME add network framelistener
  }

  void Orxonox::clientInit(std::string path)
  {
    COUT(2) << "initialising client" << std::endl;
    ogre_->setConfigPath(path);
    ogre_->setup();
/*    if(serverIp_.compare("")==0)
      client_g = new network::Client();
    else
      client_g = new network::Client(serverIp_, 55556);*/
    if(!ogre_->load()) die(/* unable to load */);
//    ogre_->getRoot()->addFrameListener(new network::ClientFrameListener());
  }

  void Orxonox::defineResources()
  {
    std::string secName, typeName, archName;
    Ogre::ConfigFile cf;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
#else
    cf.load(dataPath_ + "resources.cfg");
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
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation( std::string(macBundlePath() + "/" + archName), typeName, secName);
#else
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
#endif
      }
    }
  }

  void Orxonox::setupRenderSystem()
  {
    if (!root_->restoreConfig() && !root_->showConfigDialog())
      throw Ogre::Exception(52, "User canceled the config dialog!", "OrxApplication::setupRenderSystem()");
  }

  void Orxonox::createRenderWindow()
  {
    root_->initialise(true, "OrxonoxV2");
  }

  void Orxonox::initializeResourceGroups()
  {
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

  /**
   *
   * @param
   */
  void Orxonox::createScene(void)
  {
	// Init audio
//    auMan_ = new audio::AudioManager();

//    bulletMgr_ = new BulletManager();

    // load this file from config
//    loader_ = new loader::LevelLoader("sample.oxw");
//    loader_->loadLevel();

//    Ogre::Overlay* hudOverlay = Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
//    HUD* orxonoxHud;
//    orxonoxHud = new HUD();
//    orxonoxHud->setEnergyValue(20);
//    orxonoxHud->setEnergyDistr(20,20,60);
//    hudOverlay->show();

	/*
    auMan_->ambientAdd("a1");
    auMan_->ambientAdd("a2");
    auMan_->ambientAdd("a3");
                                //auMan->ambientAdd("ambient1");
    auMan_->ambientStart();*/
  }


  /**
   *
   */
  void Orxonox::setupScene()
  {
//    SceneManager *mgr = ogre_->getSceneManager();


//    SceneNode* node = (SceneNode*)mgr->getRootSceneNode()->getChild("OgreHeadNode");
//     SceneNode *node = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode", Vector3(0,0,0));


/*
    particle::ParticleInterface *e = new particle::ParticleInterface(mgr,"engine","Orxonox/strahl");
    e->particleSystem_->setParameter("local_space","true");
    e->setPositionOfEmitter(0, Vector3(0,-10,0));
    e->setDirection(Vector3(0,0,-1));
    e->addToSceneNode(node);
*/
  }


  void Orxonox::setupInputSystem()
  {
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;

    // fixes auto repeat problem
    #if defined OIS_LINUX_PLATFORM
      pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif

      Ogre::RenderWindow *win = ogre_->getRoot()->getAutoCreatedWindow();
    win->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    inputManager_ = OIS::InputManager::createInputSystem(pl);

    try
    {
      keyboard_ = static_cast<OIS::Keyboard*>(inputManager_->createInputObject(OIS::OISKeyboard, false));
      mouse_ = static_cast<OIS::Mouse*>(inputManager_->createInputObject(OIS::OISMouse, true));
    }
    catch (const OIS::Exception &e)
    {
      throw new Ogre::Exception(42, e.eText, "OrxApplication::setupInputSystem");
    }
  }

  // FIXME we actually want to do this differently...
  void Orxonox::createFrameListener()
  {
    TickFrameListener* TickFL = new TickFrameListener();
    ogre_->getRoot()->addFrameListener(TickFL);

    TimerFrameListener* TimerFL = new TimerFrameListener();
    ogre_->getRoot()->addFrameListener(TimerFL);

    //if(mode_!=CLIENT) // FIXME just a hack ------- remove this in future
      frameListener_ = new OrxListener(keyboard_/*, auMan_*/, mode_);
    ogre_->getRoot()->addFrameListener(frameListener_);
  }

  void Orxonox::startRenderLoop()
  {
    // FIXME
    // this is a hack!!!
    // the call to reset the mouse clipping size should probably be somewhere
    // else, however this works for the moment.
    unsigned int width, height, depth;
    int left, top;
    ogre_->getRoot()->getAutoCreatedWindow()->getMetrics(width, height, depth, left, top);

    if(mode_!=CLIENT){
      const OIS::MouseState &ms = mouse_->getMouseState();
      ms.width = width;
      ms.height = height;
    }
    ogre_->getRoot()->startRendering();
  }
}
