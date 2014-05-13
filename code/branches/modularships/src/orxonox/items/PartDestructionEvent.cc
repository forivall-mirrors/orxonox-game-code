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

#include "PartDestructionEvent.h"

#include <algorithm>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "network/NetworkFunction.h"
#include "Item.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/ModularSpaceShip.h"
#include "gametypes/Gametype.h"


namespace orxonox
{
    RegisterClass(PartDestructionEvent);

    PartDestructionEvent::PartDestructionEvent(Context* context)
        : Item(context)
    {
        RegisterObject(PartDestructionEvent);
        this->setValid(true);
    }

    PartDestructionEvent::~PartDestructionEvent()
    {

    }

    void PartDestructionEvent::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PartDestructionEvent, XMLPort, xmlelement, mode);

        XMLPortParam(PartDestructionEvent, "targetType", setTargetType, getTargetType, xmlelement, mode).defaultValues("NULL");
        XMLPortParam(PartDestructionEvent, "targetName", setTargetName, getTargetName, xmlelement, mode).defaultValues("NULL");
        XMLPortParam(PartDestructionEvent, "targetParam", setTargetParam, getTargetParam, xmlelement, mode).defaultValues("NULL");
        XMLPortParam(PartDestructionEvent, "operation", setOperation, getOperation, xmlelement, mode).defaultValues("NULL");
        XMLPortParam(PartDestructionEvent, "value", setEventValue, getEventValue, xmlelement, mode).defaultValues(0);

        /*
        XMLPortParam(PartDestructionEvent, "health", setHealth, getHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(PartDestructionEvent, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(PartDestructionEvent, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);
        */
    }

    void PartDestructionEvent::execute()
    {
        // Do not execute if this event is invalid
        if(!isValid())
        {
            orxout(internal_warning) << "Attempted to execute an invalid PartDestructionEvent!" << endl;
            return;
        }

        if (this->targetType_ == "ship")
        {
            switch (this->targetParam_) {
            case shieldhealth:
                this->parent_->getParent()->setShieldHealth(operate(this->parent_->getParent()->getShieldHealth()));
                break;
            default:
                break;
            }
            this->setValid(false);
            return;
        }
    }

    void PartDestructionEvent::setParent(ShipPart* part)
    {
        this->parent_ = part;
    }

    void PartDestructionEvent::setTargetType(std::string type)
    {
        // ship engine weapon
        if ((type == "ship") || (type == "engine") || (type == "weapon"))
        {
            this->targetType_ = type;
            return;
        }

        // Error, if invalid target-type was entered.
        orxout(internal_warning) << "\"" << type << "\" is not a valid target-type for a PartDestructionEvent. Valid types are: ship engine weapon" << endl;
        this->setValid(false);
        return;
    }

    void PartDestructionEvent::setTargetName(std::string name)
    {
        // Error, if the target-type is "weapon" or "engine", but the name of said target was not defined.
        if (((this->targetType_ == "weapon") || (this->targetType_ == "engine")) && (name == "NULL"))
        {
            orxout(internal_warning) << "The target-name of a PartDestructionEvent with target-type \"" << this->targetType_ << "\" needs to be defined!" << endl;
            return;
        }

        this->targetName_ = name;
    }

    void PartDestructionEvent::setTargetParam(std::string param)
    {
        if (this->targetType_ == "NULL")
        {
            orxout(internal_warning) << "No valid target-type defined. Cannot set target-param for this PartDestructionEvent." << endl;
            this->setValid(false);
            return;
        }

        // ship: shieldhealth maxshieldhealth shieldabsorption shieldrechargerate

        // engine:

        // weapon:

        if (this->targetType_ == "ship")
        {
            if (param == "shieldhealth")
            {
                this->targetParam_ = shieldhealth;
                return;
            }

            orxout(internal_warning) << "\"" << param << "\" is not a valid target-param for a PartDestructionEvent with target-type \"ship\". Valid types are: shieldhealth maxshieldhealth shieldabsorption shieldrechargerate" << endl;
            return;
        }

        orxout(internal_warning) << "No valid target-param defined. The chosen param is either invalid or not available for this target-type." << endl;
        this->setValid(false);
    }

    void PartDestructionEvent::setOperation(std::string operation)
    {
        // * + - destroy
        if ((operation == "*") || (operation == "+") || (operation == "-") || (operation == "destroy"))
        {
            this->operation_ = operation;
            return;
        }
        this->operation_ = "NULL";
        orxout(internal_warning) << "\"" << operation << "\" is not a valid operation for a PartDestructionEvent. Valid operations are: * + - destroy" << endl;
    }

    float PartDestructionEvent::operate(float input)
    {
        if (this->operation_ == "*")
            return input * this->value_;
        if (this->operation_ == "+")
            return input + this->value_;
        if (this->operation_ == "-")
            return input - this->value_;
        if (this->operation_ == "destroy")
        {
            return 0;
        }
        return 0;
    }

    void PartDestructionEvent::setEventValue(float value)
    {
        this->value_ = value;
    }
}
