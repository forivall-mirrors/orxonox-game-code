/**
 @file  Orxonox.h
 @brief Main Orxonox Class File
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Orxonox_H__
#define _Orxonox_H__

#include <string>
#include <deque>

#include <OgrePrerequisites.h>
#include <OIS/OISPrereqs.h>

#include "OrxonoxPrereqs.h"
#include "audio/AudioPrereqs.h"

#include "GraphicsEngine.h"


// TODO: Orxonox should maybe derive from BaseObject
//! Orxonox singleton class
namespace orxonox {

  enum gameMode{
    SERVER,
    CLIENT,
    STANDALONE
  };

  class _OrxonoxExport Orxonox
  {
    public:
      void init(int argc, char **argv, std::string path);
      void start();
      // not sure if this should be private
      void die(/* some error code */);
      void abortRequest();
      static Orxonox* getSingleton();
      inline Ogre::SceneManager* getSceneManager()         { return ogre_->getSceneManager(); };
      inline GraphicsEngine* getOgrePointer()              { return ogre_; };
      inline audio::AudioManager* getAudioManagerPointer() { return auMan_; };
      //inline OIS::Keyboard* getKeyboard()                  { return this->keyboard_; }
      //inline OIS::Mouse* getMouse()                        { return this->mouse_; }
      inline BulletManager* getBulletMgr()                 { return this->bulletMgr_; }

    private:
      Orxonox();
      virtual ~Orxonox();
      // init functions
      void serverInit(std::string path);
      void clientInit(std::string path);
      void standaloneInit(std::string path);
      // run functions
      void serverStart();
      void clientStart();
      void standaloneStart();
      void defineResources();
      void setupRenderSystem();
      void createRenderWindow();
      void initializeResourceGroups();
      void createScene(void);
      void setupScene();
      void setupInputSystem();
      void startRenderLoop();
      void updateTimers(float);
      float calculateEventTime(unsigned long, std::deque<unsigned long>&);

    private:
      GraphicsEngine*       ogre_;          //!< our dearest graphics engine <3
      std::string           dataPath_;      //!< path to data
//      loader::LevelLoader*  loader_;        //!< level loader builds the scene
      audio::AudioManager*  auMan_;         //!< audio manager
      BulletManager*        bulletMgr_;     //!< Keeps track of the thrown bullets
      static Orxonox*       singletonRef_;
      InputHandler*         inputHandler_;
      //OIS::Keyboard*        keyboard_;
      //OIS::Mouse*           mouse_;
      //OIS::InputManager*    inputManager_;
      OrxListener*          frameListener_;
      Ogre::Root*           root_;
      // TODO: make this a config-value by creating a config class for orxonox
      float                 frameSmoothingTime_;
      // little hack to actually show something dynamic in the HUD
      HUD*                  orxonoxHUD_;
      bool                  bAbort_;

      // this is used to identify the mode (server/client/...) we're in
      gameMode              mode_;
      std::string           serverIp_;
  };
}
#endif /* _Orxonox_H__ */
