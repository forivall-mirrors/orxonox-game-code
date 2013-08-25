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

#ifndef _ClientConnectionListener_H__
#define _ClientConnectionListener_H__

#include "NetworkPrereqs.h"
#include "core/object/Listable.h"

namespace orxonox
{
    class _NetworkExport ClientConnectionListener : virtual public Listable
    {
        public:
            ClientConnectionListener();
            virtual ~ClientConnectionListener() {}

            static void broadcastClientConnected(unsigned int clientID);
            static void broadcastClientDisconnected(unsigned int clientID);

            virtual void clientConnected(unsigned int clientID) = 0;
            virtual void clientDisconnected(unsigned int clientID) = 0;

        protected:
//             void getConnectedClients();
    };
}


#endif /* _ClientConnectionListener_H__ */
