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
 *   Co-authors:
 *      ...
 *
 */

#include "Shader.h"

#include <OgreCompositorManager.h>
#include <OgreRoot.h>
#include <OgrePlugin.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/GraphicsManager.h"

namespace orxonox
{
    /**
        @brief Initializes the values and sets the scene manager.
    */
    Shader::Shader(Ogre::SceneManager* scenemanager) : compositorInstance_(0)
    {
        RegisterObject(Shader);

        this->scenemanager_ = scenemanager;
        this->bVisible_ = true;
        this->bLoadCompositor_ = GameMode::showsGraphics();
        this->registeredAsListener_ = false;

        static bool hasCgProgramManager = Shader::hasCgProgramManager();

        this->bLoadCompositor_ &= hasCgProgramManager;
    }

    /**
        @brief Removes the compositor and frees the resources.
    */
    Shader::~Shader()
    {
        if (this->compositorInstance_ && GraphicsManager::getInstance().getViewport())
            Ogre::CompositorManager::getSingleton().removeCompositor(GraphicsManager::getInstance().getViewport(), this->compositorName_);
    }

    /**
        @brief Inherited from ViewportEventListener - called if the camera changes.

        Since the new camera could be in a different scene, the shader has to make sure
        it deactivates or activates itself accordingly.

        Additionally the shader has to be turned off and on even if the camera stays in
        the same scene to fix a weird behavior of Ogre.
    */
    void Shader::cameraChanged(Ogre::Viewport* viewport, Ogre::Camera* oldCamera)
    {
        if (!this->bLoadCompositor_ || !this->scenemanager_)
            return;

        // load the compositor if not already done
        if (!this->compositorName_.empty() && !this->compositorInstance_)
            this->changedCompositorName(viewport);

        // update compositor in viewport (shader should only be active if the current camera is in the same scene as the shader)

        // Note:
        // The shader needs also to be switched off and on after changing the camera in the
        // same scene to avoid weird behaviour with active compositors while switching the
        // camera (like freezing the image)
        //
        // Last known Ogre version needing this workaround:
        // 1.4.8
        // 1.7.2

        if (oldCamera && this->scenemanager_ == oldCamera->getSceneManager())
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositorName_, false);

        if (viewport->getCamera() && this->scenemanager_ == viewport->getCamera()->getSceneManager())
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositorName_, this->isVisible());
    }

    /**
        @brief Changes the compositor - default viewport.
    */
    void Shader::changedCompositorName()
    {
        // For the moment, we get the viewport always from the graphics manager
        // TODO: Try to support multiple viewports - note however that scenemanager_->getCurrentViewport() returns NULL
        //       after switching to a camera in a different scene (only for the first time this scene is displayed though)
        this->changedCompositorName(GraphicsManager::getInstance().getViewport());
    }

    /**
        @brief Changes the compositor.
    */
    void Shader::changedCompositorName(Ogre::Viewport* viewport)
    {
        if (this->bLoadCompositor_)
        {
            assert(viewport);
            if (this->compositorInstance_)
            {
                // remove the old compositor, remove the listener
                Ogre::CompositorManager::getSingleton().removeCompositor(viewport, this->oldcompositorName_);
                this->compositorInstance_->removeListener(this);
                this->compositorInstance_ = 0;
            }
            if (!this->compositorName_.empty())
            {
                // add the new compositor
                this->compositorInstance_ = Ogre::CompositorManager::getSingleton().addCompositor(viewport, this->compositorName_);
                if (this->compositorInstance_)
                {
                    // register as listener if required
                    if (this->registeredAsListener_)
                        this->compositorInstance_->addListener(this);
                    // set visibility according to the isVisible() and the camera/viewport
                    if (viewport->getCamera())
                        Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositorName_, this->isVisible() && viewport->getCamera() && this->scenemanager_ == viewport->getCamera()->getSceneManager());
                }
                else
                    COUT(2) << "Warning: Couldn't load compositor with name \"" << this->compositorName_ << "\"." << std::endl;
            }
            this->oldcompositorName_ = this->compositorName_;
        }
    }

    /**
        @brief Changes the visibility of the shader. Doesn't free any resources if set to invisible.
    */
    void Shader::updateVisibility()
    {
        if (this->compositorInstance_)
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(GraphicsManager::getInstance().getViewport(), this->compositorName_, this->isVisible());
    }

    /**
        @brief Defines a new integer value for a given parameter. The parameter will be updated if the compositor is rendered the next time.
    */
    void Shader::setParameter(size_t technique, size_t pass, const std::string& parameter, int value)
    {
        ParameterContainer container = {technique, pass, parameter, value, 0.0f, MT_Type::Int};
        this->parameters_.push_back(container);
        this->addAsListener();
    }

    /**
        @brief Defines a new float value for a given parameter. The parameter will be updated if the compositor is rendered the next time.
    */
    void Shader::setParameter(size_t technique, size_t pass, const std::string& parameter, float value)
    {
        ParameterContainer container = {technique, pass, parameter, 0, value, MT_Type::Float};
        this->parameters_.push_back(container);
        this->addAsListener();
    }

    /**
        @brief Registers the shader as CompositorInstance::Listener at the compositor. Used to change parameters.
    */
    void Shader::addAsListener()
    {
        if (!this->registeredAsListener_)
        {
            this->registeredAsListener_ = true;
            if (this->compositorInstance_)
                this->compositorInstance_->addListener(this);
        }
    }

    /**
        @brief Inherited by Ogre::CompositorInstance::Listener, called whenever the material is rendered. Used to change parameters.
    */
    void Shader::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr& materialPtr)
    {
        // iterate through the list of parameters
        for (std::list<ParameterContainer>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); ++it)
        {
            Ogre::Technique* techniquePtr = materialPtr->getTechnique(it->technique_);
            if (techniquePtr)
            {
                Ogre::Pass* passPtr = techniquePtr->getPass(it->pass_);
                if (passPtr)
                {
                    // change the value of the parameter depending on its type
                    switch (it->valueType_)
                    {
                        case MT_Type::Int:
                            passPtr->getFragmentProgramParameters()->setNamedConstant(it->parameter_, it->valueInt_);
                            break;
                        case MT_Type::Float:
                            passPtr->getFragmentProgramParameters()->setNamedConstant(it->parameter_, it->valueFloat_);
                            break;
                        default:
                            break;
                    }
                }
                else
                    COUT(2) << "Warning: No pass " << it->pass_ << " in technique " << it->technique_ << " in compositor \"" << this->compositorName_ << "\" or pass has no shader." << std::endl;
            }
            else
                COUT(2) << "Warning: No technique " << it->technique_ << " in compositor \"" << this->compositorName_ << "\" or technique has no pass with shader." << std::endl;
        }
        this->parameters_.clear();
    }

    /**
        @brief Detects if the Cg program manager plugin is active.
    */
    /* static */ bool Shader::hasCgProgramManager()
    {
        if (Ogre::Root::getSingletonPtr())
        {
            const Ogre::Root::PluginInstanceList& plugins = Ogre::Root::getSingleton().getInstalledPlugins();
            for (size_t i = 0; i < plugins.size(); ++i)
                if (plugins[i]->getName() == "Cg Program Manager")
                    return true;
        }
        return false;
    }
}
