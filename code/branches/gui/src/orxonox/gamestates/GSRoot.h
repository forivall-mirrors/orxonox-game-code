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
 *      ...
 *
 */

#ifndef _GSRoot_H__
#define _GSRoot_H__

#include "OrxonoxPrereqs.h"
#include <OgreLog.h>
#include "core/RootGameState.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _OrxonoxExport GSRoot : public RootGameState, public Ogre::LogListener, public OrxonoxClass
    {
        friend class ClassIdentifier<GSRoot>;
    public:
        GSRoot();
        ~GSRoot();

        void feedCommandLine(int argc, char** argv);

        void exitGame()
        { requestState("root"); }

    private:
        void enter();
        void leave();
        void ticked(const Clock& time);

        void setConfigValues();
        void messageLogged(const std::string& message, Ogre::LogMessageLevel lml,
            bool maskDebug, const std::string& logName);
        void setThreadAffinity();
        void setupOgre();

        Settings*             settings_;
        Ogre::Root*           ogreRoot_;                  //!< Ogre's root
        Ogre::LogManager*     ogreLogger_;
        GraphicsEngine*       graphicsEngine_;   //!< Interface to Ogre

        std::string           ogreConfigFile_;        //!< ogre config file name
        std::string           ogrePluginsFile_;       //!< ogre plugins file name
        std::string           ogreLogFile_;           //!< log file name for Ogre log messages
        int                   ogreLogLevelTrivial_;   //!< Corresponding Orxonx debug level for LL_TRIVIAL
        int                   ogreLogLevelNormal_;    //!< Corresponding Orxonx debug level for LL_NORMAL
        int                   ogreLogLevelCritical_;  //!< Corresponding Orxonx debug level for LL_CRITICAL
    };
}

#endif /* _GSRoot_H__ */
