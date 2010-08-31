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

#include <string>

#include "Packet.h"

namespace network {
namespace packet {

/**
	@author 
*/
class ClassID : public Packet
{
public:
  ClassID( unsigned int classID, std::string className );
  ClassID( unsigned char* data, int clientID );
  ~ClassID();
  
  inline unsigned int getSize() const;
  bool process();
  
  unsigned int getClassID();
  unsigned int getClassNameLength(){ return classNameLength_; }
  unsigned char *getClassName();
private:
  unsigned int classNameLength_;
};

} //namespace packet
} //namespace network

#endif