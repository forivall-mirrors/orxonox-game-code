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

#include "util/OgreForwardRefs.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    class _ToolsExport Shader : public Tickable
    {
        typedef std::pair<bool, void*>                  ParameterPointer;
        typedef std::map<std::string, ParameterPointer> ParameterMap;
        typedef std::vector<ParameterMap>               PassVector;
        typedef std::vector<PassVector>                 TechniqueVector;
        typedef std::map<std::string, TechniqueVector>  MaterialMap;

        public:
            Shader(Ogre::SceneManager* scenemanager = 0);
            virtual ~Shader();

            virtual void tick(float dt);

            inline void setVisible(bool bVisible)
            {
                if (this->bVisible_ != bVisible)
                {
                    this->bVisible_ = bVisible;
                    this->updateVisibility();
                }
            }
            inline bool isVisible() const
                { return this->bVisible_; }
            void updateVisibility();

            inline void setCompositor(const std::string& compositor)
            {
                if (this->compositor_ != compositor)
                {
                    this->compositor_ = compositor;
                    this->changedCompositor();
                }
            }
            inline const std::string& getCompositor() const
                { return this->compositor_; }
            void changedCompositor();

            void setSceneManager(Ogre::SceneManager* scenemanager);
            inline Ogre::SceneManager* getSceneManager() const
                { return this->scenemanager_; }

            void setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, float value);
            void setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, int value);

            static bool _setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, float value);
            static bool _setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, int value);
            static float getParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter);
            static bool  getParameterIsFloat(const std::string& material, size_t technique, size_t pass, const std::string& parameter);
            static bool  getParameterIsInt  (const std::string& material, size_t technique, size_t pass, const std::string& parameter);
            static ParameterPointer* getParameterPointer(const std::string& material, size_t technique, size_t pass, const std::string& parameter);

        private:
            Ogre::SceneManager* scenemanager_;
            Ogre::CompositorInstance* compositorInstance_;
            bool bVisible_;
            bool bLoadCompositor_;
            bool bViewportInitialized_;
            std::string compositor_;
            std::string oldcompositor_;

            static MaterialMap parameters_s;
            static bool bLoadedCgPlugin_s;
    };
}

#endif /* _Shader_H__ */
