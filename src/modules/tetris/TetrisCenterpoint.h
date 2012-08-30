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
    @file TetrisCenterpoint.h
    @brief Declaration of the TetrisCenterpoint class.
    @ingroup Tetris
*/

#ifndef _TetrisCenterpoint_H__
#define _TetrisCenterpoint_H__

#include "tetris/TetrisPrereqs.h"

#include <string>

#include <util/Math.h>

#include "worldentities/StaticEntity.h"

namespace orxonox
{//idea: add 2 triggers to the centerpoint (one to determine when a box would go above the centerpoint;
//the other to find out when the lowest row is filled totally)
    
    /**
    @brief
        
    
    @author
        
    @ingroup Tetris
    */
    class _TetrisExport TetrisCenterpoint : public StaticEntity
    {
        public:
            TetrisCenterpoint(BaseObject* creator); //!< Constructor. Registers and initializes the object and checks whether the gametype is actually Tetris.
            virtual ~TetrisCenterpoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method to create a TetrisCenterpoint through XML.

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
            @brief Set the size of a single stone.
            @param size The dimensions a stone has in the game world. (A stone is a cube)
            */
            void setStoneSize(float size)
                { this->stoneSize_ = size; }
            /**
            @brief Get the size of a single stone.
            @return Returns the dimensions a stone has in the game world.
            */
            float getStoneSize(void) const
                { return this->stoneSize_; }
                
            /**
            @brief Set the template for the stones.
            @param template The template name to be applied to each stone.
            */
            void setStoneTemplate(const std::string& templateName)
                { this->stoneTemplate_ = templateName; }
            /**
            @brief Get the template for the stones.
            @return Returns the template name to be applied to each stone.
            */
            const std::string& getStoneTemplate(void) const
                { return this->stoneTemplate_; }
            
            /**
            @brief Set the template for the bricks.
            @param template The template name to be applied to each brick.
            */
            void setBrickTemplate(const std::string& templateName)
                { this->brickTemplate_ = templateName; }
            /**
            @brief Get the template for the bricks.
            @return Returns the template name to be applied to each brick.
            */
            const std::string& getBrickTemplate(void) const
                { return this->brickTemplate_; }

            /**
            @brief Set the speed of the stones.
            @param speed The speed to be set.
            */
            void setStoneSpeed(float speed)
                { this->stoneSpeed_ = speed; }
            /**
            @brief Get the speed of the stones.
            @return Returns the speed a moving stone has.
            */
            float getStoneSpeed(void)
                { return this->stoneSpeed_; }

        private:
            void checkGametype(); //!< Checks whether the gametype is Tetris and if it is, sets its centerpoint.
            
            unsigned int width_;
            unsigned int height_;
            float stoneSize_;
            std::string stoneTemplate_;
            std::string brickTemplate_;
            float stoneSpeed_;

    };
}

#endif /* _TetrisCenterpoint_H__ */
