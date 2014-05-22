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
#include "items/Engine.h"
#include "gametypes/Gametype.h"
#include "chat/ChatManager.h"


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
        XMLPortParam(PartDestructionEvent, "operation", setOperation, getOperation, xmlelement, mode).defaultValues("NULL");
        XMLPortParam(PartDestructionEvent, "targetParam", setTargetParam, getTargetParam, xmlelement, mode).defaultValues("NULL");
        XMLPortParam(PartDestructionEvent, "value", setEventValue, getEventValue, xmlelement, mode).defaultValues(0);
        XMLPortParam(PartDestructionEvent, "message", setMessage, getMessage, xmlelement, mode).defaultValues("NULL");

        /*
        XMLPortParam(PartDestructionEvent, "health", setHealth, getHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(PartDestructionEvent, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(PartDestructionEvent, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);
        */
    }

    /**
    @brief
        Executes this event.
    */
    void PartDestructionEvent::execute()
    {
        // Do not execute if this event is invalid
        if(!isValid())
        {
            orxout(internal_warning) << "Attempted to execute an invalid PartDestructionEvent!" << endl;
            return;
        }

        // Output the destruction-message to the chat
        if(this->message_ != "NULL")
            ChatManager::message(this->message_);

        // Modify parameters as configured for all cases
        if (this->targetType_ == "ship")
        {
            switch (this->targetParam_) {
            case shieldhealth:
                this->parent_->getParent()->setShieldHealth(operate(this->parent_->getParent()->getShieldHealth()));
                break;
            case boostpower:
                this->parent_->getParent()->setInitialBoostPower(operate(this->parent_->getParent()->getInitialBoostPower()));
                break;
            case boostpowerrate:
                this->parent_->getParent()->setBoostPowerRate(operate(this->parent_->getParent()->getBoostPowerRate()));
                break;
            case rotationthrust:
                this->parent_->getParent()->setRotationThrust(operate(this->parent_->getParent()->getRotationThrust()));
                break;
            default:
                break;
            }
            this->setValid(false);
            return;
        }

        if (this->targetType_ == "engine")
        {
            switch (this->targetParam_) {
            case null:
                this->parent_->getParent()->removeEngine(this->parent_->getParent()->getEngineByName(targetName_));
                break;
            case boostfactor:
                this->parent_->getParent()->getEngineByName(targetName_)->setBoostFactor(operate(this->parent_->getParent()->getEngineByName(targetName_)->getBoostFactor()));
                break;
            case speedfront:
                this->parent_->getParent()->getEngineByName(targetName_)->setMaxSpeedFront(operate(this->parent_->getParent()->getEngineByName(targetName_)->getMaxSpeedFront()));
                break;
            case accelerationfront:
                this->parent_->getParent()->getEngineByName(targetName_)->setAccelerationFront(operate(this->parent_->getParent()->getEngineByName(targetName_)->getAccelerationFront()));
                break;
            default:
                break;
            }
            this->setValid(false);
            return;
        }

        if (this->targetType_ == "part")
        {
            switch (this->targetParam_) {
            case null:
                if (!this->parent_->getParent()->getShipPartByName(targetName_))
                    return;
                this->parent_->getParent()->getShipPartByName(targetName_)->setEventExecution(false);
                this->parent_->getParent()->killShipPart(targetName_);
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

    /**
    @brief
        Set type of the target
    @param param
        The desired target-type as string. Valid target-types: ship engine weapon
    */
    void PartDestructionEvent::setTargetType(std::string type)
    {
        if ((type == "ship") || (type == "engine") || (type == "weapon") || (type == "part"))
        {
            this->targetType_ = type;
            return;
        }

        // Error, if invalid target-type was entered.
        orxout(internal_warning) << "\"" << type << "\" is not a valid target-type for a PartDestructionEvent. Valid types are: ship engine weapon part" << endl;
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

    /**
    @brief
        Set the operation to be applied.
    @param param
        The desired parameter as string. Valid parameters: c.f. @ref orxnox::PartDestructionEvent::TargetParam
    */
    void PartDestructionEvent::setTargetParam(std::string param)
    {
        // A target-type needs to be defined in order to choose a parameter.
        if (this->targetType_ == "NULL")
        {
            orxout(internal_warning) << "No valid target-type defined. Cannot set target-param for this PartDestructionEvent." << endl;
            this->setValid(false);
            return;
        }

        // engine: NULL boostfactor speedfront accelerationfront
        if (this->targetType_ == "engine")
        {
            if (param == "NULL")
            {
                this->targetParam_ = null;
                return;
            }
            if (param == "boostfactor")
            {
                this->targetParam_ = boostfactor;
                return;
            }
            if (param == "speedfront")
            {
                this->targetParam_ = speedfront;
                return;
            }
            if (param == "accelerationfront")
            {
                this->targetParam_ = accelerationfront;
                return;
            }

            orxout(internal_warning) << "\"" << param << "\" is not a valid target-param for a PartDestructionEvent with target-type \"engine\". Valid types are: boostfactor speedfront accelerationfront" << endl;
            return;
        }

        // weapon:

        // ship: shieldhealth (maxshieldhealth shieldabsorption shieldrechargerate) boostpower boostpowerrate rotationthrust
        if (this->targetType_ == "ship")
        {
            if (param == "shieldhealth")
            {
                this->targetParam_ = shieldhealth;
                return;
            }
            if (param == "boostpower")
            {
                this->targetParam_ = boostpower;
                return;
            }
            if (param == "boostpowerrate")
            {
                this->targetParam_ = boostpowerrate;
                return;
            }
            if (param == "rotationthrust")
            {
                this->targetParam_ = rotationthrust;
                return;
            }

            orxout(internal_warning) << "\"" << param << "\" is not a valid target-param for a PartDestructionEvent with target-type \"ship\". Valid types are: shieldhealth maxshieldhealth shieldabsorption shieldrechargerate boostpower boostpowerrate rotationthrust" << endl;
            return;
        }

        if (this->targetType_ == "part")
        {
            if (param == "NULL")
            {
                this->targetParam_ = null;
                return;
            }

            orxout(internal_warning) << "\"" << param << "\" is not a valid target-param for a PartDestructionEvent with target-type \"part\". Valid types are: NULL (set operation to \"destroy\")" << endl;
            return;
        }

        orxout(internal_warning) << "No valid target-param defined. The chosen param is either invalid or not available for this target-type." << endl;
        this->setValid(false);
    }

    /**
    @brief
        Set the operation to be applied.
    @param operation
        The desired operator as string. Valid operators: * + - destroy
    */
    void PartDestructionEvent::setOperation(std::string operation)
    {
        // * + - destroy
        if ((operation == "*") || (operation == "+") || (operation == "-") || (operation == "set") || (operation == "destroy"))
        {
            this->operation_ = operation;
            return;
        }
        this->operation_ = "NULL";
        orxout(internal_warning) << "\"" << operation << "\" is not a valid operation for a PartDestructionEvent. Valid operations are: * + - set destroy" << endl;
    }

    /**
    @brief
        Set the message to be shown upon execution of the vent.
    @param msg
        The desired message as string.
    */
    void PartDestructionEvent::setMessage(std::string msg)
    {
        this->message_ = msg;
    }


    /**
    @brief
        Apply the configured operation and value to an input.
    @param input
        The value which should be modified
    @return
        Returns the product / sum / difference of input and configured value,
        the configured value if the operation is "set",
        or 0 if the operation is "destroy"
    */
    float PartDestructionEvent::operate(float input)
    {
        if (this->operation_ == "*")
            return input * this->value_;
        if (this->operation_ == "+")
            return input + this->value_;
        if (this->operation_ == "-")
            return input - this->value_;
        if (this->operation_ == "set")
            return this->value_;
        if (this->operation_ == "destroy")
        {
            return 0;
        }
        return 0;
    }

    /**
    @brief
        Sets the value applied with the chosen operation.
    @param value
        The value as float.
    */
    void PartDestructionEvent::setEventValue(float value)
    {
        this->value_ = value;
    }
}
