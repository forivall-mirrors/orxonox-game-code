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

#ifndef _Shader_H__
#define _Shader_H__

#include "tools/ToolsPrereqs.h"

#include <map>
#include <string>
#include <vector>

#include <OgreCompositorInstance.h>

#include "util/MultiType.h"
#include "util/OgreForwardRefs.h"
#include "core/ViewportEventListener.h"

namespace orxonox
{
    /**
        @brief Shader is a wrapper class around Ogre::CompositorInstance. It provides some
        functions to easily change the visibility and parameters for shader programs.
    */
    class _ToolsExport Shader : public ViewportEventListener, public Ogre::CompositorInstance::Listener
    {
        public:
            Shader(Ogre::SceneManager* scenemanager = 0);
            virtual ~Shader();

            /// Defines if the shader is visible or not.
            inline void setVisible(bool bVisible)
            {
                if (this->bVisible_ != bVisible)
                {
                    this->bVisible_ = bVisible;
                    this->updateVisibility();
                }
            }
            /// Returns whether or not the shader is visible.
            inline bool isVisible() const
                { return this->bVisible_; }
            void updateVisibility();

            /// Defines the compositor's name (located in a .compositor file).
            inline void setCompositorName(const std::string& name)
            {
                if (this->compositorName_ != name)
                {
                    this->compositorName_ = name;
                    this->changedCompositorName();
                }
            }
            /// Returns the compositor's name.
            inline const std::string& getCompositorName() const
                { return this->compositorName_; }
            void changedCompositorName();
            void changedCompositorName(Ogre::Viewport* viewport);

            /// Sets the scenemanager (usually provided in the constructor, but can be set later). Shouldn't be changed once it's set.
            inline void setSceneManager(Ogre::SceneManager* scenemanager)
                { this->scenemanager_ = scenemanager; }
            /// Returns the scene manager.
            inline Ogre::SceneManager* getSceneManager() const
                { return this->scenemanager_; }
            
            /// Returns the compositor instance. Normally used for manually modifiying compositor parameters, when mt-type conversatio.
            inline Ogre::CompositorInstance* getMutableCompositorInstance()
                { return this->compositorInstance_; }
        
            virtual void cameraChanged(Ogre::Viewport* viewport, Ogre::Camera* oldCamera);

            void setParameter(size_t technique, size_t pass, const std::string& parameter, int value);
            void setParameter(size_t technique, size_t pass, const std::string& parameter, float value);

            virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr& materialPtr);

        private:
            static bool hasCgProgramManager();

            Ogre::SceneManager* scenemanager_;              ///< The scenemanager for which the shader is active
            Ogre::CompositorInstance* compositorInstance_;  ///< The compositor instance representing the wrapped compositor
            bool bVisible_;                                 ///< True if the shader should be visible
            bool bLoadCompositor_;                          ///< True if the compositor should be loaded (usually false if no graphics)
            std::string compositorName_;                    ///< The name of the current compositor
            std::string oldcompositorName_;                 ///< The name of the previous compositor (used to unregister)

        private:
            void addAsListener();

            /// Helper struct to store parameters for shader programs.
            struct ParameterContainer
            {
                size_t technique_;          ///< The ID of the technique
                size_t pass_;               ///< The ID of the pass
                std::string parameter_;     ///< The name of the parameter

                int valueInt_;              ///< The desired int value of the parameter
                float valueFloat_;          ///< The desired float value of the parameter

                MT_Type::Value valueType_;  ///< The type of the parameter (currently only int or float)
            };

            std::list<ParameterContainer> parameters_;  ///< The list of parameters that should be set on the next update
            bool registeredAsListener_;                 ///< True if the shader should register itself as listener at the compositor
    };
}

#endif /* _Shader_H__ */
