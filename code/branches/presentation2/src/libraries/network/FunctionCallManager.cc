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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */

#include "FunctionCallManager.h"
#include "packet/FunctionCalls.h"

namespace orxonox {

std::map<uint32_t, packet::FunctionCalls*> FunctionCallManager::clientMap_;

// Static calls

void FunctionCallManager::addCallStatic(uint32_t functionID, uint32_t clientID)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallStatic(functionID);
}
void FunctionCallManager::addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager:: clientMap_[clientID]->addCallStatic(functionID, &mt1);
}
void FunctionCallManager::addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager:: clientMap_[clientID]->addCallStatic(functionID, &mt1, &mt2);
}
void FunctionCallManager::addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager:: clientMap_[clientID]->addCallStatic(functionID, &mt1, &mt2, &mt3);
}
void FunctionCallManager::addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager:: clientMap_[clientID]->addCallStatic(functionID, &mt1, &mt2, &mt3, &mt4);
}
void FunctionCallManager::addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager:: clientMap_[clientID]->addCallStatic(functionID, &mt1, &mt2, &mt3, &mt4, &mt5);
}


// MemberCalls

void FunctionCallManager::addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallMember(functionID, objectID);
}
void FunctionCallManager::addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallMember(functionID, objectID, &mt1);
}
void FunctionCallManager::addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallMember(functionID, objectID, &mt1, &mt2);
}
void FunctionCallManager::addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallMember(functionID, objectID, &mt1, &mt2, &mt3);
}
void FunctionCallManager::addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallMember(functionID, objectID, &mt1, &mt2, &mt3, &mt4);
}
void FunctionCallManager::addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)
{
  if(clientMap_.find(clientID)==clientMap_.end())
  {
    FunctionCallManager::clientMap_[clientID] = new packet::FunctionCalls;
    FunctionCallManager::clientMap_[clientID]->setClientID(clientID);
  }
  FunctionCallManager::clientMap_[clientID]->addCallMember(functionID, objectID, &mt1, &mt2, &mt3, &mt4, &mt5);
}

// Send calls

void FunctionCallManager::sendCalls()
{
  std::map<uint32_t, packet::FunctionCalls*>::iterator it;
  for (it = FunctionCallManager::clientMap_.begin(); it != FunctionCallManager::clientMap_.end(); it++ )
  {
    assert(!FunctionCallManager::clientMap_.empty());
    it->second->send();
  }
  FunctionCallManager::clientMap_.clear();
}


} //namespace orxonox
