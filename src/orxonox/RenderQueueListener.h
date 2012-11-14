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
#include <OgreHardwareOcclusionQuery.h>

namespace orxonox
{
    enum RenderQueueGroupID //!< these are IDs for render queues that are executed just after the main render queue defined by Ogre, we need more than one for Stencil Glow
    {
        RENDER_QUEUE_MAIN = Ogre::RENDER_QUEUE_MAIN, //reference to the main render queue
        RENDER_QUEUE_STENCIL_OBJECTS = RENDER_QUEUE_MAIN+1,
        RENDER_QUEUE_STENCIL_GLOW = RENDER_QUEUE_MAIN+2,
        RENDER_QUEUE_STENCIL_LAST = RENDER_QUEUE_STENCIL_GLOW, //this is a reference to the last render queue to be affected by stencil glow effects
        RENDER_QUEUE_HOQ = RENDER_QUEUE_STENCIL_LAST+1 //this is where we render the objects for occlusion queries (use transparent material)
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
            RenderQueueListener();
            ~RenderQueueListener();
    
            /**
            @brief
            This function is returning the current pixel count and resets the pixel state if we're ready to do another Hardware Occlusion Query
            
            @return
            current pixel count taken from the last Hardware Occlusion Query
            */
            unsigned int getPixelCount();
            
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
            
        private:
            Ogre::HardwareOcclusionQuery* hardwareOcclusionQuery_; //!< this stores the current instance of the HOQ used in the render system
            unsigned int pixelCount_; //!< this stores the last pixel count returned by the last HOQ in the corresponding render group
            
            enum PixelState //!< enum to distinguish the several HOQ pixel count states
            {
                READY_FOR_RENDER,
                QUERY_STARTED,
                READY_FOR_ACCESS
            };
            
            PixelState pixelState_; //!< this stores the current state of the Hardware Occlusion Query
    };
}

#endif /* _ORenderQueueListener_H__ */