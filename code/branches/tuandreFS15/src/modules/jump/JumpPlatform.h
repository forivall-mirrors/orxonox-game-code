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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpPlatform.h
    @brief Declaration of the JumpPlatform class.
    @ingroup Jump
*/

#ifndef _JumpPlatform_H__
#define _JumpPlatform_H__

#include "jump/JumpPrereqs.h"
#include "util/Math.h"
#include "worldentities/MovableEntity.h"

namespace orxonox
{
    class _JumpExport JumpPlatform : public MovableEntity
    {
        public:
            JumpPlatform(Context* context);
            virtual ~JumpPlatform();
            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setFigure(WeakPtr<JumpFigure> bats); //!< Set the bats for the ball.
            virtual void touchFigure();

            void setDefScoreSound(const std::string& engineSound);
            const std::string& getDefScoreSound();
            void setDefBatSound(const std::string& engineSound);
            const std::string& getDefBatSound();
            void setDefBoundarySound(const std::string& engineSound);
            const std::string& getDefBoundarySound();

            void setWidth(const float width)
                { this->width_ = width; }
            float getWidth() const
                { return width_; }
            void setHeight(const float height)
                { this->height_ = height; }
            float getHeight() const
                { return height_; }

        protected:
            float width_;
            float height_;
            WeakPtr<JumpFigure> figure_;
            WorldSound* defScoreSound_;
            WorldSound* defBatSound_;
            WorldSound* defBoundarySound_;
    };
}

#endif /* _JumpPlatform_H__ */
