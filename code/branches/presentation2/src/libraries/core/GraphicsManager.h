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

#include "CorePrereqs.h"

#include <cassert>
#include <string>
#include <OgreLog.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "util/Singleton.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        Graphics engine manager class
    */
    class _CoreExport GraphicsManager : public Singleton<GraphicsManager>, public OrxonoxClass, public Ogre::LogListener
    {
        friend class Singleton<GraphicsManager>;
    public:
        GraphicsManager(bool bLoadRenderer = true);
        ~GraphicsManager();

        void setConfigValues();

        void postUpdate(const Clock& time);

        Ogre::Viewport* getViewport()         { return this->viewport_; }
        Ogre::RenderWindow* getRenderWindow() { return this->renderWindow_; }
        size_t getRenderWindowHandle();
        bool isFullScreen() const;

        void upgradeToGraphics();
        void loadDebugOverlay();
        bool rendererLoaded() const { return renderWindow_ != NULL; }

        void setCamera(Ogre::Camera* camera);

    private:
        GraphicsManager(GraphicsManager&); // don't mess with singletons

        // OGRE initialisation
        void loadOgreRoot();
        void loadOgrePlugins();
        void loadRenderer();

        // event from Ogre::LogListener
        void messageLogged(const std::string& message, Ogre::LogMessageLevel lml,
            bool maskDebug, const std::string& logName);

        // console commands
        void printScreen();

        scoped_ptr<OgreWindowEventListener> ogreWindowEventListener_; //!< Pimpl to hide OgreWindowUtilities.h
#if OGRE_VERSION < 0x010600
        scoped_ptr<MemoryArchiveFactory>    memoryArchiveFactory_;    //!< Stores the modified particle scripts
#endif
        scoped_ptr<Ogre::LogManager>        ogreLogger_;
        scoped_ptr<Ogre::Root>              ogreRoot_;                //!< Ogre's root
        Ogre::RenderWindow* renderWindow_;             //!< the one and only render window
        Ogre::Viewport*     viewport_;                 //!< default full size viewport

        // XML files for the resources and the debug overlay
        shared_ptr<XMLFile> resources_;                //!< XML with resource locations
        shared_ptr<XMLFile> extResources_;             //!< XML with resource locations in the external path (only for dev runs)
        shared_ptr<XMLFile> debugOverlay_;             //!< XML with various debug overlays

        // config values
        std::string         ogreConfigFile_;           //!< ogre config filename
        std::string         ogrePluginsDirectory_;     //!< Directory where the Ogre plugins are located
        std::string         ogrePlugins_;              //!< Comma separated list of all plugins to load
        std::string         ogreLogFile_;              //!< log filename for Ogre log messages
        int                 ogreLogLevelTrivial_;      //!< Corresponding Orxonox debug level for LL_TRIVIAL
        int                 ogreLogLevelNormal_;       //!< Corresponding Orxonox debug level for LL_NORMAL
        int                 ogreLogLevelCritical_;     //!< Corresponding Orxonox debug level for LL_CRITICAL

        // console commands
        ConsoleCommand*     ccPrintScreen_;

        static GraphicsManager* singletonPtr_s;        //!< Pointer to the Singleton
    };
}

#endif /* _GraphicsManager_H__ */
