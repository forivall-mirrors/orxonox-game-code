/**
 @file  orxonox.h
 @brief Main Orxonox Class File
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef ORXONOX_H
#define ORXONOX_H

//#include <OgreSingleton.h>
#include <OgreSceneManager.h>

#include <OIS/OIS.h>

#include "graphicsEngine.h"
#include "../loader/LevelLoader.h"
#include "../audio/AudioManager.h"
#include "spaceship_steering.h"


// TODO: Orxonox should maybe derive from BaseObject
//! Orxonox singleton class
namespace orxonox {

  enum gameMode{
    STANDALONE,
    SERVER,
    CLIENT,
    PRESENTATION
  };
  
  class OrxListener;

  class Orxonox
  {
    public:
      void init(int argc, char **argv, std::string path);
      void start();
      // not sure if this should be private
      void die(/* some error code */);
      static Orxonox* getSingleton();
      inline SceneManager* getSceneManager() { return ogre_->getSceneManager(); };
      inline GraphicsEngine* getOgrePointer() { return ogre_; };
      inline SpaceshipSteering* getSteeringPointer() { return steering_; };
      inline audio::AudioManager* getAudioManagerPointer() { return auMan_; };
    private:
      Orxonox();
      virtual ~Orxonox();
      // init functions
      void serverInit(std::string path);
      void clientInit(std::string path);
      void standaloneInit(std::string path);
      // run functions
      void playableServer(std::string path);
      void standalone();
      void defineResources();
      void setupRenderSystem();
      void createRenderWindow();
      void initializeResourceGroups();
      void createScene(void);
      void setupScene();
      void setupInputSystem();
      void createFrameListener();
      void startRenderLoop();
    private:

      GraphicsEngine*       ogre_;      //!< our dearest graphics engine <3
      std::string           dataPath_;  //!< path to data
      loader::LevelLoader*  loader_;    //!< level loader builds the scene
      audio::AudioManager*  auMan_;     //!< audio manager
      SpaceshipSteering* 	steering_;
      static Orxonox*       singletonRef_;
      OIS::Keyboard*        keyboard_;
      OIS::Mouse*           mouse_;
      OIS::InputManager*    inputManager_;
      OrxListener*          frameListener_;
      Ogre::Root*           root_;
      
      // this is used to identify the mode (server/client/...) we're in
      gameMode              mode_;
      std::string           serverIp_;
  };
}
#endif /* ORXONOX_H */
