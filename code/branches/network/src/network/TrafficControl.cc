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

#include "TrafficControl.h"

#include <cassert>

namespace network {

TrafficControl *TraffiControl::instance_=0;

/**
 * @brief Constructor: assures that only one reference will be created and sets the pointer
 */
TrafficControl::TrafficControl()
{
  assert(instance_=0);
  instance_=this;


}


/**
 * @brief Destructor: resets the instance pointer to 0
 */
TrafficControl::~TrafficControl()
{
  instance_=0;
}


/*
void bvlabla(vector *a){
//sort a
vector *cache;
cache = new vector<unsigned int>(*a);
return a;
}
*/


}//namespace network
