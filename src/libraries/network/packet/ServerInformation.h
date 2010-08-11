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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "../NetworkPrereqs.h"

#include <string>

#ifndef SERVERINFORMATION_H
#define SERVERINFORMATION_H

namespace orxonox
{
  namespace packet
  {

    class ServerInformation
    {
      public:
        ServerInformation();
        ServerInformation(ENetEvent* event);
        ~ServerInformation();
        
        void          send( ENetPeer* peer );
        std::string   getServerIP() { return this->serverIP_; }
        std::string   getServerName() { return this->serverName_; }
        void          setServerName(std::string name) { this->serverName_ = name; }
        uint32_t      getServerRTT() { return this->serverRTT_; }
        
      private:
        std::string   serverName_;
        std::string   serverIP_;
        uint32_t      serverRTT_;
    };

  }
}

#endif // SERVERINFORMATION_H
