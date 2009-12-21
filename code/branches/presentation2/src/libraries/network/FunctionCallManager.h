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

#ifndef _FunctionCallManager_H__
#define _FunctionCallManager_H__

#include "NetworkPrereqs.h"

#include <map>
#include "util/UtilPrereqs.h"

namespace orxonox {
/**
    @author
*/

class _NetworkExport FunctionCallManager
{
public:
  static void addCallStatic(uint32_t functionID, uint32_t clientID);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4);
  static void addCallStatic(uint32_t functionID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5);

  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4);
  static void addCallMember(uint32_t functionID, uint32_t objectID, uint32_t clientID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5);

  static void sendCalls();

  static std::map<uint32_t, packet::FunctionCalls*> clientMap_;
protected:
  FunctionCallManager();
  ~FunctionCallManager();
};

} //namespace orxonox

#endif /* _FunctionCallManager_H__ */
