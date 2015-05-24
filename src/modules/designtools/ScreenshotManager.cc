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
 *      This code comes from http://www.ogre3d.org/tikiwiki/High+resolution+screenshots which is Public Domain.
 *   Co-authors:
 *      Oli Scheuss
 *      Damian 'Mozork' Frick
 *
 */

/**
   @file ScreenshotManager.cc
   @brief Implementation of the ScreenshotManager class.
   @ingroup Designtools
*/

#include "ScreenshotManager.h"

#include <OgreCamera.h>
#include <OgreRenderTexture.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreViewport.h>

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/GraphicsManager.h"
#include "core/PathConfig.h"
#include "core/Resource.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "core/singleton/ScopedSingletonWrapper.h"
#include "util/StringUtils.h"

#include "CameraManager.h"
#include "graphics/Camera.h"

// #include <X11/Xlib.h> TODO: Needed?

namespace orxonox
{

    SetConsoleCommand("printScreenHD", &ScreenshotManager::makeScreenshot_s);
    
    ManageScopedSingleton(ScreenshotManager, ScopeID::Graphics, false);

    RegisterAbstractClass(ScreenshotManager).inheritsFrom<Configurable>();

    /**
    @brief
        Constructor. 
    */
    ScreenshotManager::ScreenshotManager() : finalPicturePB_(NULL), data_(NULL)
    {
        RegisterObject(ScreenshotManager);
        
        this->setConfigValues();

        // Flag for overlay rendering
        this->disableOverlays_ = true;
    }

    ScreenshotManager::~ScreenshotManager()
    {
        this->cleanup();
    }

    /**
    @brief
        Frees used memory.
    */
    void ScreenshotManager::cleanup(void)
    {
        if(this->finalPicturePB_ != NULL)
        {
            delete this->finalPicturePB_;
            this->finalPicturePB_ = NULL;
        }
        if(this->data_ != NULL)
        {
            delete this->data_;
            this->data_ = NULL;
        }
        if(!this->tempTexture_.isNull())
            this->tempTexture_.freeMethod();
    }
    
    /**
    @brief
        Sets some config values.
    */
    void ScreenshotManager::setConfigValues(void)
    {
        // Set file extension for the Screenshot files.
        SetConfigValue(fileExtension_, ".png");
        // The grid size.
        SetConfigValue(gridSize_, 3);
    }

    /**
    @brief
        Update internal parameters.
    */
    void ScreenshotManager::update(void)
    {
        Ogre::RenderWindow* pRenderWindow = GraphicsManager::getInstance().getRenderWindow();

        // Update current window size
        this->windowWidth_   = pRenderWindow->getWidth();
        this->windowHeight_  = pRenderWindow->getHeight();

        // Create temporary texture
        this->tempTexture_ = Ogre::TextureManager::getSingleton().createManual("ScreenShotTex", Resource::getDefaultResourceGroup(), Ogre::TEX_TYPE_2D, this->windowWidth_, this->windowHeight_, 0, Ogre::PF_B8G8R8, Ogre::TU_RENDERTARGET);

        // Get the current render target of the temporary texture
        this->renderTarget_ = this->tempTexture_->getBuffer()->getRenderTarget();

        // HardwarePixelBufferSharedPtr to the buffer of the temporary texture
        this->buffer_ = this->tempTexture_->getBuffer();

        // Create PixelBox
        this->data_ = new uint8_t[(this->windowWidth_ * this->gridSize_) * (this->windowHeight_ * this->gridSize_) * 3];
        this->finalPicturePB_ = new Ogre::PixelBox(this->windowWidth_ * this->gridSize_, this->windowHeight_ * this->gridSize_, 1, Ogre::PF_B8G8R8, this->data_);
    }

    /**
    @brief
        Make a screenshot.
        The screenshot is saved in the log folder.
    */
    void ScreenshotManager::makeScreenshot()
    {
        // Get the screenshot.
        Ogre::Image* finalImage = getScreenshot();
        if(finalImage != NULL)
        {
            // Save it.
            finalImage->save(PathConfig::getInstance().getLogPathString() + "screenshot_" + getTimestamp() + this->fileExtension_);
            delete finalImage;
            orxout(user_info) << "Finished taking " << this->gridSize_*this->windowWidth_ << "x" << this->gridSize_*this->windowHeight_ << " pixel HD screenshot. Storing in log/." << endl;
        }
        else
            orxout(user_error) << "There needs to be an active camera to make screenshots." << endl;

        this->cleanup();
    }

    /**
    @brief
        Creates a screenshot and returns it.
        After calling this method the ScreenshotManager should be cleaned using cleanup().
    @return
        Returns a pointer to an Ogre::Image with the screenshot. The memory must be freed, when the image is no longer needed.
    */
    Ogre::Image* ScreenshotManager::getScreenshot()
    {
        if(CameraManager::getInstance().getActiveCamera() == NULL )
            return NULL;
        return this->getScreenshot(CameraManager::getInstance().getActiveCamera()->getOgreCamera());
    }

    /**
    @brief
        Creates a screenshot with the given camera and returns it.
        After calling this method the ScreenshotManager should be cleaned using cleanup().
    @param camera
        A pointer to the camera the screenshot should be taken with.
    @return
        Returns a pointer to an Ogre::Image with the screenshot. The memory must be freed, when the image is no longer needed.
    */
    Ogre::Image* ScreenshotManager::getScreenshot(Ogre::Camera* camera)
    {
        if(camera == NULL)
            return NULL;
        
        // Update the internal parameters.
        this->update();

        // Add the camera as viewport.
        this->renderTarget_->removeAllViewports();
        this->renderTarget_->addViewport(camera);

        // Set the viewport settings
        Ogre::Viewport *vp = renderTarget_->getViewport(0);
        vp->setClearEveryFrame(true);
        vp->setOverlaysEnabled(false);

        // Remind current overlay flag
        bool enableOverlayFlag = GraphicsManager::getInstance().getViewport()->getOverlaysEnabled();
        
        // We disable overlay rendering if it is set in config file and the viewport setting is enabled
        if(this->disableOverlays_ && enableOverlayFlag)
            GraphicsManager::getInstance().getViewport()->setOverlaysEnabled(false);
        
        Ogre::Image* finalImage = new Ogre::Image();
        
        if(this->gridSize_ <= 1)
        {
            // Simple case where the contents of the screen are taken directly
            // Also used when an invalid value is passed within gridSize (zero or negative grid size)
            this->renderTarget_->update(); // Render

            this->buffer_->blitToMemory(*this->finalPicturePB_);
            finalImage->loadDynamicImage(static_cast<unsigned char*>(finalPicturePB_->data), finalPicturePB_->getWidth(), finalPicturePB_->getHeight(),Ogre::PF_B8G8R8);
        }
        else
        {
            // Define the original frustum extents variables
            Ogre::Real originalFrustumLeft, originalFrustumRight, originalFrustumTop, originalFrustumBottom;
            // Set the original Frustum extents
            camera->getFrustumExtents(originalFrustumLeft, originalFrustumRight, originalFrustumTop, originalFrustumBottom);
            
            // Compute the Stepsize for the grid
            Ogre::Real frustumGridStepHorizontal  = (originalFrustumRight * 2) / this->gridSize_;
            Ogre::Real frustumGridStepVertical  = (originalFrustumTop * 2) / this->gridSize_;
            
            // Process each grid
            Ogre::Real frustumLeft, frustumRight, frustumTop, frustumBottom;
            for (unsigned int nbScreenshots = 0; nbScreenshots < this->gridSize_ * this->gridSize_; nbScreenshots++)
            {
                int y = nbScreenshots / this->gridSize_;
                int x = nbScreenshots - y * this->gridSize_;
                
                // Shoggoth frustum extents setting
                // Compute the new frustum extents
                frustumLeft    = originalFrustumLeft + frustumGridStepHorizontal * x;
                frustumRight  = frustumLeft + frustumGridStepHorizontal;
                frustumTop    = originalFrustumTop - frustumGridStepVertical * y;
                frustumBottom  = frustumTop - frustumGridStepVertical;
                
                // Set the frustum extents value to the camera
                camera->setFrustumExtents(frustumLeft, frustumRight, frustumTop, frustumBottom);
                
                // Ignore time duration between frames
                Ogre::Root::getSingletonPtr()->clearEventTimes();
                this->renderTarget_->update(); // Render
                
                // Define the current box
                Ogre::Box subBox = Ogre::Box(x* this->windowWidth_,y * this->windowHeight_,x * this->windowWidth_ + this->windowWidth_, y * this->windowHeight_ + this->windowHeight_);
                // Copy the content from the temp buffer into the final picture PixelBox
                // Place the tempBuffer content at the right position
                this->buffer_->blitToMemory(this->finalPicturePB_->getSubVolume(subBox));
                
                orxout(internal_info) << "Created screenshot number " << nbScreenshots << " for multi grid HD screenshot." << endl;
                
            }
            
            // Set frustum extents to previous settings
            camera->resetFrustumExtents();
            
            // Insert the PixelBox data into the Image Object
            finalImage->loadDynamicImage(static_cast<unsigned char*>(this->finalPicturePB_->data), this->finalPicturePB_->getWidth(), this->finalPicturePB_->getHeight(), 1, Ogre::PF_B8G8R8, false);
        }
        
        // Do we have to re-enable our overlays?
        if(enableOverlayFlag)
            GraphicsManager::getInstance().getViewport()->setOverlaysEnabled(true);
        
        // Reset time since last frame to pause the scene
        Ogre::Root::getSingletonPtr()->clearEventTimes();
        
        return finalImage;
    }

    /**
    @brief
        Set the size of the grid.
    @param size
        The size of the grid.
    */
    void ScreenshotManager::setGridSize(unsigned int size)
    {
        if(size == this->gridSize_)
            return;

        this->gridSize_ = size;
    }

}
