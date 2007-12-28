/**
 @file  Orxonox.h
 @brief Main Orxonox Class File
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Orxonox_H__
#define _Orxonox_H__

#include <OgrePrerequisites.h>
#include <OIS/OISPrereqs.h>

#include "OrxonoxPrereqs.h"
#include "loader/LoaderPrereqs.h"

#include "misc/String.h"
#include "GraphicsEngine.h"


// TODO: Orxonox should maybe derive from BaseObject
//! Orxonox singleton class
namespace orxonox {

  enum gameMode{
    STANDALONE,
    SERVER,
    CLIENT,
    PRESENTATION
  };

  class Orxonox
  {
    public:
      void init(int argc, char **argv, String path);
      void start();
      // not sure if this should be private
      void die(/* some error code */);
      static Orxonox* getSingleton();
      inline Ogre::SceneManager* getSceneManager()         { return ogre_->getSceneManager(); };
      inline GraphicsEngine* getOgrePointer()              { return ogre_; };
      inline audio::AudioManager* getAudioManagerPointer() { return auMan_; };
      inline OIS::Keyboard* getKeyboard()                  { return this->keyboard_; }
      inline OIS::Mouse* getMouse()                        { return this->mouse_; }
      inline BulletManager* getBulletMgr()                 { return this->bulletMgr_; }

    private:
      Orxonox();
      virtual ~Orxonox();
      // init functions
      void serverInit(String path);
      void clientInit(String path);
      void standaloneInit(String path);
      // run functions
      void playableServer(String path);
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
      String           dataPath_;  //!< path to data
      loader::LevelLoader*  loader_;    //!< level loader builds the scene
      audio::AudioManager*  auMan_;     //!< audio manager
      BulletManager*        bulletMgr_; //!< Keeps track of the thrown bullets
      static Orxonox*       singletonRef_;
      OIS::Keyboard*        keyboard_;
      OIS::Mouse*           mouse_;
      OIS::InputManager*    inputManager_;
      OrxListener*          frameListener_;
      Ogre::Root*           root_;

      // this is used to identify the mode (server/client/...) we're in
      gameMode              mode_;
      String           serverIp_;
  };
}
#endif /* _Orxonox_H__ */
