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
 *      Fabian 'x3n' Landau (original)
 *   Co-authors:
 *      Sandro 'smerkli' Merkli (copied and adapted to WAN)
 *
 */

#ifndef WANDISCOVERY_H
#define WANDISCOVERY_H

#include "NetworkPrereqs.h"
#include "packet/ServerInformation.h"
#include "util/Singleton.h"
#include "MasterServerComm.h"
#include "MasterServerProtocol.h"

#include <vector>


// tolua_begin
namespace orxonox
{

  class _NetworkExport WANDiscovery
// tolua_end
    : public Singleton<WANDiscovery>
  { // tolua_export
    friend class Singleton<WANDiscovery>;
    public:
      /** constructor */
      WANDiscovery();

      /** destructor */
      ~WANDiscovery();

      /** ask server for server list  */
      void discover(); // tolua_export

      /** \param index Index to get the name of 
       * \return The name of the server
       * 
       * Get the name of the server at index index. 
       */
      std::string getServerListItemName( unsigned int index ); // tolua_export

      /** \param index Index to get the IP of 
       * \return The IP of the server
       * 
       * Get the IP of the server at index index. 
       */
      std::string getServerListItemIP( unsigned int index ); // tolua_export

      /** \return an instance of WANDiscovery
       * 
       * Create and return an instance of WANDiscovery.
       */
      static WANDiscovery& getInstance() { return Singleton<WANDiscovery>::getInstance(); } // tolua_export
    
      int rhandler( char *addr, ENetEvent *ev );
      
    private:
      /** Singleton pointer */
      static WANDiscovery* singletonPtr_s;

      /** Master server communications object */
      MasterServerComm msc;

      /** game server list */
      std::vector<packet::ServerInformation> servers_;

  }; // tolua_export

} // tolua_export

#endif // WANDISCOVERY_H
