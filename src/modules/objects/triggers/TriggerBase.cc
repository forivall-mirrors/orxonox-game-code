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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file TriggerBase.cc
    @brief Implementation of the TriggerBase class.
    @ingroup Triggers
*/

#include "TriggerBase.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox {

    // Initialization of some static (magic) variables.
    /*static*/ const int TriggerBase::INF_s = -1;
    /*static*/ const std::string TriggerBase::and_s = "and";
    /*static*/ const std::string TriggerBase::or_s = "or";
    /*static*/ const std::string TriggerBase::xor_s = "xor";

    CreateFactory(TriggerBase);

    /**
    @brief
        Constructor. Registers the object and initializes some values.
    */
    TriggerBase::TriggerBase(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(TriggerBase);

        this->bFirstTick_ = true;

        this->delay_ = 0.0f;
        this->bSwitch_ = false;
        this->bStayActive_ = false;

        this->remainingActivations_ = INF_s;

        this->bInvertMode_ = false;
        this->mode_ = TriggerMode::EventTriggerAND;

        this->parent_ = NULL;

        this->bMultiTrigger_ = false;

        this->setSyncMode(ObjectDirection::None);
    }

    /**
    @brief
        Destructor.
    */
    TriggerBase::~TriggerBase()
    {
        
    }

    /**
    @brief
        Method for creating a TriggerBase object through XML.
        For a detailed description of the parameters please see the class description in the header file.
    */
    void TriggerBase::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TriggerBase, XMLPort, xmlelement, mode);

        XMLPortParam(TriggerBase, "delay", setDelay, getDelay, xmlelement, mode);
        XMLPortParam(TriggerBase, "switch", setSwitch, getSwitch, xmlelement, mode);
        XMLPortParam(TriggerBase, "stayactive", setStayActive, getStayActive, xmlelement, mode);
        XMLPortParam(TriggerBase, "activations", setActivations, getActivations, xmlelement, mode);
        XMLPortParam(TriggerBase, "invert", setInvert, getInvert, xmlelement, mode);
        XMLPortParamTemplate(TriggerBase, "mode", setMode, getModeString, xmlelement, mode, const std::string&);

        XMLPortObject(TriggerBase, TriggerBase, "", addTrigger, getTrigger, xmlelement, mode);
    }

    /**
    @brief
        A method that is executed each tick.
    @param dt
        The duration of the last tick.
    */
    void TriggerBase::tick(float dt)
    {
        SUPER(TriggerBase, tick, dt);
    }

    /**
    @brief
        Set the mode of the trigger.
    @param modeName
        The name of the mode as a string.
    */
    void TriggerBase::setMode(const std::string& modeName)
    {
        if (modeName == TriggerBase::and_s)
            this->setMode(TriggerMode::EventTriggerAND);
        else if (modeName == TriggerBase::or_s)
            this->setMode(TriggerMode::EventTriggerOR);
        else if (modeName == TriggerBase::xor_s)
            this->setMode(TriggerMode::EventTriggerXOR);
        else
            orxout(internal_warning, context::triggers) << "Invalid mode '" << modeName << "' in TriggerBase " << this->getName() << " &(" << this << "). Leaving mode at '" << this->getModeString() << "'." << endl;
    }

    /**
    @brief
        Get the mode of the MultiTrigger.
    @return
        Returns the mode as a string.
    */
    const std::string& TriggerBase::getModeString(void) const
    {
        if (this->mode_ == TriggerMode::EventTriggerAND)
            return TriggerBase::and_s;
        else if (this->mode_ == TriggerMode::EventTriggerOR)
            return TriggerBase::or_s;
        else if (this->mode_ == TriggerMode::EventTriggerXOR)
            return TriggerBase::xor_s;
        else // This can never happen, but the compiler needs it to feel secure.
            return TriggerBase::and_s;
    }

    /**
    @brief
        Adds a trigger as a child to the trigger.
        Beware: Loops are not prevented and potentially very bad, so just don't create any loops.
    @param trigger
        The trigger to be added.
    */
    void TriggerBase::addTrigger(TriggerBase* trigger)
    {
        assert(trigger);
        if (this != trigger)
            this->children_.insert(trigger);
        trigger->addParentTrigger(this);
    }

    /**
    @brief
        Get the child of this trigger at the given index.
    @param index
        The index.
    @return
        Returns a pointer ot the trigger. NULL if no such trigger exists.
    */
    const TriggerBase* TriggerBase::getTrigger(unsigned int index) const
    {
        // If the index is greater than the number of children.
        if (this->children_.size() <= index)
            return NULL;

        std::set<TriggerBase*>::const_iterator it;
        it = this->children_.begin();

        for (unsigned int i = 0; i != index; ++i)
            ++it;

        return (*it);
    }

}
