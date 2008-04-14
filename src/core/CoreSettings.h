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
    @file CoreSettings.h
    @brief Definition of the CoreSettings class.

    The CoreSettings class is a singleton, only used to configure some variables
    in the core through the config-file.
*/

#ifndef _CoreSettings_H__
#define _CoreSettings_H__

#include "CorePrereqs.h"

#include "OrxonoxClass.h"
#include "OutputHandler.h"

namespace orxonox
{
    //! The CoreSettings class is a singleton, only used to configure some config-values.
    class _CoreExport CoreSettings : public OrxonoxClass
    {
        public:
            static CoreSettings& getInstance();
            static bool& isCreatingCoreSettings();
            void setConfigValues();

            static int getSoftDebugLevel(OutputHandler::OutputDevice device = OutputHandler::LD_All);
            static const std::string& getLanguage();
            static void resetLanguage();

        private:
            void resetLanguageIntern();

            CoreSettings();
            CoreSettings(const CoreSettings& other) {}
            virtual ~CoreSettings();

            int softDebugLevel_;                            //!< The debug level
            int softDebugLevelConsole_;                     //!< The debug level for the console
            int softDebugLevelLogfile_;                     //!< The debug level for the logfile
            int softDebugLevelShell_;                       //!< The debug level for the ingame shell
            std::string language_;                          //!< The language
    };
}

#endif /* _CoreSettings_H__ */
