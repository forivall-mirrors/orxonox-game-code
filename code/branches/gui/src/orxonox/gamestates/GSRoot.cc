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

//#include "util/SubString.h"
#include "core/Factory.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"
#include "core/Debug.h"
#include "core/Exception.h"
#include "core/TclBind.h"
#include "core/TclThreadManager.h"
#include "GraphicsEngine.h"
#include "Settings.h"

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32 
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include "windows.h"

   //Get around Windows hackery
#  ifdef max
#    undef max
#  endif
#  ifdef min
#    undef min
#  endif
#endif

namespace orxonox
{
    SetCommandLineArgument(dataPath, "").setInformation("PATH");

    GSRoot::GSRoot()
        : RootGameState("root")
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
        CommandLine::getValue("dataPath", &dataPath);
        if (dataPath != "")
        {
            if (*dataPath.end() != '/' && *dataPath.end() != '\\')
                Settings::tsetDataPath(dataPath + "/");
            else
                Settings::tsetDataPath(dataPath);
        }

        // initialise TCL
        TclBind::getInstance().setDataPath(Settings::getDataPath());
        TclThreadManager::getInstance();

        // initialise graphics engine. Doesn't load the render window yet!
        graphicsEngine_ = new GraphicsEngine();
        graphicsEngine_->setup();       // creates ogre root and other essentials

        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        setThreadAffinity();

        // add console commands
        FunctorMember<GSRoot>* functor1 = createFunctor(&GSRoot::exitGame);
        functor1->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor1, "exit"));

        // add console commands
        FunctorMember01<GameState, const std::string&>* functor2 = createFunctor(&GameState::requestState);
        functor2->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor2, "selectGameState"));
    }

    void GSRoot::leave()
    {
        delete graphicsEngine_;
        delete settings_;

        // TODO: remove and destroy console commands
    }

    void GSRoot::ticked(const Clock& time)
    {
        TclThreadManager::getInstance().tick(time.getDeltaTime());

        this->tickChild(time);
    }

    /**
    @note
        The code of this function has been copied from OGRE, an open source graphics engine.
            (Object-oriented Graphics Rendering Engine)
        For the latest info, see http://www.ogre3d.org/

        Copyright (c) 2000-2008 Torus Knot Software Ltd
        
        OGRE is licensed under the LGPL. For more info, see ogre license info.
    */
    void GSRoot::setThreadAffinity()
    {
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // Get the current process core mask
	    DWORD procMask;
	    DWORD sysMask;
#if _MSC_VER >= 1400 && defined (_M_X64)
	    GetProcessAffinityMask(GetCurrentProcess(), (PDWORD_PTR)&procMask, (PDWORD_PTR)&sysMask);
#else
	    GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);
#endif

	    // If procMask is 0, consider there is only one core available
	    // (using 0 as procMask will cause an infinite loop below)
	    if (procMask == 0)
		    procMask = 1;

	    // Find the lowest core that this process uses
        DWORD threadMask = 1;
	    while ((threadMask & procMask) == 0)
		    threadMask <<= 1;

	    // Set affinity to the first core
	    SetThreadAffinityMask(GetCurrentThread(), threadMask);
#endif
    }
}
