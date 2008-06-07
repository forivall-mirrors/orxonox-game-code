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
 *      Reto Grieder
 *   Co-authors:
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
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

namespace orxonox {

  enum gameMode{
    SERVER,
    CLIENT,
    STANDALONE,
    DEDICATED
  };

  //! Orxonox singleton class
  class _OrxonoxExport Orxonox
  {
    public:
      bool init(int argc, char **argv);
      bool start();

      void abortRequest();
      //inline audio::AudioManager* getAudioManagerPointer() { return auMan_; };

      static Orxonox* getSingleton();
      static void destroySingleton();

      static inline void slomo(float factor) { Orxonox::setTimeFactor(factor); }
      static void setTimeFactor(float factor = 1.0);
      static inline float getTimeFactor() { return Orxonox::getSingleton()->timefactor_; }
      static inline void exit() { Orxonox::getSingleton()->abortRequest(); }

   private:
      // don't mess with singletons
      Orxonox();
      Orxonox(Orxonox& instance);
      Orxonox& operator=(const Orxonox& instance);
      ~Orxonox();

      bool loadPlayground();

      bool serverLoad();
      bool clientLoad();
      bool standaloneLoad();

      bool loadScene();

      bool startRenderLoop();

      float calculateEventTime(unsigned long, std::deque<unsigned long>&);

    private:
      GraphicsEngine*       ogre_;          //!< our dearest graphics engine <3
      //audio::AudioManager*  auMan_;         //!< audio manager
      Ogre::Timer*          timer_;         //!< Main loop timer
      // TODO: make this a config-value by creating a config class for orxonox
      float                 frameSmoothingTime_;
      HUD*                  orxonoxHUD_;
      bool                  bAbort_;        //!< aborts the render loop if true
      float                 timefactor_;    //!< A factor to change the gamespeed

      // this is used to identify the mode (server/client/...) we're in
      gameMode              mode_;
      std::string           serverIp_;
      int                   serverPort_;

      static Orxonox *singletonRef_s;
  };
}
#endif /* _Orxonox_H__ */
