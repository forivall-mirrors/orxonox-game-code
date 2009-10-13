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

#ifndef _PongCenterpoint_H__
#define _PongCenterpoint_H__

#include "pong/PongPrereqs.h"

#include <string>
#include <util/Math.h>
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _PongExport PongCenterpoint : public StaticEntity
    {
        public:
            PongCenterpoint(BaseObject* creator);
            virtual ~PongCenterpoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedGametype();

            void setBalltemplate(const std::string& balltemplate)
                { this->balltemplate_ = balltemplate; }
            const std::string& getBalltemplate() const
                { return this->balltemplate_; }

            void setBattemplate(const std::string& battemplate)
                { this->battemplate_ = battemplate; }
            const std::string& getBattemplate() const
                { return this->battemplate_; }

            void setFieldDimension(const Vector2& dimension)
                { this->width_ = dimension.x; this->height_ = dimension.y; }
            Vector2 getFieldDimension() const
                { return Vector2(this->width_, this->height_); }

            void setBallSpeed(float ballspeed)
                { this->ballspeed_ = ballspeed; }
            float getBallSpeed() const
                { return this->ballspeed_; }

            void setBallAccelerationFactor(float ballaccfactor)
                { this->ballaccfactor_ = ballaccfactor; }
            float getBallAccelerationFactor() const
                { return this->ballaccfactor_; }

            void setBatSpeed(float batspeed)
                { this->batspeed_ = batspeed; }
            float getBatSpeed() const
                { return this->batspeed_; }

            void setBatLength(float batlength)
                { this->batlength_ = batlength; }
            float getBatLength() const
                { return this->batlength_; }

        private:
            void checkGametype();

            std::string balltemplate_;
            std::string battemplate_;

            float ballspeed_;
            float ballaccfactor_;
            float batspeed_;
            float batlength_;

            float width_;
            float height_;
    };
}

#endif /* _PongCenterpoint_H__ */
