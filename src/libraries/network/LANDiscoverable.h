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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _LANDISCOVERABLE_H__
#define _LANDISCOVERABLE_H__

#include "NetworkPrereqs.h"
#include "core/config/Configurable.h"

namespace orxonox
{

  class LANDiscoverable: public Configurable
  {
    public:
      LANDiscoverable();
      virtual ~LANDiscoverable();
      void setActivity( bool bActive );
      void update();
      void updateClientNumber(int clientNumber) {this->clientNumber = clientNumber;}
;
      /** Function used for the configuration file parameter update */
      void setConfigValues();

    private:
      bool            bActive_;
      ENetHost*       host_;
      std::string     ownName;
      int clientNumber;
  };

}

#endif // LANDISCOVERABLE_H
