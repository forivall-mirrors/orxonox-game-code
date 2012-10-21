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

#include "RenderQueueListener.h"

#include <OgreRoot.h>
#include <OgreRenderQueueListener.h>

namespace orxonox
{
    void RenderQueueListener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
    {
        if (queueGroupId == RENDER_QUEUE_STENCIL_OBJECTS)
        { 
            Ogre::RenderSystem * renderSystem = Ogre::Root::getSingleton().getRenderSystem(); 

            renderSystem->clearFrameBuffer(Ogre::FBT_STENCIL); 
            renderSystem->setStencilCheckEnabled(true); 
            renderSystem->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
                STENCIL_VALUE_FOR_GLOW, STENCIL_FULL_MASK, 
                Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
        } 
        if (queueGroupId == RENDER_QUEUE_STENCIL_GLOW)
        { 
              Ogre::RenderSystem * renderSystem = Ogre::Root::getSingleton().getRenderSystem(); 
              renderSystem->setStencilCheckEnabled(true); 
              renderSystem->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL,
                  STENCIL_VALUE_FOR_GLOW, STENCIL_FULL_MASK, 
                  Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
        }
    }
   
    void RenderQueueListener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
    {
        if (queueGroupId == RENDER_QUEUE_STENCIL_LAST) 
        {
            Ogre::RenderSystem * renderSystem = Ogre::Root::getSingleton().getRenderSystem(); 
            renderSystem->setStencilCheckEnabled(false); 
            renderSystem->setStencilBufferParams(); 
        }
    }
}