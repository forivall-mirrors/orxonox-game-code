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
 *      Kevin Young
 *
 */

/**
    @file ForceField.h
    @brief Definition of the ForceField class.
    @ingroup Objects
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

    @ingroup Objects
    */
    namespace forceFieldMode
    {
        enum Value {
            tube, //!< The ForceField has a tube shape.
            sphere, //!< The ForceField has a spherical shape.
            invertedSphere, //!< The ForceField has a spherical shape but "inverted" behavior.
            newtonianGravity //!< The ForceField imitates Newtonian gravitation for use in stellar bodies.
        };
    }

    /**
    @brief
        Implements a force field, that applies a force to any @ref orxonox::MobileEntity "MobileEntity" that enters its range.

        The following parameters can be set to specify the behavior of the ForceField.
        - @b velocity The amount of force the ForceField excerts. Default is 100.
        - @b diameter The diameter of the ForceField. Default is 500.
        - @b length The length of the ForceField. Default is 2000.
        - @b mode The mode the ForceField is in. For mode:
            - <em>tube</em> A ForceField which exerts force only in the direction it is oriented. The force is only exerted on objects that are in a tube of length <em>length</em> and diameter <em>diameter</em> (with rounded start and end faces, so in fact the <em>length</em> parameter specifies a ball with <code>origin + length/2</code> as the center and <code>length/2</code> as the radius). The magintude of the force is proportional to the <em>velocity</em>, being highest when an object is in the middle of the tube (radius-wise), linearly decreasing with greater radii and finally reaching zero, when the object is <code>diameter/2</code> away from the orientation vector.
            - <em>sphere</em> A Force Field which exerts force radially away from itself, with the greatest magnitude (proportional to <em>velocity</em>) being in the origin of the ForceField, linearly decreasing with respect to the distance to the origin and finally reaching zero at distance <code>diameter/2</code>.
            Default is <em>tube</em>.
            - <em>invertedSphere</em> A ForceField which excerts force radially towards itself, with the highest force at the boundary of the sphere, linear decreasing until reaching a distance of <code>radius-length</code> from the origin, where the force reaches zero.

    @author
        Aurelian Jaggi

    @author
        Damian 'Mozork' Frick

    @ingroup Objects
    */
    class _ObjectsExport ForceField : public StaticEntity, public Tickable
    {
        public:
            ForceField(Context* context);
            virtual ~ForceField();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Creates a ForceField object through XML.
            void registerVariables(); //!< Registers the variables that should get synchronised over the network
            virtual void tick(float dt); //!< A method that is called every tick.
            

            /**
            @brief Set the velocity of the ForceField.
            @param vel The velocity to be set.
            */
            inline void setVelocity(float vel)
                { this->velocity_ = vel; }
            /**
            @brief Get the velocity of the ForceField.
            @return Returns the velocity of the ForceField.
            */
            inline float getVelocity()
                { return this->velocity_; }

            /**
            @brief Set the diameter of the ForceField. In mode StellarBody this stands for the outer radius.
            @param diam The diameter to be set.
            */
            inline void setDiameter(float diam)
                { this->radius_ = diam/2; }
            /**
            @brief Get the diameter of the ForceField. In mode StellarBody this stands for the outer radius.
            @return Returns the diameter of the ForceField.
            */
            inline float getDiameter()
                { return this->radius_*2; }
        
            /**
            @brief Set the diameter of the stellar body for the Newtonian ForceField.
            @param massDiam The diameter of the stellar body.
            */
            inline void setMassDiameter(float massDiam)
                { this->massRadius_ = massDiam/2; }
        
            /**
            @brief Get the diameter of the stellar body for the Newtonian ForceField.
            @return float Returns the diameter of the stellar body.
            */
            inline float getMassDiameter()
                { return this->massRadius_*2; }

            /**
            @brief Set the length of the ForceField.
            @param l The length to be set.
            */
            inline void setLength(float l)
                { this->halfLength_ = l/2; }
            /**
            @brief Get the length of the ForceField.
            @return Returns the length of the ForceField.
            */
            inline float getLength()
                { return this->halfLength_*2; }

            void setMode(const std::string& mode); //!< Set the mode of the ForceField.
            const std::string& getMode(void); //!< Get the mode of the ForceField.

        private:
            //! Strings to represent the modes.
            static const std::string modeTube_s;
            static const std::string modeSphere_s;
            static const std::string modeInvertedSphere_s;
            static const std::string modeNewtonianGravity_s;

            float velocity_; //!< The velocity of the ForceField.
            float radius_; //!< The radius of the ForceField.
            float massRadius_; //!< The radius of the stellar body for the Newtonian ForceField.
            float halfLength_; //!< Half of the length of the ForceField.
            int mode_; //!< The mode of the ForceField.
            
            //! Gravitational constant for Newtonian ForceFields.
            static const float gravConstant_;
            //! Attenuation factor for Newtonian ForceFields
            static const float attenFactor_;
  };
}

#endif
