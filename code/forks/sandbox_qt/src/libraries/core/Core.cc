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
    Implementation of the Core singleton
*/

#include "Core.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>

#ifdef ORXONOX_PLATFORM_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  undef min
#  undef max
#endif

#include "util/Debug.h"
#include "util/Exception.h"
#include "PathConfig.h"
#include "CommandLineParser.h"

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonPtr_s  = 0;

    SetCommandLineArgument(settingsFile, "orxonox.ini").information("THE configuration file");

#ifdef ORXONOX_PLATFORM_WINDOWS
    SetCommandLineArgument(limitToCPU, 1).information("Limits the program to one CPU/core (1, 2, 3, etc.). Default is the first core (faster than off)");
#endif

    Core::Core(const std::string& cmdLine)
    {
        // Set the hard coded fixed paths
        this->pathConfig_.reset(new PathConfig());

        // Parse command line arguments
        CommandLineParser::parseCommandLine(cmdLine);

        // Set configurable paths like log, config and media
        this->pathConfig_->setConfigurablePaths();

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% (Windows) was used
        OutputHandler::getInstance().setLogPath(PathConfig::getLogPathString());

        // Parse additional options file now that we know its path
        CommandLineParser::parseFile();

#ifdef ORXONOX_PLATFORM_WINDOWS
        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        int limitToCPU = CommandLineParser::getValue("limitToCPU").toInt();
        if (limitToCPU > 0)
            setThreadAffinity(static_cast<unsigned int>(limitToCPU));
#endif

        // Generate documentation instead of normal run?
        std::string docFilename = CommandLineParser::getValue("generateDoc").toString().toStdString();
        if (!docFilename.empty())
        {
            std::ofstream docFile(docFilename.c_str());
            if (docFile.is_open())
            {
                CommandLineParser::generateDoc(docFile);
                docFile.close();
            }
            else
                COUT(0) << "Error: Could not open file for documentation writing" << endl;
        }
    }

    /**
    @brief
        All destruction code is handled by std::auto_ptr
    */
    Core::~Core()
    {
    }

    //! Function to collect the SetConfigValue-macro calls.
    void Core::setConfigValues()
    {
#ifdef ORXONOX_RELEASE
        const unsigned int defaultLevelLogFile = 3;
#else
        const unsigned int defaultLevelLogFile = 4;
#endif
        /*
        SetConfigValueExternal(softDebugLevelLogFile_, "OutputHandler", "softDebugLevelLogFile", defaultLevelLogFile)
            .description("The maximum level of debug output shown in the log file");
        OutputHandler::getInstance().setSoftDebugLevel(OutputHandler::logFileOutputListenerName_s, this->softDebugLevelLogFile_);

        SetConfigValue(language_, Language::getInstance().defaultLanguage_)
            .description("The language of the in game text")
            .callback(this, &Core::languageChanged);
        SetConfigValue(bInitRandomNumberGenerator_, true)
            .description("If true, all random actions are different each time you start the game")
            .callback(this, &Core::initRandomNumberGenerator);
        SetConfigValue(bStartIOConsole_, true)
            .description("Set to false if you don't want to use the IOConsole (for Lua debugging for instance)");
        */
    }

    void Core::initRandomNumberGenerator()
    {
        static bool bInitialized = false;
        if (!bInitialized && this->bInitRandomNumberGenerator_)
        {
            srand(static_cast<unsigned int>(time(0)));
            rand();
            bInitialized = true;
        }
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
