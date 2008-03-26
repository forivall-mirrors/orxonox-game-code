/**
 @file  Orxonox.h
 @brief Main Orxonox Class File
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Orxonox_H__
#define _Orxonox_H__

#include <string>

#include <OgrePrerequisites.h>
//#include <OIS/OISPrereqs.h>

#include "OrxonoxPrereqs.h"
#include "audio/AudioPrereqs.h"

#include "GraphicsEngine.h"
#include "InputEventListener.h"


// TODO: Orxonox should maybe derive from BaseObject
//! Orxonox singleton class
namespace orxonox {

  enum gameMode{
    SERVER,
    CLIENT,
    STANDALONE
  };

  class _OrxonoxExport Orxonox : public InputEventListener
  {
    public:
      void init(int argc, char **argv, std::string path);
      void start();
      // not sure if this should be private
      void abortImmediate(/* some error code */);
      void abortRequest();
      inline Ogre::SceneManager*  getSceneManager()        { return ogre_->getSceneManager(); };
      inline GraphicsEngine*      getOgrePointer()         { return ogre_; };
      inline audio::AudioManager* getAudioManagerPointer() { return auMan_; };
      inline BulletManager*       getBulletMgr()           { return this->bulletMgr_; }

      static Orxonox* getSingleton();

   private:
      // don't mess with singletons
      Orxonox();
      Orxonox(Orxonox& instance);
      Orxonox& operator=(const Orxonox& instance);
      ~Orxonox();

      // init functions
      void serverInit(std::string path);
      void clientInit(std::string path);
      void standaloneInit(std::string path);

      // run functions
      void serverStart();
      void clientStart();
      void standaloneStart();

      void createScene();
      void setupInputSystem();
      void startRenderLoop();
      float calculateEventTime(unsigned long, std::deque<unsigned long>&);

      void eventOccured(InputEvent &evt);

    private:
      GraphicsEngine*       ogre_;          //!< our dearest graphics engine <3
      std::string           dataPath_;      //!< path to data
      audio::AudioManager*  auMan_;         //!< audio manager
      BulletManager*        bulletMgr_;     //!< Keeps track of the thrown bullets
      InputHandler*         inputHandler_;  //!< Handles input with key bindings
      Ogre::Root*           root_;          //!< Holy grail of Ogre
      Ogre::Timer*          timer_;         //!< Main loop timer
      // TODO: make this a config-value by creating a config class for orxonox
      float                 frameSmoothingTime_;
      // little hack to actually show something dynamic in the HUD
      HUD*                  orxonoxHUD_;
      bool                  bAbort_;        //!< aborts the render loop if true

      // this is used to identify the mode (server/client/...) we're in
      gameMode              mode_;
      std::string           serverIp_;
  };
}
#endif /* _Orxonox_H__ */
