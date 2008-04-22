/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  Orxonox.h
 @brief Main Orxonox Class File
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _Orxonox_H__
#define _Orxonox_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include <OgrePrerequisites.h>
#include "audio/AudioPrereqs.h"

#include "GraphicsEngine.h"
#include "core/InputEventListener.h"


namespace orxonox {

  enum gameMode{
    SERVER,
    CLIENT,
    STANDALONE
  };

  //! Orxonox singleton class
  class _OrxonoxExport Orxonox : public InputEventListener
  {
    public:
      void init(int argc, char **argv, std::string path);
      void start();
      // not sure if this should be private
      void abortImmediate(/* some error code */);
      void abortRequest();
      inline audio::AudioManager* getAudioManagerPointer() { return auMan_; };

      static Orxonox* getSingleton();
      static void destroySingleton();

      static inline void slomo(float factor) { Orxonox::getSingleton()->timefactor_ = factor; }
      static inline void setTimeFactor(float factor = 1.0) { Orxonox::getSingleton()->timefactor_ = factor; }
      static inline float getTimeFactor() { return Orxonox::getSingleton()->timefactor_; }
      static inline void exit() { Orxonox::getSingleton()->abortRequest(); }

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
      InputManager*         inputHandler_;  //!< Handles input with key bindings
      Ogre::Timer*          timer_;         //!< Main loop timer
      // TODO: make this a config-value by creating a config class for orxonox
      float                 frameSmoothingTime_;
      HUD*                  orxonoxHUD_;
      InGameConsole*        orxonoxConsole_;
      bool                  bAbort_;        //!< aborts the render loop if true
      float                 timefactor_;    //!< A factor to change the gamespeed

      // this is used to identify the mode (server/client/...) we're in
      gameMode              mode_;
      std::string           serverIp_;

      static Orxonox *singletonRef_s;
  };
}
#endif /* _Orxonox_H__ */
