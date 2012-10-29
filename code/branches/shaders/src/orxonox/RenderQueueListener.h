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

/**
  @file RenderQueueListener.h
  @brief Definition of the RenderQueueListener class.
*/

#ifndef _ORenderQueueListener_H__
#define _ORenderQueueListener_H__

#include "OrxonoxPrereqs.h"

#include <OgreRenderQueueListener.h>

namespace orxonox
{
    enum RenderQueueGroupID //!< these are IDs for render queues that are executed just after the main render queue defined by Ogre, we need more than one for Stencil Glow
    {
        RENDER_QUEUE_MAIN = Ogre::RENDER_QUEUE_MAIN, //reference to the main render queue
        RENDER_QUEUE_STENCIL_OBJECTS = RENDER_QUEUE_MAIN+1,
        RENDER_QUEUE_STENCIL_GLOW = RENDER_QUEUE_MAIN+2,
        RENDER_QUEUE_STENCIL_LAST = RENDER_QUEUE_STENCIL_GLOW //this is a reference to the last render queue to be affected by stencil glow effects
    };

    const int STENCIL_VALUE_FOR_GLOW = 1; //!< this is a reference value for our mask, 
                                          //!< if more than one type of stencil mask is to be used, each of them need their own value
    const int STENCIL_FULL_MASK = 0xFFFFFFFF; //!< this is a reference mask used in our stencil buffer
    
    /**
    @brief
        This class derives from the Ogre-Class RenderQueueListener and provides a way to define new rendering stages to enable use of e.g. stencil buffers
        to increase the number of shader effects we can create. Especially important for shader-based alpha blending.

    @author
        David 'davidsa' Salvisberg
    */
    class _OrxonoxExport RenderQueueListener : public Ogre::RenderQueueListener
    {
        public:
            /**
            @brief
                This function is called just before a RenderQueueGroup is rendered, this function is called by Ogre automatically with the correct parameters.
                
                In this case we use it to set the stencil buffer parameters of the render system
            */
            virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
            /**
            @brief
                This function is called just after a RenderQueueGroup has been rendered, this function is called by Ogre automatically with the correct parameters.
                
                in this case we use it to unset the stencil buffer parameters, so the rest of the render queue is unaffected by it.
            */
            virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
    };
}

#endif /* _ORenderQueueListener_H__ */