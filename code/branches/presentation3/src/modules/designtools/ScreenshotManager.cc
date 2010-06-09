/* COPYRIGHT: this code comes from http://www.ogre3d.org/wiki/index.php/High_resolution_screenshots */

#include "ScreenshotManager.h"

#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreRenderTexture.h>
#include <OgreCamera.h>
#include <OgreRoot.h>

#include "core/GraphicsManager.h"
#include "core/PathConfig.h"
#include "core/ScopedSingletonManager.h"
#include "core/ConsoleCommand.h"

#include "CameraManager.h"
#include "graphics/Camera.h"

namespace orxonox
{
    ManageScopedSingleton(ScreenshotManager, ScopeID::Graphics, false);
    SetConsoleCommandAlias(ScreenshotManager, makeScreenshot_s, "printScreenHD", true);

    ScreenshotManager::ScreenshotManager()
    {
        Ogre::RenderWindow* pRenderWindow = GraphicsManager::getInstance().getRenderWindow();
        int gridSize = 3;
        std::string fileExtension = ".png";
        bool overlayFlag = true;

        //set file extension for the Screenshot files
        mFileExtension   = fileExtension;
        // the gridsize
        mGridSize        = gridSize;
        // flag for overlay rendering
        mDisableOverlays = overlayFlag;
        //get current window size
        mWindowWidth   = pRenderWindow->getWidth();
        mWindowHeight  = pRenderWindow->getHeight();
        //create temporary texture
        mTempTex = Ogre::TextureManager::getSingleton().createManual("ScreenShotTex",
                                                                  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
                                                                    mWindowWidth, mWindowHeight,0, Ogre::PF_B8G8R8, Ogre::TU_RENDERTARGET);

        //get The current Render Target of the temp Texture
        mRT = mTempTex->getBuffer()->getRenderTarget();

        //HardwarePixelBufferSharedPtr to the Buffer of the temp Texture
        mBuffer = mTempTex->getBuffer();

        //create PixelBox
            uint8_t* data_ = new uint8_t[(mWindowWidth * mGridSize) * (mWindowHeight * mGridSize) * 3];
        mFinalPicturePB = Ogre::PixelBox(mWindowWidth * mGridSize,mWindowHeight * mGridSize,1,Ogre::PF_B8G8R8,data_);

    }


    ScreenshotManager::~ScreenshotManager()
    {
        // Don't delete data_. Somehow this pointer points anywhere but to memory location.
        //delete[] data_;
    }


    /* Creates a screenshot with the given camera.
    * @param camera Pointer to the camera "looking at" the scene of interest
    * @param fileName the filename of the screenshot file.
    */
    void ScreenshotManager::makeScreenshot() const
    {
        Ogre::Camera* camera = CameraManager::getInstance().getActiveCamera()->getOgreCamera();
        std::string fileName = PathConfig::getInstance().getLogPathString() + "screenshot_" + this->getTimestamp();

        //Remove all viewports, so the added Viewport(camera) ist the only
        mRT->removeAllViewports();
        mRT->addViewport(camera);

        //set the viewport settings
        Ogre::Viewport *vp = mRT->getViewport(0);
        vp->setClearEveryFrame(true);
        vp->setOverlaysEnabled(false);

        // remind current overlay flag
        bool enableOverlayFlag = GraphicsManager::getInstance().getViewport()->getOverlaysEnabled();

        // we disable overlay rendering if it is set in config file and the viewport setting is enabled
        if(mDisableOverlays && enableOverlayFlag)
            GraphicsManager::getInstance().getViewport()->setOverlaysEnabled(false);

        if(mGridSize <= 1)
        {
            // Simple case where the contents of the screen are taken directly
            // Also used when an invalid value is passed within gridSize (zero or negative grid size)
            mRT->update();    //render

            //write the file on the Harddisk
            mRT->writeContentsToFile(fileName + "." + mFileExtension);
        }
        else
        {
            //define the original frustum extents variables
            Ogre::Real originalFrustumLeft, originalFrustumRight, originalFrustumTop, originalFrustumBottom;
            // set the original Frustum extents
            camera->getFrustumExtents(originalFrustumLeft, originalFrustumRight, originalFrustumTop, originalFrustumBottom);

            // compute the Stepsize for the drid
            Ogre::Real frustumGridStepHorizontal  = (originalFrustumRight * 2) / mGridSize;
            Ogre::Real frustumGridStepVertical  = (originalFrustumTop * 2) / mGridSize;

            // process each grid
            Ogre::Real frustumLeft, frustumRight, frustumTop, frustumBottom;
            for (unsigned int nbScreenshots = 0; nbScreenshots < mGridSize * mGridSize; nbScreenshots++)
            {
                int y = nbScreenshots / mGridSize;
                int x = nbScreenshots - y * mGridSize;

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
                mRT->update();    //render

                //define the current
                Ogre::Box subBox = Ogre::Box(x* mWindowWidth,y * mWindowHeight,x * mWindowWidth + mWindowWidth, y * mWindowHeight + mWindowHeight);
                //copy the content from the temp buffer into the final picture PixelBox
                //Place the tempBuffer content at the right position
                mBuffer->blitToMemory(mFinalPicturePB.getSubVolume(subBox));

            }

            // set frustum extents to previous settings
            camera->resetFrustumExtents();

            Ogre::Image finalImage; //declare the final Image Object
            //insert the PixelBox data into the Image Object
            finalImage = finalImage.loadDynamicImage(static_cast<unsigned char*>(mFinalPicturePB.data), mFinalPicturePB.getWidth(),mFinalPicturePB.getHeight(),Ogre::PF_B8G8R8);
            // Save the Final image to a file
            finalImage.save(fileName + "." + mFileExtension);

        }

        // do we have to re-enable our overlays?
        if(enableOverlayFlag)
            GraphicsManager::getInstance().getViewport()->setOverlaysEnabled(true);


        // reset time since last frame to pause the scene
        Ogre::Root::getSingletonPtr()->clearEventTimes();
    }

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
