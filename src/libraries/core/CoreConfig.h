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

#ifndef _CoreConfig_H__
#define _CoreConfig_H__

#include "core/CorePrereqs.h"

#include "core/config/Configurable.h"

namespace orxonox
{
    class _CoreExport CoreConfig : virtual public Configurable
    {
        public:
            CoreConfig();

            void setConfigValues();

            //! Developers bit. If returns false, some options are not available as to not confuse the normal user.
            inline bool inDevMode(void) const
                { return this->bDevMode_; }

            //! Returns the configured language.
            const std::string& getLanguage()
                { return this->language_; }
            void resetLanguage();

            inline bool getStartIOConsole() const
                { return this->bStartIOConsole_; }

            void updateLastLevelTimestamp();
            inline long long getLastLevelTimestamp() const
                { return this->lastLevelTimestamp_; }

            void updateOgreConfigTimestamp();
            inline long long getOgreConfigTimestamp() const
                { return this->ogreConfigTimestamp_; }

        private:
            void devModeChanged();
            void languageChanged();
            void initRandomNumberGenerator();

            bool                      bDevMode_;                   //!< Developers bit. If set to false, some options are not available as to not confuse the normal user.
            std::string               language_;                   //!< The language
            bool                      bInitRandomNumberGenerator_; //!< If true, srand(time(0)) is called
            bool                      bStartIOConsole_;            //!< Set to false if you don't want to use the IOConsole
            long long                 lastLevelTimestamp_;         ///< Timestamp when the last level was started
            long long                 ogreConfigTimestamp_;        ///< Timestamp wehen the ogre config level was modified
    };

    //! Informs about changes in the Development Mode.
    class DevModeListener : virtual public Listable
    {
    public:
        DevModeListener();
        virtual ~DevModeListener() {}
        virtual void devModeChanged(bool value) = 0;
    };
}

#endif /* _CoreConfig_H__ */
