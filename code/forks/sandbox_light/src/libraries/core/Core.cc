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
 *      Fabian 'x3n' Landau
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Implementation of the Core singleton with its global variables (avoids boost include)
*/

#include "Core.h"

#include <cassert>
#include <ctime>
#include <vector>

#ifdef ORXONOX_PLATFORM_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  undef min
#  undef max
#endif

#include "util/Clock.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/SignalHandler.h"
#include "PathConfig.h"
#include "CommandLineParser.h"
#include "LuaState.h"

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonPtr_s  = 0;

    SetCommandLineArgument(settingsFile, "orxonox.ini").information("THE configuration file");
#ifdef ORXONOX_PLATFORM_WINDOWS
    SetCommandLineArgument(limitToCPU, 0).information("Limits the program to one cpu/core (1, 2, 3, etc.). 0 turns it off (default)");
#endif

    /**
    @brief
        Helper class for the Core singleton: we cannot derive
        Core from OrxonoxClass because we need to handle the Identifier
        destruction in the Core destructor.
    */
    class CoreConfiguration
    {
    public:
        CoreConfiguration()
        {
        }

        void initialise()
        {
#ifdef NDEBUG
            const unsigned int defaultLevelConsole = 1;
            const unsigned int defaultLevelLogfile = 3;
            const unsigned int defaultLevelShell   = 1;
#else
            const unsigned int defaultLevelConsole = 3;
            const unsigned int defaultLevelLogfile = 4;
            const unsigned int defaultLevelShell   = 3;
#endif
            softDebugLevelConsole_ = defaultLevelConsole;
            softDebugLevelLogfile_ = defaultLevelLogfile;
            softDebugLevelShell_   = defaultLevelShell;
            this->debugLevelChanged();

            bInitializeRandomNumberGenerator_ = true;
            this->initializeRandomNumberGenerator();
        }

        /**
            @brief Callback function if the debug level has changed.
        */
        void debugLevelChanged()
        {
            // softDebugLevel_ is the maximum of the 3 variables
            this->softDebugLevel_ = this->softDebugLevelConsole_;
            if (this->softDebugLevelLogfile_ > this->softDebugLevel_)
                this->softDebugLevel_ = this->softDebugLevelLogfile_;
            if (this->softDebugLevelShell_ > this->softDebugLevel_)
                this->softDebugLevel_ = this->softDebugLevelShell_;

            OutputHandler::setSoftDebugLevel(OutputHandler::LD_All,     this->softDebugLevel_);
            OutputHandler::setSoftDebugLevel(OutputHandler::LD_Console, this->softDebugLevelConsole_);
            OutputHandler::setSoftDebugLevel(OutputHandler::LD_Logfile, this->softDebugLevelLogfile_);
            OutputHandler::setSoftDebugLevel(OutputHandler::LD_Shell,   this->softDebugLevelShell_);
        }

        void initializeRandomNumberGenerator()
        {
            static bool bInitialized = false;
            if (!bInitialized && this->bInitializeRandomNumberGenerator_)
            {
                srand(static_cast<unsigned int>(time(0)));
                rand();
                bInitialized = true;
            }
        }

        int softDebugLevel_;                            //!< The debug level
        int softDebugLevelConsole_;                     //!< The debug level for the console
        int softDebugLevelLogfile_;                     //!< The debug level for the logfile
        int softDebugLevelShell_;                       //!< The debug level for the ingame shell
        bool bInitializeRandomNumberGenerator_;         //!< If true, srand(time(0)) is called
    };


    Core::Core(const std::string& cmdLine)
        // Cleanup guard for identifier destruction (incl. XMLPort, configValues, consoleCommands)
        : configuration_(new CoreConfiguration()) // Don't yet create config values!
    {
        // Set the hard coded fixed paths
        this->pathConfig_.reset(new PathConfig());

        // Parse command line arguments AFTER the modules have been loaded (static code!)
        CommandLineParser::parseCommandLine(cmdLine);

        // Set configurable paths like log, config and media
        this->pathConfig_->setConfigurablePaths();

        // create a signal handler (only active for linux)
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_.reset(new SignalHandler());
        this->signalHandler_->doCatch(PathConfig::getExecutablePathString(), PathConfig::getLogPathString() + "orxonox_crash.log");

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(PathConfig::getLogPathString());

        // Parse additional options file now that we know its path
        CommandLineParser::parseFile();

#ifdef ORXONOX_PLATFORM_WINDOWS
        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLineParser::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity(static_cast<unsigned int>(limitToCPU));
#endif

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        this->configuration_->initialise();
    }

    /**
    @brief
        All destruction code is handled by scoped_ptrs and ScopeGuards.
    */
    Core::~Core()
    {
    }

    /**
        @brief Returns the softDebugLevel for the given device (returns a default-value if the class is right about to be created).
        @param device The device
        @return The softDebugLevel
    */
    /*static*/ int Core::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        switch (device)
        {
        case OutputHandler::LD_All:
            return Core::getInstance().configuration_->softDebugLevel_;
        case OutputHandler::LD_Console:
            return Core::getInstance().configuration_->softDebugLevelConsole_;
        case OutputHandler::LD_Logfile:
            return Core::getInstance().configuration_->softDebugLevelLogfile_;
        case OutputHandler::LD_Shell:
            return Core::getInstance().configuration_->softDebugLevelShell_;
        default:
            assert(0);
            return 2;
        }
    }

     /**
        @brief Sets the softDebugLevel for the given device. Please use this only temporary and restore the value afterwards, as it overrides the configured value.
        @param device The device
        @param level The level
    */
    /*static*/ void Core::setSoftDebugLevel(OutputHandler::OutputDevice device, int level)
    {
        if (device == OutputHandler::LD_All)
            Core::getInstance().configuration_->softDebugLevel_ = level;
        else if (device == OutputHandler::LD_Console)
            Core::getInstance().configuration_->softDebugLevelConsole_ = level;
        else if (device == OutputHandler::LD_Logfile)
            Core::getInstance().configuration_->softDebugLevelLogfile_ = level;
        else if (device == OutputHandler::LD_Shell)
            Core::getInstance().configuration_->softDebugLevelShell_ = level;

        OutputHandler::setSoftDebugLevel(device, level);
    }

    /**
    @note
        The code of this function has been copied and adjusted from OGRE, an open source graphics engine.
            (Object-oriented Graphics Rendering Engine)
        For the latest info, see http://www.ogre3d.org/

        Copyright (c) 2000-2008 Torus Knot Software Ltd

        OGRE is licensed under the LGPL. For more info, see OGRE license.
    */
    void Core::setThreadAffinity(int limitToCPU)
    {
#ifdef ORXONOX_PLATFORM_WINDOWS

        if (limitToCPU <= 0)
            return;

        unsigned int coreNr = limitToCPU - 1;
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

        // if the core specified with coreNr is not available, take the lowest one
        if (!(procMask & (1 << coreNr)))
            coreNr = 0;

        // Find the lowest core that this process uses and coreNr suggests
        DWORD threadMask = 1;
        while ((threadMask & procMask) == 0 || (threadMask < (1u << coreNr)))
            threadMask <<= 1;

        // Set affinity to the first core
        SetThreadAffinityMask(GetCurrentThread(), threadMask);
#endif
    }
}
