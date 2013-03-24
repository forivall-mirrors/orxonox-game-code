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

#ifndef _WANDiscoverable_H__
#define _WANDiscoverable_H__

#include "NetworkPrereqs.h"
#include "core/class/OrxonoxClass.h"
#include "core/CoreIncludes.h"
#include "MasterServerComm.h"

namespace orxonox
{

  class _NetworkExport WANDiscoverable: public OrxonoxClass
  {
    public:
      /** constructor */
      WANDiscoverable();

      /** destructor */
      ~WANDiscoverable();

      /** \return Address of the master server
       * 
       * Get the master server address 
       */
      std::string getMSAddress()
      { return this->msaddress; }

      /** Function used for the configuration file parameter update */
      void setConfigValues();
      
      /** Function used to set the activity/discoverability */
      void setActivity( bool bActive );

      /** Master server communications object */
      MasterServerComm msc;
      
    private:
      /** Function used to connect to the master server */
      bool connect();
      
      /** Function used to disconnect from the master server */
      void disconnect();
      
      /** master server address */
      std::string msaddress;
      bool        bActive_;

  };

}

#endif // _WANDiscoverable_H__
