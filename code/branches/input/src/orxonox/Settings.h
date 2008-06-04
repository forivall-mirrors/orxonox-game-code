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
    @brief Definition of the Settings class.

    The static Settings class is only used to configure some variables
    through the config-file.
*/

#ifndef _Settings_H__
#define _Settings_H__

#include "OrxonoxPrereqs.h"

namespace orxonox
{
  class _OrxonoxExport Settings : public OrxonoxClass
  {
    public:
      void setConfigValues();

      static const std::string& getDataPath();

      static void tsetDataPath(const std::string& path);

    private:
      Settings();
      Settings(const Settings& instance);
      ~Settings();
      static Settings& getSingleton();

      void _tsetDataPath(const std::string& path);

      std::string dataPath_;               //!< Path to the game data
  };
}

#endif /* _Settings_H__ */
