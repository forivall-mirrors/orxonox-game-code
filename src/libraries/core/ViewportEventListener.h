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

#ifndef _ViewportEventListener_H__
#define _ViewportEventListener_H__

#include "CorePrereqs.h"

#include "util/OgreForwardRefs.h"
#include "object/Listable.h"

namespace orxonox
{
    class _CoreExport ViewportEventListener : virtual public Listable
    {
        public:
            virtual void cameraChanged(Ogre::Viewport* viewport, Ogre::Camera* oldCamera) {}

        protected:
            ViewportEventListener();
            virtual ~ViewportEventListener() {}
    };
}

#endif /* _ViewportEventListener_H__ */
