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
#ifndef NETWORKCLASSID_H
#define NETWORKCLASSID_H

#include "../NetworkPrereqs.h"

#include <string>

#include "Packet.h"

namespace orxonox {
namespace packet {

/**
	@author
*/
class _NetworkExport ClassID : public Packet
{
public:
  ClassID( unsigned int classID, std::string className );
  ClassID( uint8_t* data, unsigned int clientID );
  ~ClassID();

  inline unsigned int getSize() const;
  bool process();

  unsigned int getClassID();
  unsigned int getClassNameLength(){ return classNameLength_; }
  unsigned char *getClassName();
private:
  uint32_t classNameLength_;
};

} //namespace packet
} //namespace orxonox

#endif
