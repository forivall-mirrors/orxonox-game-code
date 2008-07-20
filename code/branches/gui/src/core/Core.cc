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
    @file Core.cc
    @brief Implementation of the Core class.
*/

#include "Core.h"
#include "Language.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "input/InputManager.h"
#include "TclThreadManager.h"

namespace orxonox
{
    /**
        @brief Constructor: Registers the object and sets the config-values.
        @param A reference to a global variable, used to avoid an infinite recursion in getSoftDebugLevel()
    */
    Core::Core()
    {
        RegisterRootObject(Core);
        this->setConfigValues();
    }

    /**
        @brief Sets the bool to true to avoid static functions accessing a deleted object.
    */
    Core::~Core()
    {
        isCreatingCoreSettings() = true;
    }

    /**
        @brief Returns true if the Core instance is not yet ready and the static functions have to return a default value.
    */
    bool& Core::isCreatingCoreSettings()
    {
        static bool bCreatingCoreSettings = true;
        static bool bFirstTime = true;
        if (bFirstTime)
        {
            bFirstTime = false;
            Core::getInstance();
        }
        return bCreatingCoreSettings;
    }

    /**
        @brief Returns a unique instance of Core.
        @return The instance
    */
    Core& Core::getInstance()
    {
        static Core instance = Core();

        // If bCreatingSoftDebugLevelObject is true, we're just about to create an instance of the DebugLevel class
        if (Core::isCreatingCoreSettings())
        {
            isCreatingCoreSettings() = false;
            instance.setConfigValues();
        }
        return instance;
    }

    /**
        @brief Function to collect the SetConfigValue-macro calls.
    */
    void Core::setConfigValues()
    {
        SetConfigValue(softDebugLevelConsole_, 3).description("The maximal level of debug output shown in the console");
        SetConfigValue(softDebugLevelLogfile_, 3).description("The maximal level of debug output shown in the logfile");
        SetConfigValue(softDebugLevelShell_, 1).description("The maximal level of debug output shown in the ingame shell");

        // softDebugLevel_ is the maximum of the 3 variables
        this->softDebugLevel_ = this->softDebugLevelConsole_;
        if (this->softDebugLevelLogfile_ > this->softDebugLevel_)
            this->softDebugLevel_ = this->softDebugLevelLogfile_;
        if (this->softDebugLevelShell_ > this->softDebugLevel_)
            this->softDebugLevel_ = this->softDebugLevelShell_;


        std::string temp = this->language_;
        SetConfigValue(language_, Language::getLanguage().defaultLanguage_).description("The language of the ingame text");

        if (this->language_ != temp)
        {
            // Read the translation file after the language was configured
            Language::getLanguage().readTranslatedLanguageFile();
        }
    }

    /**
        @brief Returns the softDebugLevel for the given device (returns a default-value if the class ist right about to be created).
        @param device The device
        @return The softDebugLevel
    */
    int Core::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        if (!Core::isCreatingCoreSettings())
        {
            if (device == OutputHandler::LD_All)
                return Core::getInstance().softDebugLevel_;
            else if (device == OutputHandler::LD_Console)
                return Core::getInstance().softDebugLevelConsole_;
            else if (device == OutputHandler::LD_Logfile)
                return Core::getInstance().softDebugLevelLogfile_;
            else if (device == OutputHandler::LD_Shell)
                return Core::getInstance().softDebugLevelShell_;
        }

        // Return a constant value while we're creating the object
        return 2;
    }

     /**
        @brief Sets the softDebugLevel for the given device. Please use this only temporary and restore the value afterwards, as it overrides the configured value.
        @param device The device
        @param level The level
    */
     void Core::setSoftDebugLevel(OutputHandler::OutputDevice device, int level)
     {
        if (!Core::isCreatingCoreSettings())
        {
            if (device == OutputHandler::LD_All)
                Core::getInstance().softDebugLevel_ = level;
            else if (device == OutputHandler::LD_Console)
                Core::getInstance().softDebugLevelConsole_ = level;
            else if (device == OutputHandler::LD_Logfile)
                Core::getInstance().softDebugLevelLogfile_ = level;
            else if (device == OutputHandler::LD_Shell)
                Core::getInstance().softDebugLevelShell_ = level;
        }
     }

    /**
        @brief Returns the configured language.
    */
    const std::string& Core::getLanguage()
    {
        if (!Core::isCreatingCoreSettings())
            return Core::getInstance().language_;

        return Language::getLanguage().defaultLanguage_;
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
        @brief Ticks every core class in a specified sequence. Has to be called
               every Orxonox tick!
        @param dt Delta Time
    */
    void Core::tick(float dt)
    {
        TclThreadManager::getInstance().tick(dt);
        InputManager::tick(dt);
    }
}

/**
    @brief Returns the soft debug level, stored in the only existing instance of the DebugLevel class, configured in the config-file.
    @return The soft debug level
*/
int getSoftDebugLevel()
{
    return orxonox::Core::getSoftDebugLevel();
}
