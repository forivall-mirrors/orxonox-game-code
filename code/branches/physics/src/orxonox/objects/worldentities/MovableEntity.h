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

#ifndef _MovableEntity_H__
#define _MovableEntity_H__

#include "OrxonoxPrereqs.h"

#include "MobileEntity.h"
#include "network/ClientConnectionListener.h"

namespace orxonox
{
    class _OrxonoxExport MovableEntity : public MobileEntity, public network::ClientConnectionListener
    {
        public:
            MovableEntity(BaseObject* creator);
            virtual ~MovableEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();

        private:
            void clientConnected(unsigned int clientID);
            void clientDisconnected(unsigned int clientID);
            void resynchronize();

            inline void processLinearVelocity()
                { this->setVelocity(this->linearVelocity_); }
            inline void processAngularVelocity()
                { this->setAngularVelocity(this->angularVelocity_); }

            void overwritePosition();
            void overwriteOrientation();

            void positionChanged(bool bContinuous);
            void orientationChanged(bool bContinuous);

            Vector3 overwrite_position_;
            Quaternion overwrite_orientation_;
    };
}

#endif /* _MovableEntity_H__ */
