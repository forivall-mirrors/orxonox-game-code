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
 *      ...
 *
 */

/**
    @file CoreSettings.cc
    @brief Implementation of the CoreSettings class.
*/

#include "CoreSettings.h"
#include "Language.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"

namespace orxonox
{
    /**
        @brief Constructor: Registers the object and sets the config-values.
        @param A reference to a global variable, used to avoid an infinite recursion in getSoftDebugLevel()
    */
    CoreSettings::CoreSettings()
    {
        RegisterRootObject(CoreSettings);
        this->setConfigValues();
    }

    /**
        @brief Sets the bool to true to avoid static functions accessing a deleted object.
    */
    CoreSettings::~CoreSettings()
    {
        isCreatingCoreSettings() = true;
    }

    /**
        @brief Returns true if the CoreSettings instance is not yet ready and the static functions have to return a default value.
    */
    bool& CoreSettings::isCreatingCoreSettings()
    {
        static bool bCreatingCoreSettings = true;
        static bool bFirstTime = true;
        if (bFirstTime)
        {
            bFirstTime = false;
            CoreSettings::getInstance();
        }
        return bCreatingCoreSettings;
    }

    /**
        @brief Returns a unique instance of CoreSettings.
        @return The instance
    */
    CoreSettings& CoreSettings::getInstance()
    {
        static CoreSettings instance = CoreSettings();

        // If bCreatingSoftDebugLevelObject is true, we're just about to create an instance of the DebugLevel class
        if (CoreSettings::isCreatingCoreSettings())
        {
            isCreatingCoreSettings() = false;
            instance.setConfigValues();
        }
        return instance;
    }

    /**
        @brief Function to collect the SetConfigValue-macro calls.
    */
    void CoreSettings::setConfigValues()
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
        @brief Static function that holds the singleton.
    */
    int CoreSettings::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        if (!CoreSettings::isCreatingCoreSettings())
        {
            if (device == OutputHandler::LD_All)
                return CoreSettings::getInstance().softDebugLevel_;
            else if (device == OutputHandler::LD_Console)
                return CoreSettings::getInstance().softDebugLevelConsole_;
            else if (device == OutputHandler::LD_Logfile)
                return CoreSettings::getInstance().softDebugLevelLogfile_;
            else if (device == OutputHandler::LD_Shell)
                return CoreSettings::getInstance().softDebugLevelShell_;
        }

        // Return a constant value while we're creating the object
        return 2;
    }

    /**
        @brief Returns the configured language.
    */
    const std::string& CoreSettings::getLanguage()
    {
        if (!CoreSettings::isCreatingCoreSettings())
            return CoreSettings::getInstance().language_;

        return Language::getLanguage().defaultLanguage_;
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    void CoreSettings::resetLanguage()
    {
        CoreSettings::getInstance().resetLanguageIntern();
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    void CoreSettings::resetLanguageIntern()
    {
        ResetConfigValue(language_);
    }
}

/**
    @brief Returns the soft debug level, stored in the only existing instance of the DebugLevel class, configured in the config-file.
    @return The soft debug level
*/
int getSoftDebugLevel()
{
    return orxonox::CoreSettings::getSoftDebugLevel();
}
