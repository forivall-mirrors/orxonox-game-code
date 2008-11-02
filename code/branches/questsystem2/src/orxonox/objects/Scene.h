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

#ifndef _Scene_H__
#define _Scene_H__

#include "OrxonoxPrereqs.h"

#include "network/Synchronisable.h"
#include "core/BaseObject.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport Scene : public BaseObject, public network::Synchronisable
    {
        public:
            Scene(BaseObject* creator);
            virtual ~Scene();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline Ogre::SceneManager* getSceneManager() const
                { return this->sceneManager_; }
            inline Ogre::SceneNode* getRootSceneNode() const
                { return this->rootSceneNode_; }

            void setSkybox(const std::string& skybox);
            inline const std::string& getSkybox() const
                { return this->skybox_; }

            void setAmbientLight(const ColourValue& colour);
            inline const ColourValue& getAmbientLight() const
                { return this->ambientLight_; }

            void setShadow(bool bShadow);
            inline bool getShadow() const
                { return this->bShadows_; }

        private:
            void addObject(BaseObject* object);
            BaseObject* getObject(unsigned int index) const;

            void networkcallback_applySkybox()
                { this->setSkybox(this->skybox_); }
            void networkcallback_applyAmbientLight()
                { this->setAmbientLight(this->ambientLight_); }

            Ogre::SceneManager*    sceneManager_;
            Ogre::SceneNode*       rootSceneNode_;
            std::string            skybox_;
            ColourValue            ambientLight_;
            std::list<BaseObject*> objects_;
            bool                   bShadows_;
    };
}

#endif /* _Scene_H__ */
