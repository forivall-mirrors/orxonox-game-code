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
 *      Gion-Andri Cantieni
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file SkyboxGenerator.h
    @brief Definition of the SkyboxGenerator class.
    @ingroup Designtools
*/

#ifndef __SkyboxGenerator_h__
#define __SkyboxGenerator_h__

#include "core/OrxonoxClass.h"
#include "util/Singleton.h"
#include "tools/interfaces/Tickable.h"

#include <OgreMath.h>

namespace orxonox
{
    
    /**
    @brief
        The SkyboxGenerator class is a singleton that allows for the creation of a skybox from a level by taking pictures to all 6 sides.
        The 6 images created by the createSkybox() method are placed in the log folder in your build directory.
        
        The image filename prefix and the file extension can both be adjusted as config values, so can the desired size of the skybox faces.
        
    @author
        Gion-Andri Cantieni
    @author
        Damian 'Mozork' Frick
    @ingroup Designtools
    */
    class SkyboxGenerator : public virtual OrxonoxClass, public Singleton<SkyboxGenerator>, public Tickable
    {
        friend class Singleton<SkyboxGenerator>;

        public:
            SkyboxGenerator();
            virtual ~SkyboxGenerator();
            void tick(float dt); // This is where the skybox generation happens.
            static void createSkybox(void); // Generate the 6 faces of a skybox.
            void setConfigValues(void); // Sets some config values.
            
            /**
            @brief Set the size of the skybox faces to be generated.
            @param size The size in pixels.
            */
            inline void setSize(unsigned int size)
                { this->size_ = size; }
                
        protected:
            /**
            @brief Starts the generation of the skybox.
            */
            inline void startSkyboxGeneration(void)
                { this->bGenerateSkybox_ = true; }

        private:
            void setupCamera(Ogre::Camera* camera); // Set up the input camera to be ready to generate a skybox face.
            void setupRenderWindow(Ogre::RenderWindow* renderWindow); // Setup the input render window to be ready to generate the skybox.
            void restoreRenderWindow(Ogre::RenderWindow* renderWindow); // Restore the render window.

            void saveImage(Ogre::Image* image, const std::string& name) const;
            
            static SkyboxGenerator* singletonPtr_s; //!< Singleton pointer.
            
            unsigned int size_; //!< The desired size of the skybox faces.
            std::string skyboxPrefix_; //!< Prefix for the generated image files.
            std::string imageExtension_; //!< Extension of the generated image files.
            
            // FLow control variables
            bool bGenerateSkybox_; //!< Whether a skybox is currently being generated.
            bool bCaptionsRemoved_; //!< Whether the overlays have been removed.
            bool bSetup_; //!< Whether the render window is being setup.
            bool bWait_; //!< Whether we have to wait for the setup to take effect.
            bool bCreateFace_; //!< Whether the faces are being created,
            unsigned int faceCounter_; //!< Counter to keep track of which skybox face is being created.
            bool bCleanup_; //!< Whether the generator is being cleaned up.
            
            std::vector<std::string> names_; //!< The names of the image files for the skybox faces to be generated.
            std::vector< std::pair<int, int> > rotations_; //!< The rotation in yaw an pitch direction that is applied to the camera after a specific face has been generated.
            
            // Storage variables
            float aspectRatio_; //!< The backed up aspect ratio of the camera.
            Ogre::Radian fovy_; //!< The backed up field of view of the camera.
            unsigned int windowWidth_; //!< The backed up window width.
            unsigned int windowHeight_; //!< The backed up window height.
            bool windowFullScreen_; //!< Whether the window was in fullscreen mode.
            unsigned int gridSize_; //!< The backed up grid size of the ScreenshotManager.

    };
}

#endif  // __SkyboxGenerator_h__