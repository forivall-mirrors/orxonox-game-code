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
 *      Benjamin de Capitani
 *
 *
 */

#include "AnimatedModel.h"

#include <OgreEntity.h>
#include <OgreAnimationState.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(AnimatedModel);

    AnimatedModel::AnimatedModel(BaseObject* creator) : Model(creator)
    {
        RegisterObject(AnimatedModel);
    }


    AnimatedModel::~AnimatedModel()
    {
        if (this->isInitialized() && this->mesh_.getEntity())
            this->detachOgreObject(this->mesh_.getEntity());
    }

    void AnimatedModel::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AnimatedModel, XMLPort, xmlelement, mode);
        XMLPortParam(AnimatedModel,"anims",setAnims,getAnims, xmlelement,mode).defaultValues("");
        XMLPortParam(AnimatedModel,"loop", setAnimLoop,getAnimLoop, xmlelement,mode).defaultValues(true);
        XMLPortParam(AnimatedModel,"enabled", setAnimEnabled,getAnimEnabled, xmlelement,mode).defaultValues(true);
    }


    void AnimatedModel::changedMesh()
    {
        if (GameMode::showsGraphics())
        {
            if (this->mesh_.getEntity())
                this->detachOgreObject(this->mesh_.getEntity());

            this->mesh_.setMeshSource(this->getScene()->getSceneManager(), this->meshSrc_);

            if (this->mesh_.getEntity())
            {
                this->attachOgreObject(this->mesh_.getEntity());
                this->mesh_.getEntity()->setCastShadows(this->bCastShadows_);
                this->setAnimationState(this->bAnimLoop_, this->bAnimEnabled_, this->anims_);
                this->mesh_.setVisible(this->isVisible());
            }
        }
    }

    void AnimatedModel::changedAnimationState()
    {
        this->setAnimationState(this->bAnimLoop_, this->bAnimEnabled_, this->anims_);
    }
    void AnimatedModel::setAnimationState(bool loop, bool enabled, const std::string& state)
    {
        if(state!="")
        {
        if(this->mesh_.getEntity()->getAnimationState(state))
        {
        Ogre::AnimationState* as = this->mesh_.getEntity()->getAnimationState(state);
        as->setLoop(loop);
        as->setEnabled(enabled);
        this->anims_ = state;
        }
        }
    }

    void AnimatedModel::setAnimLoop(bool loop)
    {
        this->bAnimLoop_ = loop;
    }
    void AnimatedModel::setAnimEnabled(bool enabled)
    {
        this->bAnimEnabled_ = enabled;
    }
    void AnimatedModel::tick(float dt)
    {
        if(this->mesh_.getEntity()->getAnimationState(anims_))
        {
// Test to change Material at runtime!

//            Ogre::MaterialPtr mat = this->mesh_.getEntity()->getSubEntity(0)->getMaterial();
//            mat->setDiffuse(0.4, 0.3, 0.1, 0.1);
//            mat->setAmbient(0.3, 0.7, 0.8);
//            mat->setSpecular(0.5, 0.5, 0.5, 0.1);
//            Ogre::SceneBlendType sbt = Ogre::SBT_ADD;
//
//            mat->setSceneBlending(sbt);

            Ogre::AnimationState* as = this->mesh_.getEntity()->getAnimationState(anims_);
            as->addTime(dt);
    }
}
}
