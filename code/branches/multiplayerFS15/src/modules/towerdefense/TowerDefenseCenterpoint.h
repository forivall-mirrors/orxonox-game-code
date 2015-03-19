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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
 @brief
 See TowerDefenseReadme.txt for Information.
 @ingroup TowerDefense
 */


#ifndef _TowerDefenseCenterpoint_H__
#define _TowerDefenseCenterpoint_H__

#include "towerdefense/TowerDefensePrereqs.h"

#include <string>
#include <util/Math.h>

#include "worldentities/MobileEntity.h"

namespace orxonox
{
    class _TowerDefenseExport TowerDefenseCenterpoint : public MobileEntity
    {
        public:
            TowerDefenseCenterpoint(Context* context);
            virtual ~TowerDefenseCenterpoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedGametype();

            /**
            @brief The width and hight in number of tiles. Default is 15 for both.
            */
            void setWidth(unsigned int width)
                { this->width_ = width; }

            unsigned int getWidth(void) const
                { return this->width_; }

            void setHeight(unsigned int height)
                { this->height_ = height; }

            unsigned int getHeight(void) const
                { return this->height_; }

            /**
                @brief How to convert to world coordinates, e.g. that 0,15 is not at -8,-8 but at -80,-80 (if scale would be 10)
            */
            void setTileScale(unsigned int tileScale)
                { this->tileScale_ = tileScale; }

            unsigned int getTileScale(void) const
                { return this->tileScale_; }

            /**
            @brief Set the template for the towers.
            @param template The template name to be applied to each tower.
            */
            void setTowerTemplate(const std::string& templateName)
                { this->towerTemplate_ = templateName; }

            const std::string& getTowerTemplate(void) const
                { return this->towerTemplate_; }

        private:
            void checkGametype();

            unsigned int width_;
            unsigned int height_;
            unsigned int tileScale_;

            std::string towerTemplate_;
    };
}

#endif /* _TowerDefenseCenterpoint_H__ */
