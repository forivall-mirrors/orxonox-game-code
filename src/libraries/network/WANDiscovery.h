/*
 *   ORXONOX - the hottest 3D action shooter ever to exist > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc., 51
 *   Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Sandro 'smerkli' Merkli
 *   Co-authors:
 *      Oliver Scheuss (original)
 *
 */

#ifndef _WANDiscovery_H__
#define _WANDiscovery_H__

#include "NetworkPrereqs.h"
#include "packet/ServerInformation.h"
#include "core/CoreIncludes.h"
#include "core/class/OrxonoxClass.h"
#include "core/config/ConfigValueIncludes.h"
#include "MasterServerComm.h"
#include "MasterServerProtocol.h"

#include <vector>

#define WANDISC_MAXTRIES 5

// tolua_begin
namespace orxonox
{

  class _NetworkExport WANDiscovery
// tolua_end
    : public OrxonoxClass
  { // tolua_export
    public:
      /** constructor */
      WANDiscovery(); // tolua_export

      /** destructor */
      ~WANDiscovery();

      /** \return Address of the master server
       * 
       * Get the master server address 
       */
      std::string getMSAddress()
      { return this->msaddress; }

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

      /* todo: might make this private and use getter/setter methods
       * at some later time. 
       */
      /** game server list */
      std::vector<packet::ServerInformation> servers_;

      /** Function used for the configuration file parameter update */
      void setConfigValues();

      /** Master server communications object */
      MasterServerComm msc;

      int rhandler( char *addr, ENetEvent *ev );
      
    private:
      /** master server address */
      std::string msaddress;

  }; // tolua_export

} // tolua_export

#endif // _WANDiscovery_H__
