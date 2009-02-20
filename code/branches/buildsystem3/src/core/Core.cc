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
 *   Co-authors:
 *      Reto Grieder
 *
 */

/**
    @file
    @brief Implementation of the Core class.
*/

#include "Core.h"
#include <cassert>
#include "Language.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "LuaBind.h"
#include "CommandLine.h"

namespace orxonox
{
    bool Core::bShowsGraphics_s = false;
    bool Core::bHasServer_s     = false;
    bool Core::bIsClient_s      = false;
    bool Core::bIsStandalone_s  = false;
    bool Core::bIsMaster_s      = false;

    bool Core::isDevBuild_s     = false;
    std::string Core::configPath_s(ORXONOX_CONFIG_INSTALL_PATH); // from OrxonoxConfig.h
    std::string Core::logPath_s   (ORXONOX_LOG_INSTALL_PATH);    // from OrxonoxConfig.h

    Core* Core::singletonRef_s = 0;

    SetCommandLineArgument(mediaPath, "").information("PATH");

    /**
        @brief Constructor: Registers the object and sets the config-values.
        @param A reference to a global variable, used to avoid an infinite recursion in getSoftDebugLevel()
    */
    Core::Core()
    {
        RegisterRootObject(Core);

        assert(Core::singletonRef_s == 0);
        Core::singletonRef_s = this;

        this->bInitializeRandomNumberGenerator_ = false;
        this->setConfigValues();

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(Core::logPath_s);

        // Possible media path override by the command line
        if (!CommandLine::getArgument("mediaPath")->hasDefaultValue())
        {
            std::string mediaPath = CommandLine::getValue("mediaPath");
            Core::tsetMediaPath(mediaPath);
        }
    }

    /**
        @brief Sets the bool to true to avoid static functions accessing a deleted object.
    */
    Core::~Core()
    {
        assert(Core::singletonRef_s);
        Core::singletonRef_s = 0;
    }

    /**
        @brief Function to collect the SetConfigValue-macro calls.
    */
    void Core::setConfigValues()
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
        SetConfigValue(softDebugLevelConsole_, defaultLevelConsole)
            .description("The maximal level of debug output shown in the console").callback(this, &Core::debugLevelChanged);
        SetConfigValue(softDebugLevelLogfile_, defaultLevelLogfile)
            .description("The maximal level of debug output shown in the logfile").callback(this, &Core::debugLevelChanged);
        SetConfigValue(softDebugLevelShell_, defaultLevelShell)
            .description("The maximal level of debug output shown in the ingame shell").callback(this, &Core::debugLevelChanged);

        SetConfigValue(language_, Language::getLanguage().defaultLanguage_).description("The language of the ingame text").callback(this, &Core::languageChanged);
        SetConfigValue(bInitializeRandomNumberGenerator_, true).description("If true, all random actions are different each time you start the game").callback(this, &Core::initializeRandomNumberGenerator);

        // Media path (towards config and log path) is ini-configurable
        const char* defaultMediaPath = ORXONOX_MEDIA_INSTALL_PATH;
        if (Core::isDevBuild())
            defaultMediaPath = ORXONOX_MEDIA_DEV_PATH;

        SetConfigValue(mediaPath_, defaultMediaPath)
            .description("Relative path to the game data.").callback(this, &Core::mediaPathChanged);

    }

    /**
        @brief Callback function if the debug level has changed.
    */
    void Core::debugLevelChanged()
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

    /**
        @brief Callback function if the language has changed.
    */
    void Core::languageChanged()
    {
        // Read the translation file after the language was configured
        Language::getLanguage().readTranslatedLanguageFile();
    }

    /**
    @brief
        Callback function if the media path has changed.
    */
    void Core::mediaPathChanged()
    {
        if (mediaPath_ != "" && mediaPath_[mediaPath_.size() - 1] != '/')
        {
            ModifyConfigValue(mediaPath_, set, mediaPath_ + "/");
        }

        if (mediaPath_ == "")
        {
            ModifyConfigValue(mediaPath_, set, "/");
            COUT(2) << "Warning: Data path set to \"/\", is that really correct?" << std::endl;
        }
    }

    /**
        @brief Returns the softDebugLevel for the given device (returns a default-value if the class ist right about to be created).
        @param device The device
        @return The softDebugLevel
    */
    int Core::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        switch (device)
        {
        case OutputHandler::LD_All:
            return Core::getInstance().softDebugLevel_;
        case OutputHandler::LD_Console:
            return Core::getInstance().softDebugLevelConsole_;
        case OutputHandler::LD_Logfile:
            return Core::getInstance().softDebugLevelLogfile_;
        case OutputHandler::LD_Shell:
            return Core::getInstance().softDebugLevelShell_;
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
     void Core::setSoftDebugLevel(OutputHandler::OutputDevice device, int level)
     {
        if (device == OutputHandler::LD_All)
            Core::getInstance().softDebugLevel_ = level;
        else if (device == OutputHandler::LD_Console)
            Core::getInstance().softDebugLevelConsole_ = level;
        else if (device == OutputHandler::LD_Logfile)
            Core::getInstance().softDebugLevelLogfile_ = level;
        else if (device == OutputHandler::LD_Shell)
            Core::getInstance().softDebugLevelShell_ = level;

        OutputHandler::setSoftDebugLevel(device, level);
     }

    /**
        @brief Returns the configured language.
    */
    const std::string& Core::getLanguage()
    {
        return Core::getInstance().language_;
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    void Core::resetLanguage()
    {
        Core::getInstance().resetLanguageIntern();
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    void Core::resetLanguageIntern()
    {
        ResetConfigValue(language_);
    }

    /**
    @brief
        Temporary sets the media path
    @param path
        The new media path
    */
    void Core::_tsetMediaPath(const std::string& path)
    {
        if (*path.end() != '/' && *path.end() != '\\')
        {
            ModifyConfigValue(mediaPath_, tset, path + "/");
        }
        else
        {
            ModifyConfigValue(mediaPath_, tset, path);
        }
    }

    void Core::initializeRandomNumberGenerator()
    {
        static bool bInitialized = false;
        if (!bInitialized && this->bInitializeRandomNumberGenerator_)
        {
            srand(time(0));
            rand();
            bInitialized = true;
        }
    }

    /*static*/ void Core::setDevBuild()
    {
        // Be careful never to call this function before main()!

        Core::isDevBuild_s = true;
        // Constants taken from OrxonoxConfig.h
        Core::configPath_s = ORXONOX_CONFIG_DEV_PATH;
        Core::logPath_s    = ORXONOX_LOG_DEV_PATH;
    }
}
