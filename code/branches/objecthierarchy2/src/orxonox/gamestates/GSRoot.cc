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

#include "util/Exception.h"
#include "util/Debug.h"
#include "core/Factory.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"
#include "core/Shell.h"
#include "core/TclBind.h"
#include "core/TclThreadManager.h"
#include "core/LuaBind.h"
#include "tools/Timer.h"
#include "objects/Tickable.h"
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
    SetCommandLineArgument(dataPath, "").information("PATH");
    SetCommandLineArgument(limitToCPU, 1).information("0: off | #cpu");

    GSRoot::GSRoot()
        : RootGameState("root")
        , settings_(0)
        , tclBind_(0)
        , tclThreadManager_(0)
        , shell_(0)
    {
        RegisterRootObject(GSRoot);
        setConfigValues();
    }

    GSRoot::~GSRoot()
    {
    }

    void GSRoot::setConfigValues()
    {
    }

    void GSRoot::enter()
    {
        // creates the class hierarchy for all classes with factories
        Factory::createClassHierarchy();

        // Create the lua interface
        this->luaBind_ = new LuaBind();

        // instantiate Settings class
        this->settings_ = new Settings();

        std::string dataPath = CommandLine::getValue("dataPath");
        if (dataPath != "")
        {
            if (*dataPath.end() != '/' && *dataPath.end() != '\\')
                Settings::tsetDataPath(dataPath + "/");
            else
                Settings::tsetDataPath(dataPath);
        }

        // initialise TCL
        this->tclBind_ = new TclBind(Settings::getDataPath());
        this->tclThreadManager_ = new TclThreadManager(tclBind_->getTclInterpreter());

        // create a shell
        this->shell_ = new Shell();

        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLine::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity((unsigned int)(limitToCPU - 1));

        // add console commands
        FunctorMember<GSRoot>* functor1 = createFunctor(&GSRoot::exitGame);
        functor1->setObject(this);
        ccExit_ = createConsoleCommand(functor1, "exit");
        CommandExecutor::addConsoleCommandShortcut(ccExit_);

        // add console commands
        FunctorMember01<GameStateBase, const std::string&>* functor2 = createFunctor(&GameStateBase::requestState);
        functor2->setObject(this);
        ccSelectGameState_ = createConsoleCommand(functor2, "selectGameState");
        CommandExecutor::addConsoleCommandShortcut(ccSelectGameState_);
    }

    void GSRoot::leave()
    {
        // destroy console commands
        delete this->ccExit_;
        delete this->ccSelectGameState_;

        delete this->shell_;
        delete this->tclThreadManager_;
        delete this->tclBind_;

        delete this->settings_;
        delete this->luaBind_;
    }

    void GSRoot::ticked(const Clock& time)
    {
        TclThreadManager::getInstance().tick(time.getDeltaTime());

        for (ObjectList<TimerBase>::iterator it = ObjectList<TimerBase>::begin(); it; ++it)
            it->tick(time);

        /*** HACK *** HACK ***/
        // Call the Tickable objects
        for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
            it->tick(time.getDeltaTime());
        /*** HACK *** HACK ***/

        this->tickChild(time);
    }

    /**
    @note
        The code of this function has been copied and adjusted from OGRE, an open source graphics engine.
            (Object-oriented Graphics Rendering Engine)
        For the latest info, see http://www.ogre3d.org/

        Copyright (c) 2000-2008 Torus Knot Software Ltd
        
        OGRE is licensed under the LGPL. For more info, see OGRE license.
    */
    void GSRoot::setThreadAffinity(unsigned int limitToCPU)
    {
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // Get the current process core mask
	    DWORD procMask;
	    DWORD sysMask;
#  if _MSC_VER >= 1400 && defined (_M_X64)
	    GetProcessAffinityMask(GetCurrentProcess(), (PDWORD_PTR)&procMask, (PDWORD_PTR)&sysMask);
#  else
	    GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);
#  endif

	    // If procMask is 0, consider there is only one core available
	    // (using 0 as procMask will cause an infinite loop below)
	    if (procMask == 0)
		    procMask = 1;

        // if the core specified with limitToCPU is not available, take the lowest one
        if (!(procMask & (1 << limitToCPU)))
            limitToCPU = 0;

	    // Find the lowest core that this process uses and limitToCPU suggests
        DWORD threadMask = 1;
	    while ((threadMask & procMask) == 0 || (threadMask < (1u << limitToCPU)))
		    threadMask <<= 1;

	    // Set affinity to the first core
	    SetThreadAffinityMask(GetCurrentThread(), threadMask);
#endif
    }
}
