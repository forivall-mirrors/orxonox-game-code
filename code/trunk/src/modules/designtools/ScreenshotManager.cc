/* COPYRIGHT: this code comes from http://www.ogre3d.org/wiki/index.php/High_resolution_screenshots */

#include "ScreenshotManager.h"

#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreRenderTexture.h>
#include <OgreCamera.h>
#include <OgreRoot.h>

#include "util/ScopedSingletonManager.h"
#include "core/GraphicsManager.h"
#include "core/PathConfig.h"
#include "core/command/ConsoleCommand.h"

#include "CameraManager.h"
#include "graphics/Camera.h"

namespace orxonox
{
    ManageScopedSingleton(ScreenshotManager, ScopeID::Graphics, false);
    SetConsoleCommand("printScreenHD", &ScreenshotManager::makeScreenshot_s);

    ScreenshotManager::ScreenshotManager()
    {
        Ogre::RenderWindow* pRenderWindow = GraphicsManager::getInstance().getRenderWindow();

        //set file extension for the Screenshot files
        this->mFileExtension_  = ".png";
        // the gridsize
        this->mGridSize_ = 3;
        // flag for overlay rendering
        this->mDisableOverlays_ = true;
        //get current window size
        this->mWindowWidth_   = pRenderWindow->getWidth();
        this->mWindowHeight_  = pRenderWindow->getHeight();
        //create temporary texture
        this->mTempTex_ = Ogre::TextureManager::getSingleton().createManual("ScreenShotTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, this->mWindowWidth_, this->mWindowHeight_, 0, Ogre::PF_B8G8R8, Ogre::TU_RENDERTARGET);

        //get The current Render Target of the temp Texture
        this->mRT_ = this->mTempTex_->getBuffer()->getRenderTarget();

        //HardwarePixelBufferSharedPtr to the Buffer of the temp Texture
        this->mBuffer_ = this->mTempTex_->getBuffer();

        //create PixelBox
        uint8_t* data_ = new uint8_t[(this->mWindowWidth_ * this->mGridSize_) * (this->mWindowHeight_ * this->mGridSize_) * 3];
        this->mFinalPicturePB_ = Ogre::PixelBox(this->mWindowWidth_ * this->mGridSize_, this->mWindowHeight_ * this->mGridSize_, 1, Ogre::PF_B8G8R8, data_);

    }


    ScreenshotManager::~ScreenshotManager()
    {
        // Don't delete data_. Somehow this pointer points anywhere but to memory location.
        //delete[] data_;
    }


    /**
    @brief
        Creates a screenshot with the given camera.
    */
    void ScreenshotManager::makeScreenshot() const
    {
        Ogre::Camera* camera = CameraManager::getInstance().getActiveCamera()->getOgreCamera();
        std::string fileName = PathConfig::getInstance().getLogPathString() + "screenshot_" + this->getTimestamp();

        //Remove all viewports, so the added Viewport(camera) ist the only
        mRT_->removeAllViewports();
        mRT_->addViewport(camera);

        //set the viewport settings
        Ogre::Viewport *vp = mRT_->getViewport(0);
        vp->setClearEveryFrame(true);
        vp->setOverlaysEnabled(false);

        // remind current overlay flag
        bool enableOverlayFlag = GraphicsManager::getInstance().getViewport()->getOverlaysEnabled();

        // we disable overlay rendering if it is set in config file and the viewport setting is enabled
        if(mDisableOverlays_ && enableOverlayFlag)
            GraphicsManager::getInstance().getViewport()->setOverlaysEnabled(false);

        if(mGridSize_ <= 1)
        {
            // Simple case where the contents of the screen are taken directly
            // Also used when an invalid value is passed within gridSize (zero or negative grid size)
            mRT_->update();    //render

            //write the file on the Harddisk
            mRT_->writeContentsToFile(fileName + "." + mFileExtension_);
        }
        else
        {
            //define the original frustum extents variables
            Ogre::Real originalFrustumLeft, originalFrustumRight, originalFrustumTop, originalFrustumBottom;
            // set the original Frustum extents
            camera->getFrustumExtents(originalFrustumLeft, originalFrustumRight, originalFrustumTop, originalFrustumBottom);

            // compute the Stepsize for the drid
            Ogre::Real frustumGridStepHorizontal  = (originalFrustumRight * 2) / mGridSize_;
            Ogre::Real frustumGridStepVertical  = (originalFrustumTop * 2) / mGridSize_;

            // process each grid
            Ogre::Real frustumLeft, frustumRight, frustumTop, frustumBottom;
            for (unsigned int nbScreenshots = 0; nbScreenshots < mGridSize_ * mGridSize_; nbScreenshots++)
            {
                int y = nbScreenshots / mGridSize_;
                int x = nbScreenshots - y * mGridSize_;

                // Shoggoth frustum extents setting
                // compute the new frustum extents
                frustumLeft    = originalFrustumLeft + frustumGridStepHorizontal * x;
                frustumRight  = frustumLeft + frustumGridStepHorizontal;
                frustumTop    = originalFrustumTop - frustumGridStepVertical * y;
                frustumBottom  = frustumTop - frustumGridStepVertical;

                // set the frustum extents value to the camera
                camera->setFrustumExtents(frustumLeft, frustumRight, frustumTop, frustumBottom);

                // ignore time duration between frames
                Ogre::Root::getSingletonPtr()->clearEventTimes();
                mRT_->update();    //render

                //define the current
                Ogre::Box subBox = Ogre::Box(x* mWindowWidth_,y * mWindowHeight_,x * mWindowWidth_ + mWindowWidth_, y * mWindowHeight_ + mWindowHeight_);
                //copy the content from the temp buffer into the final picture PixelBox
                //Place the tempBuffer content at the right position
                mBuffer_->blitToMemory(mFinalPicturePB_.getSubVolume(subBox));

            }

            // set frustum extents to previous settings
            camera->resetFrustumExtents();

            Ogre::Image finalImage; //declare the final Image Object
            //insert the PixelBox data into the Image Object
            finalImage = finalImage.loadDynamicImage(static_cast<unsigned char*>(mFinalPicturePB_.data), mFinalPicturePB_.getWidth(), mFinalPicturePB_.getHeight(),Ogre::PF_B8G8R8);
            // Save the Final image to a file
            finalImage.save(fileName + "." + mFileExtension_);

        }

        // do we have to re-enable our overlays?
        if(enableOverlayFlag)
            GraphicsManager::getInstance().getViewport()->setOverlaysEnabled(true);


        // reset time since last frame to pause the scene
        Ogre::Root::getSingletonPtr()->clearEventTimes();
    }

    /**
    @brief
        Set the size of the grid.
    @param size
        The size of the grid.
    */
    void ScreenshotManager::setGridSize(unsigned int size)
    {
        if(size == this->mGridSize_)
            return;

        this->mGridSize_ = size;
        // New PixelBox for the changed size.
        uint8_t* data_ = new uint8_t[(this->mWindowWidth_ * this->mGridSize_) * (this->mWindowHeight_ * this->mGridSize_) * 3];
        this->mFinalPicturePB_ = Ogre::PixelBox(this->mWindowWidth_ * this->mGridSize_, this->mWindowHeight_ * this->mGridSize_, 1, Ogre::PF_B8G8R8, data_);
    }

    /**
    @brief
        Get a timestamp for the curent time instant.
    @return
        Returns a string with the timestamp.
    */
    std::string ScreenshotManager::getTimestamp()
    {
        struct tm *pTime;
        time_t ctTime; time(&ctTime);
        pTime = localtime( &ctTime );
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << (pTime->tm_mon + 1)
            << std::setw(2) << std::setfill('0') << pTime->tm_mday
            << std::setw(2) << std::setfill('0') << (pTime->tm_year + 1900)
            << "_" << std::setw(2) << std::setfill('0') << pTime->tm_hour
            << std::setw(2) << std::setfill('0') << pTime->tm_min
            << std::setw(2) << std::setfill('0') << pTime->tm_sec;
        return oss.str();
    }

}
