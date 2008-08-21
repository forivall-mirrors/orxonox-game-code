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

#include "util/SubString.h"
#include "core/Factory.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"
#include "core/Debug.h"
#include "core/Exception.h"
#include "core/TclBind.h"
#include "core/Core.h"
#include "core/CommandLine.h"
#include "GraphicsEngine.h"
#include "Settings.h"

namespace orxonox
{
    SetCommandLineArgument(dataPath, "").setInformation("PATH");

    GSRoot::GSRoot()
        : GameState("root")
        , settings_(0)
        , graphicsEngine_(0)
    {
    }

    GSRoot::~GSRoot()
    {
    }

    //SetCommandLineArgument(asdf1, "haha").setShortcut("a").setUsageInformation("1|2|3");
    //SetCommandLineArgument(asdf2, 3).setShortcut("b");
    //SetCommandLineArgument(asdf3, Vector2()).setShortcut("c");
    //SetCommandLineArgument(adsf4, 1.4f).setShortcut("d");
    //SetCommandLineSwitch(showGraphics).setShortcut("g");

    void GSRoot::feedCommandLine(int argc, char** argv)
    {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i)
            args.push_back(argv[i]);

        //std::string line = "-a --asdf3 (3,3) -d -5 -b - 5.4";
        //SubString tokens(line, " ", " ", false, 92, false, 34, true, 40, 41, false, 0);

        try
        {
            orxonox::CommandLine::parse(args);
            //CommandLine::parse(tokens.getAllStrings());
        }
        catch (orxonox::ArgumentException& ex)
        {
            COUT(1) << ex.what() << std::endl;
            COUT(0) << "Usage:" << std::endl << "orxonox " << CommandLine::getUsageInformation() << std::endl;
        }
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

        // instantiate Settings class
        this->settings_ = new Settings();

        std::string dataPath;
        CommandLine::getCommandLineValue("dataPath", &dataPath);
        if (dataPath != "")
        {
            if (*dataPath.end() != '/' && *dataPath.end() != '\\')
                Settings::tsetDataPath(dataPath + "/");
            else
                Settings::tsetDataPath(dataPath);
        }

        // initialise TCL
        TclBind::getInstance().setDataPath(Settings::getDataPath());

        // initialise graphics engine. Doesn't load the render window yet!
        graphicsEngine_ = new GraphicsEngine();
        graphicsEngine_->setup();       // creates ogre root and other essentials

        // console commands
        FunctorMember<GSRoot>* functor = createFunctor(&GSRoot::loadGame);
        functor->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "loadGame"));

        requestState("gui");
    }

    void GSRoot::leave()
    {
        delete graphicsEngine_;

        delete settings_;
    }

    bool GSRoot::tick(float dt)
    {
        if (this->getActiveChild())
            this->getActiveChild()->tick(dt);
        return true;
    }

    /**
    @brief
        Requests a state.
    */
    void GSRoot::loadGame(const std::string& name)
    {
        this->requestState(name);
    }
}
