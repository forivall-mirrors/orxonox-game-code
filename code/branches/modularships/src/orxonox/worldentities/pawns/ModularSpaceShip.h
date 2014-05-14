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
 *      Noe Pedrazzini
 *
 */

#ifndef _ModularSpaceShip_H__
#define _ModularSpaceShip_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <LinearMath/btVector3.h>

#include "tools/Timer.h"
#include "util/Math.h"
#include "util/OrxAssert.h"

#include "SpaceShip.h"
#include "items/ShipPart.h"
#include <map>

namespace orxonox
{

    /**
    @brief
        The SpaceShip is the principal entity through which the player interacts with the game. Its main function is to fly, however many things, such as @ref orxonox::Engine Engines or @ref orxonox::Weapon Weapons, can be attached to it.
        The ModularSpaceShip is an extension of a @ref orxonox::SpaceShip SpaceShip, which uses @ref orxonox::ShipPart ShipParts to allow entities attached to it to be destroyed individually.

        There are several parameters that define the behavior of the ModularSpaceShip>
        - The <b>rotationThrust</b>, specifies the force with which the ModularSpaceShip rotates.
        - The <b>boost</b>, there are quite some parameters pertaining to boosting. The boost is a special move of the ModularSpaceShip, where, for a limited amount of time, it can fly considerably faster than usual. The <b>boostPower</b> is the amount of power available for boosting. The <b>boostPowerRate</b> is the rate at which the boost power is replenished. The <b>boostRate</b> is the rate at which boosting uses power. And the <b>boostCooldownDuration</b> is the time the ModularSpaceShip cannot boost, once all the boost power has been used up. Naturally all of these parameters must be non-negative.
        - The <b>boost shaking</b>, when the ModularSpaceShip boosts, the camera shakes to create a more immersive effect. Two parameters can be used to adjust the effect. The <b>shakeFrequency</b> is the frequency with which the camera shakes. And the <b>shakeAmplitude</b> is the amount with which the camera shakes. Again these parameters must bee non-negative.
        - The <b>lift</b> creates a more natural flight feeling through the addition of a lift force. There are again tow parameters that can be specified. The <b>lift</b> which is the lift force that is applied. And the <b>stallSpeed</b> which is the forward speed after which no more lift is generated.

        As mentioned @ref orxonox::Engine Engines can be mounted on the ModularSpaceShip.
        In order to assign attached entities to a ShipPart, a ShipPart with the same name as the corresponding entity needs to be created in the <parts> tag.
        Here is a (primitive) example of a ModularSpaceShip defined in XML:
        @code
        <ModularSpaceShip
            rotationThrust    = 50

            lift = 1;
            stallSpeed = 220;

            boostPower            = 15
            boostPowerRate        = 1
            boostRate             = 5
            boostCooldownDuration = 10

            shakeFrequency = 15
            shakeAmplitude = 9

            collisionType     = "dynamic"
            mass              = 100
            linearDamping     = 0.7
            angularDamping    = 0.9999999
            >
                <attached>
                    <StaticEntity name="wing"  . . .  />
                    <StaticEntity name="tail" . . . />
                </attached>
                <parts>
                    <ShipPart name="wing" . . . />
                    <ShipPart name="tail" . . . />
                </parts>
                <engines>
                    <Engine />
                    <Engine />
                </engines>
            </ModularSpaceShip>
        @endcode

    @author
        Fabian 'x3n' Landau, Noe Pedrazzini
    */
    class _OrxonoxExport ModularSpaceShip : public SpaceShip
    {
        public:
            ModularSpaceShip(Context* context);
            virtual ~ModularSpaceShip();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void attach(WorldEntity* object);

            void addPartEntityAssignment(StaticEntity* entity, ShipPart* part);
            ShipPart* getPartOfEntity(StaticEntity* entity) const;

            virtual void damage(float damage, float healthdamage = 0.0f, float shielddamage = 0.0f, Pawn* originator = NULL, const btCollisionShape* cs = NULL);

            static void killShipPart(std::string name);

            void addShipPart(ShipPart* part);
            ShipPart* getShipPart(unsigned int index);
            bool hasShipPart(ShipPart* part) const;
            void removeShipPart(ShipPart* part);

            void detach(WorldEntity* object);

            virtual void updatePartAssignment();

        protected:


        private:
            void registerVariables();
            std::vector<ShipPart*> partList_;                       // The list of all Parts mounted on this ModularSpaceShip.
            std::map<StaticEntity*, ShipPart*> partMap_;            // Map of Part-Entity-assignments
            static std::map<StaticEntity*, ShipPart*>* partMap_s;
        
    };
}

#endif /* _ModularSpaceShip_H__ */
