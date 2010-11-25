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
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file ForceField.h
    @brief Definition of the ForceField class.
    @ingroup Objects
*/

/**
@file ForceField.h
@brief Definition of the ForceField class.
@inGroup Objects
*/

#ifndef _ForceField_H__
#define _ForceField_H__

#include "objects/ObjectsPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    /**
    @brief
        The mode of the ForceField.
    
    @inGroup Objects
    */
    namespace forceFieldMode
    {
        enum Value {
            tube, //!< The ForceField has a tube shape.
            sphere //!< The ForceField has a spherical shape.
            
        };
    }

    /**
    @brief
        Implements a force field, that applies a force to any @ref orxonox::MoblieEnity "MobileEntity" that enters its range.
        
        The following parameters can be set to specify the behavior of the ForceField.
        - @b velocity The amount of force the ForceField excerts.
        - @b diameter The diameter of the ForceField.
        - @b length The length of the ForceField.
        - @b mode The mode the ForceField is in. For mode:
        -- <em>tube</em> A ForceField which exerts force only in the direction it is oriented. The force is only exerted on objects that are in a tube of length <em>length</em> and diameter <em>diameter</em>. The magintude of the force is proportional to the <em><velocity/em>, being highest when an object is in the middle of the tube (radius-wise), linearly decreasing with greater radii and finally reaching zero, when the object is <code>diameter/2</code> away from the orientation vector.
        -- <em>sphere</em> A Force Field which exerts force radially away from itself, with the greatest magnitude (proportional to <em>velocity</em>) being in the origin of the ForceField, linearly decreasing with respect to the distance to the origin and finally reaching zero at distance <code>diameter/2</code>.
        
    @author
        Aurelian Jaggi
        
    @author
        Damian 'Mozork' Frick
        
    @inGroup Objects
    */
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
                { return this->velocity_; }

            inline void setDiameter(float diam)
                { this->diameter_ = diam; this->radius_ = diam/2; }

            inline float getDiameter()
                { return this->diameter_; }

            inline void setLength(float l)
                { this->length_ = l; this->halfLength_ = l/2; }

            inline float getLength()
                { return this->length_; }
                
            void setMode(const std::string& mode);
                
            inline const std::string& getMode(void);

        private:
            static const std::string modeTube_s;
            static const std::string modeSphere_s;
        
            float velocity_;
            float diameter_;
            float radius_;
            float length_;
            float halfLength_;
            forceFieldMode::Value mode_;
  };
}

#endif

