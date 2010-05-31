/* COPYRIGHT: this code comes from http://www.ogre3d.org/wiki/index.php/High_resolution_screenshots */

#ifndef __ScreenshotManager_h__
#define __ScreenshotManager_h__

#include "DesignToolsPrereqs.h"

#include <string>
#include <cstring>
#include <cstdlib>

#include <OgrePrerequisites.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{


    /* Class encapsulates Screenshot functionality and provides a method for making multi grid screenshots.	
    *  pRenderWindow:    Pointer to the render window.  This could be "mWindow" from the ExampleApplication,
    *              the window automatically created obtained when calling
    *              Ogre::Root::getSingletonPtr()->initialise(false) and retrieved by calling
    *              "Ogre::Root::getSingletonPtr()->getAutoCreatedWindow()", or the manually created
    *              window from calling "mRoot->createRenderWindow()".
    *  gridSize:      The magnification factor.  A 2 will create a 2x2 grid, doubling the size of the
                screenshot.  A 3 will create a 3x3 grid, tripling the size of the screenshot.
    *  fileExtension:    The extension of the screenshot file name, hence the type of graphics file to generate.
    *              To generate "MyScreenshot.png" this parameter would contain ".png".
    */
    class ScreenshotManager : public OrxonoxClass, public Singleton<ScreenshotManager>
    {
        friend class Singleton<ScreenshotManager>;

    public:
        ScreenshotManager();
        ~ScreenshotManager();

      /* Creates a screenshot with the given camera.
        * @param camera Pointer to the camera "looking at" the scene of interest
        * @param fileName the filename of the screenshot file.
      */
        void makeScreenshot() const;

        static void makeScreenshot_s()
            { getInstance().makeScreenshot(); }
      
    protected:
        static std::string getTimestamp();

        std::string    mFileExtension;
        unsigned int   mGridSize, mWindowWidth, mWindowHeight;
        bool           mDisableOverlays;
        //temp texture with current screensize
        Ogre::TexturePtr mTempTex;
        Ogre::RenderTexture* mRT;
        Ogre::HardwarePixelBufferSharedPtr mBuffer;
        //PixelBox for a large Screenshot, if grid size is > 1
        Ogre::PixelBox  mFinalPicturePB;
        uint8_t* data_;

        static ScreenshotManager* singletonPtr_s;
    };

}

#endif  // __ScreenshotManager_h__
