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
#include "core/GameMode.h"
#include "GamestateHandler.h"
#include "Host.h"
#include "util/OrxAssert.h"

namespace orxonox {

std::map<uint32_t, packet::FunctionCalls*> FunctionCallManager::sPeerMap_;
std::vector<std::pair<FunctionCall, std::pair<uint32_t, uint32_t> > > FunctionCallManager::sIncomingFunctionCallBuffer_;


void FunctionCallManager::addCall(uint32_t functionID, uint32_t objectID, uint32_t peerID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)
{
  if(sPeerMap_.find(peerID)==sPeerMap_.end())
  {
    FunctionCallManager::sPeerMap_[peerID] = new packet::FunctionCalls;
    FunctionCallManager::sPeerMap_[peerID]->setPeerID(peerID);
  }
  FunctionCallManager::sPeerMap_[peerID]->addCall(functionID, objectID, mt1, mt2, mt3, mt4, mt5);
}

// Send calls

void FunctionCallManager::sendCalls(orxonox::Host* host)
{
  std::map<uint32_t, packet::FunctionCalls*>::iterator it;
  for (it = FunctionCallManager::sPeerMap_.begin(); it != FunctionCallManager::sPeerMap_.end(); ++it )
  {
    assert(!FunctionCallManager::sPeerMap_.empty());
    it->second->send(host);
  }
  FunctionCallManager::sPeerMap_.clear();
}

void FunctionCallManager::bufferIncomingFunctionCall(const orxonox::FunctionCall& fctCall, uint32_t minGamestateID, uint32_t peerID)
{
  FunctionCallManager::sIncomingFunctionCallBuffer_.push_back( std::make_pair(fctCall, std::make_pair(minGamestateID, peerID)));
}

void FunctionCallManager::processBufferedFunctionCalls()
{
  std::vector<std::pair<FunctionCall, std::pair<uint32_t, uint32_t> > >::iterator it = FunctionCallManager::sIncomingFunctionCallBuffer_.begin();
  while( it!=FunctionCallManager::sIncomingFunctionCallBuffer_.end() )
  {
    OrxAssert( Host::getActiveInstance(), "No Host class existing" );
    if( it->second.first <= Host::getActiveInstance()->getLastReceivedGamestateID(it->second.second) && it->first.execute() )
      FunctionCallManager::sIncomingFunctionCallBuffer_.erase(it);
    else
    {
      ++it;
    }
  }
}


} //namespace orxonox
