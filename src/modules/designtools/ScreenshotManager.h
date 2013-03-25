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
   @file ScreenshotManager.h
   @brief Definition of the ScreenshotManager class.
   @ingroup Designtools
*/

#ifndef __ScreenshotManager_h__
#define __ScreenshotManager_h__

#include "DesignToolsPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>

#include "util/Singleton.h"
#include "core/config/Configurable.h"

namespace orxonox
{

    /**
    @brief
        Class encapsulates screenshot functionality and provides a method for making multi grid (i.e. HD) screenshots.
        
    @author
        This code comes from http://www.ogre3d.org/tikiwiki/High+resolution+screenshots which is Public Domain.
    @author
        Oli Scheuss
    @author
        Damian 'Mozork' Frick
    @ingroup Designtools
    */
    class ScreenshotManager : public Configurable, public Singleton<ScreenshotManager>
    {
        friend class Singleton<ScreenshotManager>;

        public:
            ScreenshotManager();
            virtual ~ScreenshotManager();
            void setConfigValues(void); // Sets some config values.

            void makeScreenshot(); // Make a screenshot.
            Ogre::Image* getScreenshot(); // Creates a screenshot and returns it.
            Ogre::Image* getScreenshot(Ogre::Camera* camera); // Creates a screenshot with the given camera and returns it.

            /**
            @brief Creates a screenshot.
            */
            static void makeScreenshot_s()
                { ScreenshotManager::getInstance().makeScreenshot(); }

            void setGridSize(unsigned int size); // Set the size of the grid.
            /**
            @brief Get the current grid size.
            @return Returns the size of the grid.
            */
            inline unsigned int getGridSize(void)
                { return this->gridSize_; }

            void cleanup(void); // Frees used memory.
            
        protected:
            void update(void); // Update internal parameters.
            
            static ScreenshotManager* singletonPtr_s;

            std::string fileExtension_; //!< The image extension used to save the screenshot.
            unsigned int gridSize_; //!< The magnification factor.  A 2 will create a 2x2 grid, doubling the size of the screenshot.  A 3 will create a 3x3 grid, tripling the size of the screenshot.
            unsigned int windowWidth_, windowHeight_; //!< The width and height of the window.
            bool disableOverlays_; //!< Whether overlays should be disabled.

            Ogre::TexturePtr tempTexture_; //!< Temporary texture with current screen size.
            Ogre::RenderTexture* renderTarget_; //!< Render target for the temporary texture.
            Ogre::HardwarePixelBufferSharedPtr buffer_; //!< Buffer for the temporary texture.
            
            Ogre::PixelBox* finalPicturePB_; //!< PixelBox for large screenshots, contains the screenshot for gridSize_ > 1.
            uint8_t* data_; //!< Data pointer for the PixelBox.

    };

}

#endif  // __ScreenshotManager_h__
