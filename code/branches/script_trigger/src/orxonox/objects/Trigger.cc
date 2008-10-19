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

#include "OrxonoxStableHeaders.h"
#include "Trigger.h"

#include <OgreBillboard.h>
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/XMLPort.h"

namespace orxonox
{

  ConsoleCommand(Trigger, setVisibility, AccessLevel::Debug, false).setDefaultValues(0);

  CreateFactory(Trigger);

  Trigger::Trigger()
  {
    RegisterObject(Trigger);

    mode_ = TM_EventTriggerAND;
    bActive_ = false;
    bInvertMode_ = false;
    delay_ = 0.0;
    bTriggered_ = false;
    bUpdating_ = false;
    remainingActivations_ = -1;
    bStayOn_ = false;
    latestState_ = 0x0;

    debugBillboard_.setBillboardSet("Examples/Flare", ColourValue(1.0, 0.0, 0.0), 1);
    this->getNode()->attachObject(debugBillboard_.getBillboardSet());
  }

  Trigger::~Trigger()
  {
  }

  void Trigger::init()
  {
    this->setVisibility(true);
    timeSinceLastEvent_ = delay_;
  }

  void Trigger::setVisibility(bool bVisible)
  {
    if(bVisible)
      this->setScale(2,2,2);
    else
      this->setScale(0,0,0);
  }

  const Trigger* Trigger::getTrigger(unsigned int index) const
  {
    if (children_.size() <= index)
      return NULL;

    std::set<Trigger*>::iterator it;
    it = children_.begin();

    for ( unsigned int i = 0; i != index; i++ )
    {
      it++;
    }
    return *it;
  }

  void Trigger::tick(float dt)
  {

    bool newTriggered;
    newTriggered = this->isTriggered();

    // check if new triggering event is really new
    if((this->latestState_ & 0x1) != newTriggered)
    {
      // create new state
      if(newTriggered)
      {
        latestState_ |= 1; // set trigger bit
        this->switchState();
      }
      else
      {
        latestState_ &= 0xFE; // set trigger bit
        if (this->bStayOn_)
          this->storeState();
        else
          this->switchState();
      }
    }

    if(remainingTime_ > 0.0)
    {
      remainingTime_ -= dt;
      // only increase when acctually waiting for a state in the queue
      if(timeSinceLastEvent_ >= 0.0)
        timeSinceLastEvent_ += dt;
    }

    while(remainingTime_ <= 0.0 && stateChanges_.size() > 0)
    {
      // time ran out, change state to new one
      char newState = stateChanges_.front().second;
      bTriggered_ = (newState & 0x1);
      bActive_ = newState & 2;
      this->stateChanges_.pop();
      if(stateChanges_.size() != 0)
        remainingTime_ = stateChanges_.front().first;
      else
        timeSinceLastEvent_ = delay_;
    }



    if (bTriggered_ && bActive_)
      this->setBillboardColour(ColourValue(0.5, 1.0, 0.0));
    else if (!bTriggered_ && bActive_)
      this->setBillboardColour(ColourValue(0.0, 1.0, 0.0));
    else if (bTriggered_ && !bActive_)
      this->setBillboardColour(ColourValue(1.0, 0.5, 0.0));
    else
      this->setBillboardColour(ColourValue(1.0, 0.0, 0.0));
    bUpdating_ = false;
  }

  void Trigger::setBillboardColour(ColourValue colour)
  {
    this->debugBillboard_.getBillboardSet()->getBillboard(0)->setColour(colour);
  }

  void Trigger::storeState()
  {
    // put state change into queue
    this->stateChanges_.push(std::pair<float,char>(timeSinceLastEvent_, latestState_));
    // reset time since last event
    timeSinceLastEvent_ = 0.0;

    if(this->stateChanges_.size() == 1)
      remainingTime_ = stateChanges_.front().first;
  }

  bool Trigger::isTriggered(TriggerMode mode)
  {
    if(bUpdating_)
      return bTriggered_;

    if( children_.size() != 0 )
    {
      bUpdating_ = true;
      bool returnval = false;
      switch(mode)
      {
        case TM_EventTriggerAND:
          returnval = checkAnd();
          break;
        case TM_EventTriggerOR:
          returnval = checkOr();
          break;
        case TM_EventTriggerXOR:
          returnval = checkXor();
          break;
        default:
          returnval = false;
          break;
      }
      if(bInvertMode_)
        return !returnval;
      else
        return returnval;
    }
    return true;
  }

  void Trigger::setDelay(float delay)
  {
    this->delay_ = delay;
  }

  void Trigger::setMode(std::string modeName)
  {
    if (modeName == "and")
      setMode(TM_EventTriggerAND);
    else if (modeName == "or")
      setMode(TM_EventTriggerOR);
    else if (modeName == "xor")
      setMode(TM_EventTriggerXOR);
  }

  void Trigger::XMLPort(Element& xmlelement, XMLPort::Mode mode)
  {
    WorldEntity::XMLPort(xmlelement, mode);

    XMLPortParamLoadOnly(Trigger, "delay", setDelay, xmlelement, mode);
    XMLPortParamLoadOnly(Trigger, "stayOn", setStayOn, xmlelement, mode);
    XMLPortParamLoadOnly(Trigger, "activations", setActivations, xmlelement, mode);
    //XMLPortParamLoadOnlyTemplate(Trigger, "mode", setMode, xmlelement, mode, std::string);

    XMLPortObject(Trigger, Trigger, "", addTrigger, getTrigger, xmlelement, mode, false, true);

    this->init();
  }

  void Trigger::addTrigger(Trigger* trig)
  {
    if (this != trig)
      this->children_.insert(trig);
  }

  bool Trigger::switchState()
  {
    if ( remainingActivations_ <= -1 || this->latestState_ & 2 || remainingActivations_ > 0)
    {
      this->latestState_ ^= 2; // toggle state bit
      // increase activation count
      if (this->latestState_ & 2) remainingActivations_--;
      this->storeState();

      return true;
    }
    return false;
  }


  bool Trigger::checkAnd()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if(!((*it)->isActive()))
        return false;
    }
    return true;
  }

  bool Trigger::checkOr()
  {
    std::set<Trigger*>::iterator it;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if((*it)->isActive())
        return true;
    }
    return false;
  }

  bool Trigger::checkXor()
  {
    std::set<Trigger*>::iterator it;
    bool test = false;
    for(it = this->children_.begin(); it != this->children_.end(); it++)
    {
      if(test && (*it)->isActive())
        return false;
      if((*it)->isActive())
        test = true;
    }
    return test;
  }

}
