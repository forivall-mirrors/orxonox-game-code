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

#ifndef _GameConfig_H__
#define _GameConfig_H__

#include "core/CorePrereqs.h"

#include "core/config/Configurable.h"

namespace orxonox
{
    class _CoreExport GameConfig : virtual public Configurable
    {
        public:
            GameConfig();

            void setConfigValues();

            inline unsigned int getStatisticsRefreshCycle() const
                { return this->statisticsRefreshCycle_; }
            inline unsigned int getStatisticsAvgLength() const
                { return this->statisticsAvgLength_; }
            inline unsigned int getFpsLimit() const
                { return this->fpsLimit_; }

        private:
            // config values
            unsigned int statisticsRefreshCycle_;
            unsigned int statisticsAvgLength_;
            unsigned int fpsLimit_;
    };
}

#endif /* _GameConfig_H__ */
