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

/**
    @file Core.h
    @brief Declaration of the Settings class.

    The static Settings class is only used to configure some variables
    through the config-file.
*/

#ifndef _Settings_H__
#define _Settings_H__

#include "OrxonoxPrereqs.h"
#include <string>
#include "core/OrxonoxClass.h"
#include "util/Debug.h"
#include "util/MultiType.h"

namespace orxonox
{
    class _OrxonoxExport Settings : public OrxonoxClass
    {
        friend class ClassIdentifier<Settings>;
        friend class GSRoot;

    public:
        static const std::string& getDataPath()
        { assert(singletonRef_s); return singletonRef_s->dataPath_; }
        static void tsetDataPath(const std::string& path)
        { assert(singletonRef_s); singletonRef_s->_tsetDataPath(path); }

    private:
        Settings();
        Settings(const Settings& instance);
        ~Settings() { singletonRef_s = 0; }

        static Settings& _getInstance() { assert(singletonRef_s); return *singletonRef_s; }
        void _tsetDataPath(const std::string& path);
        void dataPathChanged();

        void setConfigValues();

        std::string dataPath_;                                 //!< Path to the game data

        static Settings* singletonRef_s;                       //!< Static pointer to the only instance.
    };

}

#endif /* _Settings_H__ */
