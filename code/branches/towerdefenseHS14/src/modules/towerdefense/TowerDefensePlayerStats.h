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
 *
 *   Co-authors:
 *      ...
 *
 */

 /**
    @brief
        This manages the stats of the player. It is used by 'TowerDefense', the gametype

    @ingroup TowerDefense
 */


#ifndef _TowerDefensePlayerStats_H__
#define _TowerDefensePlayerStats_H__

#include "towerdefense/TowerDefensePrereqs.h"

namespace orxonox
{
    typedef enum _TowerCosts {
        // Set to 0 for debug...
        TDDefaultTowerCost = 20
    } TowerCost;

    class _TowerDefenseExport TowerDefensePlayerStats
    {
    public:
        TowerDefensePlayerStats();

        inline int getCredit()
            { return credit_; }

        inline void setCredit(int credit)
            { credit_ = credit; }

        inline void buyTower(TowerCost cost)
            { credit_ -= cost;}

        inline int getWaveNumber()
            { return waveNumber_; }

        inline void didLoadNextWave()
            { waveNumber_++; }

    private:
        int credit_;
        int waveNumber_;
        //int baseHealth_;
    };
}

#endif /* _TowerDefensePlayerStats_H__ */
