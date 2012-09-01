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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */


#ifndef _ForceField_H__
#define _ForceField_H__

#include "objects/ObjectsPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _ObjectsExport ForceField : public StaticEntity, public Tickable
    {
    public:
        ForceField(BaseObject* creator);
        virtual ~ForceField();
        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        virtual void tick(float dt);

        inline void setVelocity(float vel)
            { this->velocity_ = vel; }

        inline float getVelocity()
            { return velocity_; }

        inline void setDiameter(float diam)
            { this->diameter_ = diam; }

        inline float getDiameter()
            { return diameter_; }

        inline void setLength(float l)
            { this->length_ = l; }

        inline float getLength()
            { return length_; }

    private:
        float velocity_;
        float diameter_;
        float length_;
  };
}

#endif

