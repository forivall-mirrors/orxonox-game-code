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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#ifndef _FunctionCall_H__
#define _FunctionCall_H__

#include "network/NetworkPrereqs.h"
#include "util/UtilPrereqs.h"
#include "util/MultiType.h"

#include <cassert>
#include <vector>

namespace orxonox {
/**
    @author
*/


class _NetworkExport FunctionCall
{
public:
  FunctionCall();
  ~FunctionCall();

  inline unsigned int getSize() const { return this->size_; }
  bool execute();

  void setCallStatic( uint32_t networkID, const MultiType* mt1=0, const MultiType* mt2=0, const MultiType* mt3=0, const MultiType* mt4=0, const MultiType* mt5=0);
  void setCallMember( uint32_t networkID, uint32_t objectID, const MultiType* mt1=0, const MultiType* mt2=0, const MultiType* mt3=0, const MultiType* mt4=0, const MultiType* mt5=0);
  
  void saveData( uint8_t*& mem );
  void loadData( uint8_t*& mem );
private:
  uint32_t                  nrOfArguments_;
  bool                      bIsStatic_;
  uint32_t                  functionID_;
  uint32_t                  objectID_;
  uint32_t                  size_;
  std::vector<MultiType>    arguments_;
};

} //namespace orxonox

#endif /* _FunctionCalls_H__ */
