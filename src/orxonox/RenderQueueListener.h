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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ORenderQueueListener_H__
#define _ORenderQueueListener_H__

#include "OrxonoxPrereqs.h"

#include <OgreRenderQueueListener.h>

namespace orxonox
{
    /* Defining some render queue groups based around the main render queue to enable a stenicl buffer based glow effect */
    enum RenderQueueGroupID
    {
        RENDER_QUEUE_MAIN = Ogre::RENDER_QUEUE_MAIN,
        RENDER_QUEUE_STENCIL_OBJECTS = RENDER_QUEUE_MAIN+1,
        RENDER_QUEUE_STENCIL_GLOW = RENDER_QUEUE_MAIN+2,
        RENDER_QUEUE_STENCIL_LAST = RENDER_QUEUE_STENCIL_GLOW
    };

    const int STENCIL_VALUE_FOR_GLOW = 1; //if more than one type of stencil mask is to be used it needs to use another value
    const int STENCIL_FULL_MASK = 0xFFFFFFFF;
    
    /* Deriving from the Ogre RenderQueueListener to define our own handling of the different rendering stages to enable alpha based shader/glow effects */
    class _OrxonoxExport RenderQueueListener : public Ogre::RenderQueueListener
    {
        public:
            virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
            virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
    };
}

#endif /* _ORenderQueueListener_H__ */