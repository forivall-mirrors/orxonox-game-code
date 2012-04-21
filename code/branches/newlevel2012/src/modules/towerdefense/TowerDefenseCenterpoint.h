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
    @file TowerDefenseCenterpoint.h
    @brief Declaration of the TowerDefenseCenterpoint class.
    @ingroup TowerDefense
*/

#ifndef _TowerDefenseCenterpoint_H__
#define _TowerDefenseCenterpoint_H__

#include "towerdefense/TowerDefensePrereqs.h"

#include <string>

#include <util/Math.h>

#include "worldentities/StaticEntity.h"

namespace orxonox
{
    
    /**
    @brief
        
    
    @author
        
    @ingroup TowerDefense
    */
    class _TowerDefenseExport TowerDefenseCenterpoint : public StaticEntity
    {
        public:
            TowerDefenseCenterpoint(BaseObject* creator); //!< Constructor. Registers and initializes the object and checks whether the gametype is actually TowerDefense.
            virtual ~TowerDefenseCenterpoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method to create a TowerDefenseCenterpoint through XML.

            virtual void changedGametype(); //!< Is called when the gametype has changed.
            
            /**
            @brief Set the width of the playing field.
            @param width The width in number of tiles.
            */
            void setWidth(unsigned int width)
                { this->width_ = width; }
            /**
            @brief Get the width of the playing field.
            @return Returns the width in number of tiles.
            */
            unsigned int getWidth(void) const
                { return this->width_; }
                
            /**
            @brief Set the height of the playing field.
            @param height The height in number of tiles.
            */
            void setHeight(unsigned int height)
                { this->height_ = height; }
            /**
            @brief Get the height of the playing field.
            @return Returns the height in number of tiles.
            */
            unsigned int getHeight(void) const
                { return this->height_; }
                
            /**
            @brief Set the template for the towers.
            @param template The template name to be applied to each tower.
            */
            void setTowerTemplate(const std::string& templateName)
                { this->towerTemplate_ = templateName; }
            /**
            @brief Get the template for the towers.
            @return Returns the template name to be applied to each tower.
            */
            const std::string& getTowerTemplate(void) const
                { return this->towerTemplate_; }
            

        private:
            void checkGametype(); //!< Checks whether the gametype is TowerDefense and if it is, sets its centerpoint.
            
            unsigned int width_;
            unsigned int height_;
            std::string towerTemplate_;

    };
}

#endif /* _TowerDefenseCenterpoint_H__ */
