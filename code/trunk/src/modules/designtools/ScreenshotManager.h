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

    /**
    @brief
        Class encapsulates Screenshot functionality and provides a method for making multi grid screenshots.
    */
    class ScreenshotManager : public OrxonoxClass, public Singleton<ScreenshotManager>
    {
        friend class Singleton<ScreenshotManager>;

        public:
            ScreenshotManager();
            virtual ~ScreenshotManager();

            void makeScreenshot() const; //!< Creates a screenshot with the given camera.

            /**
            @brief Creates a screenshot with a given size.
            @param size Size is factor by which the current screen size is scaled.
            */
            static void makeScreenshot_s(unsigned int size)
                { getInstance().setGridSize(size); getInstance().makeScreenshot(); }

            void setGridSize(unsigned int size); //!< Set the size of the grid.

        protected:
            static std::string getTimestamp();

            std::string mFileExtension_;
            unsigned int mGridSize_; //!< The magnification factor.  A 2 will create a 2x2 grid, doubling the size of the screenshot.  A 3 will create a 3x3 grid, tripling the size of the screenshot.
            unsigned int mWindowWidth_, mWindowHeight_;
            bool mDisableOverlays_;
            //! temp texture with current screensize
            Ogre::TexturePtr mTempTex_;
            Ogre::RenderTexture* mRT_;
            Ogre::HardwarePixelBufferSharedPtr mBuffer_;
            //! PixelBox for a large Screenshot, if grid size is > 1
            Ogre::PixelBox  mFinalPicturePB_;
            uint8_t* data_;

            static ScreenshotManager* singletonPtr_s;
    };

}

#endif  // __ScreenshotManager_h__
