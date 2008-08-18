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

#include "OrxonoxStableHeaders.h"
#include "GSRoot.h"

#include <cassert>
#include "core/Factory.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/Debug.h"
//#include "core/Exception.h"
#include "core/TclBind.h"
#include "core/Core.h"
#include "GraphicsEngine.h"
#include "Settings.h"

namespace orxonox
{
    GSRoot::GSRoot()
        : GameState("root")
        , graphicsEngine_(0)
    {
    }

    GSRoot::~GSRoot()
    {
    }

    void GSRoot::enter()
    {
#if ORXONOX_DEBUG_MODE == 1
        ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox_d.ini");
#else
        ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
#endif

        // creates the class hierarchy for all classes with factories
        Factory::createClassHierarchy();

        // TODO: config values
        //setConfigValues();

        const Settings::CommandLineArgument* dataPath = Settings::getCommandLineArgument("dataPath");
        assert(dataPath);
        if (!dataPath->bHasDefaultValue_)
        {
            if (*dataPath->value_.getString().end() != '/' && *dataPath->value_.getString().end() != '\\')
                Settings::tsetDataPath(dataPath->value_.getString() + "/");
            else
                Settings::tsetDataPath(dataPath->value_.getString());
        }

        // initialise TCL
        TclBind::getInstance().setDataPath(Settings::getDataPath());

        graphicsEngine_ = new GraphicsEngine();
        graphicsEngine_->setup();       // creates ogre root and other essentials

    }

    void GSRoot::leave()
    {
        if (this->graphicsEngine_)
            delete graphicsEngine_;
    }

    bool GSRoot::tick(float dt)
    {
        if (this->getActiveChild())
            this->getActiveChild()->tick(dt);
        return true;
    }
}
