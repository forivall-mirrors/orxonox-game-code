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
 *      Felix Schulthess
 *
 */

/**
@file
@brief
    Declaration of GraphicsManager Singleton.
 */

#ifndef _GraphicsManager_H__
#define _GraphicsManager_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <cassert>
#include <OgreLog.h>

#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        Graphics engine manager class
    */
    class _OrxonoxExport GraphicsManager : public OrxonoxClass, public Ogre::LogListener
    {
    public:
        GraphicsManager();
        ~GraphicsManager();

        void setConfigValues();
        void initialise();

        void update(const Clock& time);

        void detailLevelParticleChanged();
        inline unsigned int getDetailLevelParticle() const
            { return this->detailLevelParticle_; }

        inline Ogre::Viewport* getViewport() const
            { return this->viewport_; }
        inline Ogre::RenderWindow* getRenderWindow()
            { return this->renderWindow_; }

        void setCamera(Ogre::Camera* camera);

        static GraphicsManager& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

    private:
        GraphicsManager(GraphicsManager&); // don't mess with singletons

        // OGRE initialisation
        void setupOgre();
        void loadOgrePlugins();
        void declareResources();
        void loadRenderer();
        void initialiseResources();

        // event from Ogre::LogListener
        void messageLogged(const std::string& message, Ogre::LogMessageLevel lml,
            bool maskDebug, const std::string& logName);

        // console commands
        void printScreen();

    private:
        bool                loaded_;

        Ogre::Root*         ogreRoot_;                 //!< Ogre's root
        Ogre::LogManager*   ogreLogger_;
        Ogre::RenderWindow* renderWindow_;             //!< the one and only render window
        Ogre::Viewport*     viewport_;                 //!< default full size viewport
        OgreWindowEventListener* ogreWindowEventListener_;

        // stats (Hack)
        float               avgTickTime_;              //!< time in ms to tick() one frame
        float               avgFramesPerSecond_;       //!< number of frames processed in one second

        // config values
        unsigned int        detailLevelParticle_;      //!< Detail level of particle effects (0: off, 1: low, 2: normal, 3: high)
        std::string         resourceFile_;             //!< resources file name
        std::string         ogreConfigFile_;           //!< ogre config file name
        std::string         ogrePluginsFolder_;        //!< Folder where the Ogre plugins are located
        std::string         ogrePlugins_;              //!< Comma separated list of all plugins to load
        std::string         ogreLogFile_;              //!< log file name for Ogre log messages
        int                 ogreLogLevelTrivial_;      //!< Corresponding Orxonx debug level for LL_TRIVIAL
        int                 ogreLogLevelNormal_;       //!< Corresponding Orxonx debug level for LL_NORMAL
        int                 ogreLogLevelCritical_;     //!< Corresponding Orxonx debug level for LL_CRITICAL

        // console commands
        ConsoleCommand*     ccPrintScreen_;

        static GraphicsManager* singletonRef_s;        //!< Pointer to the Singleton
    };
}

#endif /* _GraphicsManager_H__ */
