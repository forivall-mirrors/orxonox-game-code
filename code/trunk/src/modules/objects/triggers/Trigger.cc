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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Trigger.cc
    @brief Implementation of the Trigger class.
    @ingroup NormalTrigger
*/

#include "Trigger.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "core/command/ConsoleCommand.h"
#include "Scene.h"

namespace orxonox
{

  SetConsoleCommand("Trigger", "debugFlares", &Trigger::debugFlares).defaultValues(false);

  CreateFactory(Trigger);

  Trigger::Trigger(BaseObject* creator) : TriggerBase(creator)
  {
    RegisterObject(Trigger);

    this->bActive_ = false;
    this->bTriggered_ = false;
    this->latestState_ = 0x0;

    this->remainingTime_ = 0.0f;
    this->timeSinceLastEvent_ = 0.0f;

//    this->bUpdating_ = false;

    if (this->getScene() && GameMode::showsGraphics())
    {
      this->debugBillboard_.setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);
      this->debugBillboard_.setVisible(false);

      if (this->debugBillboard_.getBillboardSet())
          this->attachOgreObject(this->debugBillboard_.getBillboardSet());
    }

    this->setSyncMode(0x0);
  }

  Trigger::~Trigger()
  {
  }

  void Trigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
  {
    SUPER(Trigger, XMLPort, xmlelement, mode);
  }

  void Trigger::tick(float dt)
  {
    if(this->bFirstTick_)
    {
      this->bFirstTick_ = false;
      this->triggered(false);
    }

    // Check if the object is active (this is NOT Trigger::isActive()!)
    if (!this->BaseObject::isActive())
        return;

    SUPER(Trigger, tick, dt);

    bool newTriggered = this->isTriggered() ^ this->getInvert();

    // check if new triggering event is really new
    if ((this->latestState_ & 0x1) != newTriggered)
    {
      // create new state
      if (newTriggered)
      {
        this->latestState_ |= 1; // set trigger bit to 1
        this->switchState();
      }
      else
      {
        this->latestState_ &= 0xFE; // set trigger bit to 0
        if (!this->getSwitch())
          this->switchState();
      }
    }

    if (this->remainingTime_ > 0.0)
    {
      this->remainingTime_ -= dt;
      // only increase when acctually waiting for a state in the queue
      if (this->timeSinceLastEvent_ >= 0.0)
        this->timeSinceLastEvent_ += dt;
    }

    while (this->remainingTime_ <= 0.0 && this->stateChanges_.size() > 0)
    {
      // time ran out, change state to new one
      char newState = this->stateChanges_.front().second;
      this->bTriggered_ = (newState & 0x1);
      this->bActive_ = newState & 2;
      this->triggered(this->bActive_);
      this->stateChanges_.pop();
      if (this->stateChanges_.size() != 0)
        this->remainingTime_ = this->stateChanges_.front().first;
      else
        this->timeSinceLastEvent_ = this->getDelay();
    }

    if (this->bTriggered_ && this->bActive_)
      this->setBillboardColour(ColourValue(0.5, 1.0, 0.0));
    else if (!this->bTriggered_ && this->bActive_)
      this->setBillboardColour(ColourValue(0.0, 1.0, 0.0));
    else if (this->bTriggered_ && !this->bActive_)
      this->setBillboardColour(ColourValue(1.0, 0.5, 0.0));
    else
      this->setBillboardColour(ColourValue(1.0, 0.0, 0.0));
  }

  void Trigger::triggered(bool bIsTriggered)
  {
    this->fireEvent(bIsTriggered);
  }

  bool Trigger::isTriggered(TriggerMode::Value mode)
  {
//    if (this->bUpdating_)
//      return this->bTriggered_;

//    this->bUpdating_ = true;
    if (this->children_.size() != 0)
    {
      bool returnval = false;

      switch (mode)
      {
        case TriggerMode::EventTriggerAND:
          returnval = checkAnd();
          break;
        case TriggerMode::EventTriggerOR:
          returnval = checkOr();
          break;
        case TriggerMode::EventTriggerXOR:
          returnval = checkXor();
          break;
        default:
          returnval = false;
          break;
      }
//      this->bUpdating_ = false;

      return returnval;
    }
    return true;
  }

  bool Trigger::checkAnd()
  {
    std::set<TriggerBase*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); ++it)
    {
      if (!(*it)->isActive())
        return false;
    }
    return true;
  }

  bool Trigger::checkOr()
  {
    std::set<TriggerBase*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); ++it)
    {
      if ((*it)->isActive())
        return true;
    }
    return false;
  }

  bool Trigger::checkXor()
  {
    std::set<TriggerBase*>::iterator it;
    bool test = false;
    for(it = this->children_.begin(); it != this->children_.end(); ++it)
    {
      if (test && (*it)->isActive())
        return false;
      if ((*it)->isActive())
        test = true;
    }
    return test;
  }

  bool Trigger::switchState()
  {
    if (( (this->latestState_ & 2) && this->getStayActive() && (this->remainingActivations_ <= 0))
     || (!(this->latestState_ & 2)                          && (this->remainingActivations_ == 0)))
      return false;
    else
    {
      this->latestState_ ^= 2; // toggle state bit

      // increase activation count
      if (this->latestState_ & 2 && this->remainingActivations_ > 0)
        this->remainingActivations_--;

      this->storeState();

      return true;
    }
  }

  void Trigger::storeState()
  {
    // put state change into queue
    this->stateChanges_.push(std::pair<float, char>(this->timeSinceLastEvent_, this->latestState_));
    // reset time since last event
    this->timeSinceLastEvent_ = 0.0;

    if (this->stateChanges_.size() == 1)
      this->remainingTime_ = this->stateChanges_.front().first;
  }

  void Trigger::delayChanged(void)
  {
    this->timeSinceLastEvent_ = this->getDelay();
  }

  void Trigger::debugFlares(bool bVisible)
  {
    for (ObjectList<Trigger>::iterator it = ObjectList<Trigger>::begin(); it != ObjectList<Trigger>::end(); ++it)
      it->setVisible(bVisible);
  }

  void Trigger::setBillboardColour(const ColourValue& colour)
  {
    this->debugBillboard_.setColour(colour);
  }

  void Trigger::changedVisibility()
  {
    SUPER(Trigger, changedVisibility);

    this->debugBillboard_.setVisible(this->isVisible());
  }
}
