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
 *      Oliver Scheuss (original)
 *
 */

#include "WANDiscoverable.h"

#include <enet/enet.h>
#include <cstring>

#include "MasterServerProtocol.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"


namespace orxonox
{

  WANDiscoverable::WANDiscoverable(): bActive_(false)
  {
    /* debugging output */
    orxout(verbose, context::master_server) << "Creating WANDiscoverable." << endl;
  
    /* register object in orxonox */
    RegisterObject(WANDiscoverable);

    /* check for the masterserver address option in orxonox.ini */
    this->setConfigValues();
    
  }

  void WANDiscoverable::setConfigValues()
  {
    /* update msaddress string from orxonox.ini config file, if it 
     * has changed. 
     */
    SetConfigValueExternal(msaddress, "WANDiscovery", "msaddress", "orxonox.net");
//     SetConfigValue( msaddress, "orxonox.net");
  } 

  WANDiscoverable::~WANDiscoverable()
  {
    if( this->bActive_ )
      this->disconnect();
  }
  
  void WANDiscoverable::setActivity(bool bActive)
  {
    if( bActive==this->bActive_ )
      return;
    
    if( bActive )
    {
      if( this->connect() )
        this->bActive_ = true;
    }
    else
    {
      this->disconnect();
      this->bActive_ = false;
    }
  }
  
  bool WANDiscoverable::connect()
  {
    /* initialize it and see if it worked */
    if( msc.initialize() )
    {
      orxout(internal_error, context::master_server) << "Could not initialize master server communications!" << endl;
      return false;
    }
    
    /* connect and see if it worked */
    if( msc.connect( this->msaddress.c_str(), ORX_MSERVER_PORT ) )
    {
      orxout(internal_error, context::master_server) << "Could not connect to master server at " 
                 << this->msaddress << endl;
      return false;
    }
                 
    /* debugging output */
    orxout(verbose, context::master_server) << "Initialization of WANDiscoverable complete." << endl;
    
    
    // Now register the server at the master server
    this->msc.sendRequest( MSPROTO_GAME_SERVER " " MSPROTO_REGISTER_SERVER );
    
    return true;
  }

  void WANDiscoverable::disconnect()
  {
    this->msc.sendRequest( MSPROTO_GAME_SERVER " " MSPROTO_SERVERDC );
    msc.disconnect();
  }



  
} // namespace orxonox
