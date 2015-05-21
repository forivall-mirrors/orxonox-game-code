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
 *      Noe Pedrazzini
 *   Co-authors:
 *      ...
 *
 */

#ifndef _PartDestructionEvent_H__
#define _PartDestructionEvent_H__

#include "OrxonoxPrereqs.h"
#include "Item.h"

#include <string>


namespace orxonox // tolua_export
{ // tolua_export
    /**
        @brief
            In order to assign attached entities to a ShipPart, a ShipPart with the same name as the corresponding entity needs to be created in the <parts> tag.
            Here is a (primitive) example of a ModularSpaceShip with ShipParts and PartDestructionEvents defined in XML:
            @code
            <ModularSpaceShip
                ...
                >
                    <attached>
                        <StaticEntity name="generator"  . . .  />
                        <StaticEntity name="tail" . . . />
                    </attached>
                    <parts>
                        <ShipPart name="generator" . . . >
                            <destructionevents>
                                <PartDestructionEvent targetType="ship" targetParam="boostpowerrate" operation="-" value="0.5" message="Your boost-regeneration is reduced!" />
                            </destructionevents>
                        </ShipPart>
                        <ShipPart name="tail" . . . >
                            <destructionevents>
                                <PartDestructionEvent ... />
                            </destructionevents>
                        </ShipPart>
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
    class _OrxonoxExport PartDestructionEvent // tolua_export
        : public Item
    { // tolua_export

        public:

            /**
                @brief
                    List of all allowed parameters.
                */
            enum TargetParam
            {
                shieldhealth,
                maxshieldhealth,
                shieldabsorption,
                shieldrechargerate,
                boostpower,         // Amount of available boost
                boostpowerrate,     // Recharge-rate of boost
                rotationthrust,
                boostfactor,
                speedfront,
                accelerationfront,
                null
            };

            PartDestructionEvent(Context* context);
            virtual ~PartDestructionEvent();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void execute();

            inline void setValid(bool valid)
                { this->valid_ = valid; }
            inline bool isValid()
                { return this->valid_; }

            void setParent(ShipPart* parent);
            inline ShipPart* getParent()
                { return this->parent_; }

            void setTargetType(std::string type);
            inline std::string getTargetType()
                { return this->targetType_; }

            void setTargetName(std::string name);
            inline std::string getTargetName()
                { return this->targetName_; }

            void setTargetParam(std::string param);
            inline std::string getTargetParam()
                { return this->targetName_; }

            void setOperation(std::string operation);
            inline std::string getOperation()
                { return this->operation_; }

            void setMessage(std::string msg);
            inline std::string getMessage()
                { return this->message_; }

            float operate(float input);

            void setEventValue(float value);
            inline float getEventValue()
                { return this->value_; }

        protected:

        private:

            ShipPart* parent_;          //!< Pointer to the ShipPart this event belongs to
            bool valid_;                //!< Whether this event is valid or not.

            std::string targetType_;    //!< The type of the target. (ship weapon engine)
            std::string targetName_;    //!< The name of the target.
            TargetParam targetParam_;   //!< The parameter to be modified
            std::string operation_;     //!< The operation to be applied
            float value_;               //!< The value used to do the operation
            std::string message_;       //!< The message which is shown in chat when the event is executed.



    }; // tolua_export
} // tolua_export

#endif /* _PartDestructionEvent_H__ */
