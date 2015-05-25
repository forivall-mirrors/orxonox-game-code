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

#include "CoreConfig.h"

#include "util/output/LogWriter.h"
#include "util/output/OutputManager.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/Language.h"
#include "core/PathConfig.h"

namespace orxonox
{
    RegisterClassNoArgs(CoreConfig);

    CoreConfig::CoreConfig()
        : bDevMode_(false)
        , bStartIOConsole_(true)
        , lastLevelTimestamp_(0)
        , ogreConfigTimestamp_(0)
    {
        RegisterObject(CoreConfig);
    }

    //! Function to collect the SetConfigValue-macro calls.
    void CoreConfig::setConfigValues()
    {
        SetConfigValueExternal(OutputManager::getInstance().getLogWriter()->configurableMaxLevel_,
                               OutputManager::getInstance().getLogWriter()->getConfigurableSectionName(),
                               OutputManager::getInstance().getLogWriter()->getConfigurableMaxLevelName(),
                               OutputManager::getInstance().getLogWriter()->configurableMaxLevel_)
            .description("The maximum level of output shown in the log file")
            .callback(static_cast<BaseWriter*>(OutputManager::getInstance().getLogWriter()), &BaseWriter::changedConfigurableLevel);
        SetConfigValueExternal(OutputManager::getInstance().getLogWriter()->configurableAdditionalContextsMaxLevel_,
                               OutputManager::getInstance().getLogWriter()->getConfigurableSectionName(),
                               OutputManager::getInstance().getLogWriter()->getConfigurableAdditionalContextsMaxLevelName(),
                               OutputManager::getInstance().getLogWriter()->configurableAdditionalContextsMaxLevel_)
            .description("The maximum level of output shown in the log file for additional contexts")
            .callback(static_cast<BaseWriter*>(OutputManager::getInstance().getLogWriter()), &BaseWriter::changedConfigurableAdditionalContextsLevel);
        SetConfigValueExternal(OutputManager::getInstance().getLogWriter()->configurableAdditionalContexts_,
                               OutputManager::getInstance().getLogWriter()->getConfigurableSectionName(),
                               OutputManager::getInstance().getLogWriter()->getConfigurableAdditionalContextsName(),
                               OutputManager::getInstance().getLogWriter()->configurableAdditionalContexts_)
            .description("Additional output contexts shown in the log file")
            .callback(static_cast<BaseWriter*>(OutputManager::getInstance().getLogWriter()), &BaseWriter::changedConfigurableAdditionalContexts);

        SetConfigValue(bDevMode_, PathConfig::buildDirectoryRun())
            .description("Developer mode. If not set, hides some things from the user to not confuse him.")
            .callback(this, &CoreConfig::devModeChanged);
        SetConfigValue(language_, Language::getInstance().defaultLanguage_)
            .description("The language of the in game text")
            .callback(this, &CoreConfig::languageChanged);
        SetConfigValue(bInitRandomNumberGenerator_, true)
            .description("If true, all random actions are different each time you start the game")
            .callback(this, &CoreConfig::initRandomNumberGenerator);
        SetConfigValue(bStartIOConsole_, true)
            .description("Set to false if you don't want to use the IOConsole (for Lua debugging for instance)");
        SetConfigValue(lastLevelTimestamp_, 0)
            .description("Timestamp when the last level was started.");
        SetConfigValue(ogreConfigTimestamp_, 0)
            .description("Timestamp when the ogre config file was changed.");
    }

    /** Callback function for changes in the dev mode that affect debug levels.
        The function behaves according to these rules:
        - 'normal' mode is defined based on where the program was launched: if
          the launch path was the build directory, development mode \c on is
          normal, otherwise normal means development mode \c off.
        - Debug levels should not be hard configured (\c config instead of
          \c tconfig) in non 'normal' mode to avoid strange behaviour.
        - Changing the development mode from 'normal' to the other state will
          immediately change the debug levels to predefined values which can be
          reconfigured with \c tconfig.
    @note
        The debug levels for the IOConsole and the InGameConsole can be found
        in the Shell class. The same rules apply.
    */
    void CoreConfig::devModeChanged()
    {
        // Inform listeners
        ObjectList<DevModeListener>::iterator it = ObjectList<DevModeListener>::begin();
        for (; it != ObjectList<DevModeListener>::end(); ++it)
            it->devModeChanged(bDevMode_);
    }

    //! Callback function if the language has changed.
    void CoreConfig::languageChanged()
    {
        // Read the translation file after the language was configured
        Language::getInstance().readTranslatedLanguageFile();
    }

    void CoreConfig::initRandomNumberGenerator()
    {
        static bool bInitialized = false;
        if (!bInitialized && this->bInitRandomNumberGenerator_)
        {
            srand(static_cast<unsigned int>(time(0)));
            rand();
            bInitialized = true;
        }
    }

    //! Sets the language in the config-file back to the default.
    void CoreConfig::resetLanguage()
    {
        ResetConfigValue(language_);
    }

    void CoreConfig::updateLastLevelTimestamp()
    {
        ModifyConfigValue(lastLevelTimestamp_, set, static_cast<long long>(time(NULL)));
    }

    void CoreConfig::updateOgreConfigTimestamp()
    {
        ModifyConfigValue(ogreConfigTimestamp_, set, static_cast<long long>(time(NULL)));
    }


    RegisterAbstractClass(DevModeListener).inheritsFrom<Listable>();

    DevModeListener::DevModeListener()
    {
        RegisterObject(DevModeListener);
    }
}
