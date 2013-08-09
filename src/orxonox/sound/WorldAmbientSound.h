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

#ifndef _WorldAmbientSound_H__
#define _WorldAmbientSound_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox
{
    /**
        @brief This class is used to play and control ambient sound in a level file. See AmbientSound for the
        actual implementation of the background music player.
    */
    class _OrxonoxExport WorldAmbientSound : public BaseObject, public Synchronisable
    {
        public:
            WorldAmbientSound(Context* context);
            virtual ~WorldAmbientSound();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedActivity();

            void play();

        private:
            void registerVariables();

            AmbientSound* ambientSound_;
    };
}

#endif /* _WorldAmbientSound_H__ */
