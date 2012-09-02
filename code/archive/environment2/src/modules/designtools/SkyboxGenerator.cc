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
    @file SkyboxGenerator.cc
    @brief Implementation of the SkyboxGenerator class.
*/

#include "SkyboxGenerator.h"

#include <string>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/GraphicsManager.h"
#include "core/PathConfig.h"
#include "core/Resource.h"
#include "core/command/ConsoleCommand.h"
#include "core/command/CommandExecutor.h"

#include "controllers/HumanController.h"
#include "graphics/Camera.h"
#include "worldentities/ControllableEntity.h"

#include "ScreenshotManager.h"

// #include <X11/Xlib.h> TODO: Needed?

namespace orxonox
{

    SetConsoleCommand("SkyboxGenerator", "createSkybox", &SkyboxGenerator::createSkybox).addShortcut();

    ManageScopedSingleton(SkyboxGenerator, ScopeID::Graphics, false);

    /**
    @brief
        Constructor. Registers and initializes the singleton.
    */
    SkyboxGenerator::SkyboxGenerator()
    {
        RegisterRootObject(SkyboxGenerator);

        this->setConfigValues();
        
        this->bGenerateSkybox_ = false;
        this->bCaptionsRemoved_ = false;
        this->bSetup_ = true;
        this->bWait_ = false;
        this->bCreateFace_ = true;
        this->bCleanup_ = true;
        this->faceCounter_ = 0;
        
        this->names_.push_back("fr");
        this->names_.push_back("lf");
        this->names_.push_back("bk");
        this->names_.push_back("rt");
        this->names_.push_back("up");
        this->names_.push_back("dn");
        
        this->rotations_.push_back(std::pair<int, int>(90, 0));
        this->rotations_.push_back(std::pair<int, int>(90, 0));
        this->rotations_.push_back(std::pair<int, int>(90, 0));
        this->rotations_.push_back(std::pair<int, int>(90, 90));
        this->rotations_.push_back(std::pair<int, int>(0, 180));
        this->rotations_.push_back(std::pair<int, int>(0, 90));
    }

    /**
    @brief
        Destructor.
    */
    SkyboxGenerator::~SkyboxGenerator()
    {

    }

    /**
    @brief
        Sets some config values.
    */
    void SkyboxGenerator::setConfigValues( )
    {
        SetConfigValue(skyboxPrefix_, "SkyboxFile_");
        SetConfigValue(imageExtension_, ".png");
        SetConfigValue(size_, 1024);
    }
    
    /**
    @brief
        Generate the 6 faces of a skybox.
    */
    void SkyboxGenerator::createSkybox()
    {
        // Pause
        CommandExecutor::execute("pause");
        // Start the skybox generation process.
        SkyboxGenerator::getInstance().startSkyboxGeneration();
    }

    /**
    @brief
        This is where the skybox generation happens.
        Generating a skybox takes several (up to 10) ticks.
    */
    void SkyboxGenerator::tick(float dt)
    {
        // Whether a skybox is currently being generated.
        if(this->bGenerateSkybox_)
        {
            // Wait one tick.
            if(this->bWait_)
            {
                this->bWait_ = false;
                return;
            }

            ControllableEntity* entity = NULL;
            if(HumanController::getLocalControllerSingleton() != NULL && HumanController::getLocalControllerSingleton()->getControllableEntity() != NULL)
                entity = HumanController::getLocalControllerSingleton()->getControllableEntity();
            else
            {
                COUT(1) << "You must be in a level to generate a skybox." << endl;
                this->bGenerateSkybox_ = false;
                return;
            }
            Ogre::Camera* camera = entity->getCamera()->getOgreCamera();
            Ogre::RenderWindow* renderWindow = GraphicsManager::getInstance().getRenderWindow();
            
            // Setup the SkyboxGenerator to generate a skybox.
            if(this->bSetup_)
            {
                // If there are captions being displayed, don't.
                if(!this->bCaptionsRemoved_)
                {
                    CommandExecutor::execute("GametypeStatus displayCaption false");
                    this->bCaptionsRemoved_ = true;
                    return;
                }
                
                // Store the settings for the camera.
                this->fovy_ = camera->getFOVy();
                this->aspectRatio_ = camera->getAspectRatio();
                // Setup the render window.
                this->setupRenderWindow(renderWindow);
                // Add the log path to the standard resource group.
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(PathConfig::getInstance().getLogPathString(), "FileSystem", Resource::getDefaultResourceGroup());
                
                COUT(4) << "Setting up SkyboxGenerator..." << endl;
                
                this->bSetup_ = false;
                this->bWait_ = true;
            }
            // Create one of the faces. (faceCounter_ decides which)
            else if(this->bCreateFace_)
            {
                // Setup the camera.
                this->setupCamera(camera);
                // Take the picture using the ScreenshotManager and save it.
                this->saveImage(ScreenshotManager::getInstance().getScreenshot(camera), this->skyboxPrefix_+this->names_[this->faceCounter_]+this->imageExtension_);
                // Rotate the camera to be ready for taking the next picture.
                std::pair<int, int> rotate = this->rotations_[this->faceCounter_];
                if(rotate.first != 0)
                    entity->yaw(Degree((float)rotate.first));
                if(rotate.second != 0)
                    entity->pitch(Degree((float)rotate.second));
                
                COUT(4) << "Created face number " << this->faceCounter_ << "." << endl;
                // Check whether we've generated all 6 faces.
                if(++this->faceCounter_ >= 6)
                    this->bCreateFace_ = false;
            }
            // Cleanup after the successful creation of a skybox.
            else if(this->bCleanup_)
            {
                // Reset the camera parameters.
                camera->setAspectRatio(this->aspectRatio_);
                camera->setFOVy(this->fovy_);
                // Restore the render window.
                this->restoreRenderWindow(renderWindow);
                // Remove the log path from the standard resource group.
                Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(PathConfig::getInstance().getLogPathString(), Resource::getDefaultResourceGroup());
                
                // Reset the flow parameters for the next skybox generation.
                this->bGenerateSkybox_ = false;
                this->bSetup_ = true;
                this->bWait_ = false;
                this->bCreateFace_ = true;
                this->bCleanup_ = true;
                this->faceCounter_ = 0;
                
                // Display captions again.
                CommandExecutor::execute("GametypeStatus displayCaption true");
                this->bCaptionsRemoved_ = false;
                
                // Unpause.
                CommandExecutor::execute("pause");
                
                COUT(3) << "Skybox with face size " << this->size_ << "x" << this->size_ << " pixels created. Storing in log/." << endl;
            }
        }
    }
    
    /**
    @brief
        Set up the input camera to be ready to generate a skybox face.
    @param camera
        The camera to be set up.
    */
    void SkyboxGenerator::setupCamera(Ogre::Camera* camera)
    {
        camera->setFOVy(Degree(90));
        camera->setAspectRatio(1.0);
    }
    
    /**
    @brief
        Setup the input render window to be ready to generate the skybox.
    @param renderWindow
        The render window to be set up.
    */
    void SkyboxGenerator::setupRenderWindow(Ogre::RenderWindow* renderWindow)
    {
        // Store current window properties.
        this->windowWidth_ = renderWindow->getWidth();
        this->windowHeight_ = renderWindow->getHeight();
        this->windowFullScreen_ = renderWindow->isFullScreen();
        // Store current ScreenshotManager grid size.
        this->gridSize_ = ScreenshotManager::getInstance().getGridSize();
        
        unsigned int size = this->size_;
        
        // If the desired skybox face size is bigger than what the current render window can accommodate we find a size that is a multiple of 256, that fits into the current render window adjust the grid size of the ScreenshotManager such that the screenshots generated are at least as big as we need. 
        if(this->windowHeight_ < this->size_ || this->windowWidth_ < this->size_)
        {
            unsigned int min = std::min(this->windowHeight_, this->windowWidth_);
            unsigned int step = 256;
            assert(min >= step);
            size = step;
            while(min >= size+step)
                size += step;
            
            unsigned int gridSize = 1;
            while(gridSize*size < this->size_)
                gridSize++;
            
            renderWindow->setFullscreen(false, size, size);
            ScreenshotManager::getInstance().setGridSize(gridSize);
        }
        else
            ScreenshotManager::getInstance().setGridSize(1);

    }
    
    /**
    @brief
        Restore the render window.
        Reset the window size, reset the grid size of the ScreenshotManager.
    @param renderWindow
        The render window to be restored.
    */
    void SkyboxGenerator::restoreRenderWindow(Ogre::RenderWindow* renderWindow)
    {
        // Restore window size.
        renderWindow->setFullscreen(this->windowFullScreen_, this->windowWidth_, this->windowHeight_);
        // Restore grid size.
        ScreenshotManager::getInstance().setGridSize(this->gridSize_);
    }
    
    /**
    @brief
        Resizes and saves the input image under the input name.
    @param image
        A pointer to the image to be saved. The image is deleted afterwards,
    @param name
        The desired filename of the image.
    */
    void SkyboxGenerator::saveImage(Ogre::Image* image, const std::string& name) const
    {
        image->save(PathConfig::getInstance().getLogPathString()+name);
        delete image;
        // Loading the resizing, then saving again. This seems stupid, but resizing doesn't seem to work otherwise.
        // If someone figures this out, feel free to adjust.
        image = new Ogre::Image();
        image->load(name, Resource::getDefaultResourceGroup());
        image->resize(this->size_, this->size_);
        image->save(PathConfig::getInstance().getLogPathString()+name);
        delete image;
    }
}
