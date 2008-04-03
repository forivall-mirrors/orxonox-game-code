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
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>

//****** OIS *******
#include <OIS/OIS.h>

//****** STD *******
#include <iostream>
#include <exception>
#include <string.h>

//***** ORXONOX ****
//misc
#include "util/Sleep.h"

// loader and audio
#include "loader/LevelLoader.h"
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"
#include "network/NetworkFrameListener.h"

// objects
#include "objects/Tickable.h"
#include "tools/Timer.h"
#include "objects/NPC.h"
#include "core/ArgReader.h"
#include "core/Factory.h"
#include "core/Debug.h"
#include "hud/HUD.h"
#include "hud/Bar.h"
#include "objects/weapon/BulletManager.h"
#include "GraphicsEngine.h"

#include "Orxonox.h"

namespace orxonox
{
   // put this in a seperate Class or solve the problem in another fashion
  class OrxListener : public Ogre::FrameListener
  {
    public:
      OrxListener(OIS::Keyboard *keyboard, audio::AudioManager*  auMan, gameMode mode)
      {
        mKeyboard = keyboard;
        mode_=mode;
        auMan_ = auMan;
      }

      bool frameStarted(const Ogre::FrameEvent& evt)
      {
        auMan_->update();
        updateAI();

        if(mode_ == PRESENTATION)
          server_g->tick(evt.timeSinceLastFrame);
        else if(mode_ == CLIENT)
          client_g->tick(evt.timeSinceLastFrame);

        usleep(10);

        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
      }

      void updateAI()
      {
        for(Iterator<NPC> it = ObjectList<NPC>::start(); it; ++it)
        {
          it->update();
        }
      }

    private:
      gameMode mode_;
      OIS::Keyboard *mKeyboard;
      audio::AudioManager*  auMan_;
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
    this->loader_ = 0;
    this->auMan_ = 0;
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
    ogre_->startRender();
    //TODO: run engine
    Factory::createClassHierarchy();
    createScene();
    setupScene();
    setupInputSystem();
    if(mode_!=CLIENT){ // remove this in future ---- presentation hack
    }
    else
      std::cout << "client here" << std::endl;
    createFrameListener();
    switch(mode_){
    case PRESENTATION:
      //ogre_->getRoot()->addFrameListener(new network::ServerFrameListener());
      //std::cout << "could not add framelistener" << std::endl;
      server_g->open();
      break;
    case CLIENT:
      client_g->establishConnection();
      break;
    case SERVER:
    case STANDALONE:
    default:
      break;
    }
    startRenderLoop();
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
      server_g = new network::Server(); //!< add port and bindadress
      server_g->open(); //!< open server and create listener thread
      if(ogre_ && ogre_->getRoot())
        ogre_->getRoot()->addFrameListener(new network::ServerFrameListener()); // adds a framelistener for the server
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
    server_g = new network::Server(); // FIXME add some settings if wanted
    if(!ogre_->load()) die(/* unable to load */);
    // FIXME add network framelistener
  }

  void Orxonox::clientInit(std::string path)
  {
    COUT(2) << "initialising client" << std::endl;
    ogre_->setConfigPath(path);
    ogre_->setup();
    if(serverIp_.compare("")==0)
      client_g = new network::Client();
    else
      client_g = new network::Client(serverIp_, 55556);
    if(!ogre_->load()) die(/* unable to load */);
    ogre_->getRoot()->addFrameListener(new network::ClientFrameListener());
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
    auMan_ = new audio::AudioManager();

    bulletMgr_ = new BulletManager();

    // load this file from config
    loader_ = new loader::LevelLoader("sample.oxw");
    loader_->loadLevel();

//    Ogre::Overlay* hudOverlay = Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
 //   Ogre::OverlayContainer* cont = Ogre::OverlayManager::createOverlayElement("Orxonox/HUD1.2/MyShip", "Orxonox/HUD1.2/shieldLeftTop", false);
 //   Ogre::OverlayContainer* cont =  Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2/MapBackGround");

//    hud::Bar* newBar = Ogre::OverlayManager::createOverlayElement("Element", "ElementName");
//new hud::Bar(1,true,Ogre::ColourValue::Black,"Orxonox/hi");
 //   cont->addChild(newBar-> element_);
//   hud::Bar* newBar = Ogre::OverlayManager::createOverlayElement("orxonox/bar", "orxonox/newbar", false);
  

//    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

//    Ogre::Overlay* hudOverlay = overlayManager.create("orxonoxsuperoverlay"); 

/*    Bar* newBar = static_cast<Bar*>(overlayManager.createOverlayElement("Panel", "Bar"));
    newBar->setLeft(0);
    newBar->setTop(0);
    newBar->setWidth(100);
    newBar->setHeight(10);
    newBar->setMaterialName("Orxonox/Red");
    newBar->setMetricsMode(Ogre::GMM_PIXELS);	
    newBar->setPercentage(0.5);
    newBar->show();
   
*/
/*
    Bar* newBar;
    newBar = new Bar(0,0,100,10,Bar::LEFT, Bar::RED,"hallo");
    newBar->reset(50);

    Bar* newBar2;
    newBar2 = new Bar(0,20,100,10,Bar::UP,Bar::GREEN,"hallo2");
    newBar2->reset(50);

    Bar* newBar3;
    newBar3 = new Bar(0,40,100,10,Bar::RIGHT,Bar::RED,"hallo3");
    newBar3->reset(50);

    Bar* newBar4;
    newBar4 = new Bar(0,60,100,10,Bar::DOWN,Bar::YELLOW,"hallo4");
    newBar4->reset(50);
    newBar4->hide();
    newBar4->show();

    SmartBar* newBar5;
    newBar5 = new SmartBar(0,80,100,10,Bar::DOWN,"hallo5");

    SmartBar* newBar6;
    newBar6 = new SmartBar(50,80,100,10,Bar::DOWN,"hallo6");

    newBar5->reset(56);

   
*/
/*
    Ogre::OverlayElement* element = overlayManager.createOverlayElement("Panel",s);
    element->setLeft(0);
    element->setTop(0);
    element->setWidth(100);
    element->setHeight(10);
    element->setMaterialName("Orxonox/Red");
    element->setMetricsMode(Ogre::GMM_PIXELS);
*/
/*    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*> (overlayManager.createOverlayElement("Panel", "PanelName"));
    panel->setLeft(10);
    panel->setTop(10);
    panel->setWidth(100);
    panel->setHeight(100);
    panel->setMetricsMode(Ogre::GMM_PIXELS);	
    panel->show();


    if(dynamic_cast<Ogre::TextAreaOverlayElement*> (overlayManager.createOverlayElement("TextArea", "hallo7"))!= NULL ){panel->addChild(newBar5->element);}

    Ogre::TextAreaOverlayElement* newEle = dynamic_cast<Ogre::TextAreaOverlayElement*> (overlayManager.createOverlayElement("TextArea", "hallo8"));
    newEle->setColour(Ogre::ColourValue::Blue);
    newEle->setCaption("hallo");
    newEle->setPosition(0,0);
    newEle->setDimensions(0,0);
    newEle->setHorizontalAlignment(Ogre::GHA_LEFT);
//    newEle->setMaterialName("Orxonox/Red");
    newEle->setCharHeight(18);
    newEle->setFontName("BlueHighway");
//    newEle->setMetricsMode(Ogre::GMM_PIXELS);	


    Ogre::OverlayElement* newEle2 =Ogre:: OverlayManager::getSingleton().getOverlayElement("Orxonox/HUD1.2/RocketNum1");
    newEle2->setColour(Ogre::ColourValue::Red);
    newEle2->setCaption("hallo");
    newEle2->setPosition(-50,0);
    newEle2->setDimensions(0,0);
//    newEle->setMaterialName("Orxonox/Red");
//    newEle2->setCharHeight(18);
//    newEle2->setFontName("BlueHighway");
//    newEle2->setMetricsMode(Ogre::GMM_PIXELS);


    panel->addChild(newEle2);

*/

   
//    hudOverlay->add2D(panel);
/*    panel->addChild(newBar->element);
    panel->addChild(newBar2->element);
    panel->addChild(newBar3->element);
    panel->addChild(newBar4->element);
    panel->addChild(newBar5->element);
    panel->addChild(newBar6->element);
*///    panel->addChild(newEle);

    

//    HUD2* orxonoxHud;
//    orxonoxHud = new HUD2();
//    orxonoxHud->setEnergyValue(20);
//    orxonoxHud->setEnergyDistr(20,20,60);
//    hudOverlay->show();

    HUD* orxonoxHUD = new HUD(1);


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
      frameListener_ = new OrxListener(keyboard_, auMan_, mode_);
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
