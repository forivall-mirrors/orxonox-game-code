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
  @file RenderQueueListener.cc
  @brief Implementation of the RenderQueueListener class.
*/

#include "RenderQueueListener.h"

#include <OgreRoot.h>
#include <OgreRenderQueueListener.h>
#include <OgreHardwareOcclusionQuery.h>

namespace orxonox
{
    RenderQueueListener::RenderQueueListener() : pixelCount_(0), pixelState_(RenderQueueListener::READY_FOR_RENDER)
    {
        hardwareOcclusionQuery_ = Ogre::Root::getSingleton().getRenderSystem()->createHardwareOcclusionQuery(); //create a new HOQ for the scene this listener is used in
    }
    
    RenderQueueListener::~RenderQueueListener()
    {
        Ogre::Root::getSingleton().getRenderSystem()->destroyHardwareOcclusionQuery(hardwareOcclusionQuery_); //destroy the created HOQ
    }
    
    /**
    @brief
    This function is returning the current pixel count and resets the pixel state if we're ready to do another Hardware Occlusion Query
    
    @return
    current pixel count taken from the last Hardware Occlusion Query
    */
    unsigned int RenderQueueListener::getPixelCount()
    {
        if(this->pixelState_==RenderQueueListener::READY_FOR_ACCESS)
        {
            this->hardwareOcclusionQuery_->pullOcclusionQuery(&(this->pixelCount_));
            this->pixelState_=RenderQueueListener::READY_FOR_RENDER;
        }
        return this->pixelCount_;
    }
    
    /**
    @brief
    This function is called just before a RenderQueueGroup is rendered, this function is called by Ogre automatically with the correct parameters.

    In this case we use it to set the stencil buffer parameters of the render system and issue a Hardware Occlusion Query
    */
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
        if (queueGroupId == RENDER_QUEUE_HOQ && this->pixelState_==RenderQueueListener::READY_FOR_RENDER)
        { 
            this->hardwareOcclusionQuery_->beginOcclusionQuery();
            this->pixelState_=RenderQueueListener::QUERY_STARTED;
            //TODO: Skip this rendering step altogheter if we haven't requested the pixel count yet, not sure if this is possible without a custom SceneManager
        }
    }
    
    /**
    @brief
        This function is called just after a RenderQueueGroup has been rendered, this function is called by Ogre automatically with the correct parameters.
        
        in this case we use it to unset the stencil buffer parameters, so the rest of the render queue is unaffected by it.
    */
    void RenderQueueListener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
    {
        if (queueGroupId == RENDER_QUEUE_STENCIL_LAST) 
        {
            Ogre::RenderSystem * renderSystem = Ogre::Root::getSingleton().getRenderSystem(); 
            renderSystem->setStencilCheckEnabled(false); 
            renderSystem->setStencilBufferParams(); 
        }
        if (queueGroupId == RENDER_QUEUE_HOQ && this->pixelState_==RenderQueueListener::QUERY_STARTED)
        {
            this->hardwareOcclusionQuery_->endOcclusionQuery();
            this->pixelState_=RenderQueueListener::READY_FOR_ACCESS;
        }
    }
}