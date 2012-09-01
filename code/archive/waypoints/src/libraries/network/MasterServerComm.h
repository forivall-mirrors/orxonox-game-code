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
 *      Sandro 'smerkli' Merkli
 *   Co-authors:
 *      ...
 *
 */

#ifndef _MasterServerComm_H__
#define _MasterServerComm_H__

#include "NetworkPrereqs.h"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <enet/enet.h>


namespace orxonox
{
  class _NetworkExport MasterServerComm 
  { 
    public: 
      /** constructor */
      MasterServerComm();

      /** destructor */
      ~MasterServerComm();

      /** \return 0 for success, other for error
       * 
       * Initialize everything for the master server communication 
       */
      int initialize();
      
      void update();


      /** \param address Address to connect to (Host name or IP in text form)
       * \param port Port to connect on 
       * \return 0 for success, other for error 
       * 
       * Connect to the master server with the given address on the given port. 
       */
      int connect( const char *address, unsigned int port );

      /** \return 0 for success, other for error
       * 
       * Disconnect from master server.
       */
      int disconnect( void );

      /** \param data The data to be sent. 
       * \return 0 for success, other for error. 
       * 
       * Send a request to the master server containing data specified in data
       */
      int sendRequest( const char *data );

      /** \param data The data to be sent. 
       * \return 0 for success, other for error. 
       * 
       * Send a request to the master server containing data specified in data
       * (string version)
       */
      int sendRequest( std::string data );

      /** \param callback The callback function to call with data received.
       *  \param delayms Delay in milliseconds.
       * \return 0 for success, other for error
       * 
       * Poll the master server for new data and act accordingly */
      int pollForReply( WANDiscovery* listener, int delayms );

    private:
      /** client handle */
      ENetHost* client;

      /** event data holder */
      ENetEvent event;

      /** address holder */
      ENetAddress address;

      /** peer data holder */
      ENetPeer* peer;
  }; 

} 

#endif /* _MasterServerComm_H__ */
