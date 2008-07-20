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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
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

#include "core/OrxonoxClass.h"
#include "audio/AudioPrereqs.h"
#include "GraphicsEngine.h"
#include "Settings.h"

namespace orxonox
{
  //! Orxonox singleton class
    class _OrxonoxExport Orxonox : public OrxonoxClass
  {
    public:
      Orxonox();
      ~Orxonox();

      void start();
      void setConfigValues();

      void abortRequest();

      static Orxonox& getSingleton();

      static void slomo(float factor) { Orxonox::setTimeFactor(factor); }
      static float getTimeFactor()    { return Orxonox::getSingleton().timefactor_; }
      static void loadGame(const std::string& name);
      static void exit()              { Orxonox::getSingleton().abortRequest(); }
      static void setTimeFactor(float factor = 1.0);

   private:
      // don't mess with singletons
      Orxonox(Orxonox& instance);

      bool loadLevel(const GameMode& mode);
      bool serverLoad();
      bool clientLoad();
      bool standaloneLoad();

      bool loadPlayground();
      bool loadScene();
      void startRenderLoop();

    private:
      GraphicsEngine*       ogre_;          //!< our dearest graphics engine <3
      Level*                startLevel_;    //!< current hard coded default level
      Level*                hud_;           //!< 'level' object fo the HUD
      Radar*                radar_;         //!< represents the Radar (not the HUD part)
      //audio::AudioManager*  auMan_;         //!< audio manager
      Ogre::Timer*          timer_;         //!< Main loop timer
      bool                  bAbort_;        //!< aborts the render loop if true
      float                 timefactor_;    //!< A factor to change the gamespeed

      // this is used to identify the mode (server/client/...) we're in
      GameMode              mode_;
      GameMode              modeRequest_;

      // config values
      float                 debugRefreshTime_;

      static Orxonox *singletonRef_s;
  };
}
#endif /* _Orxonox_H__ */
