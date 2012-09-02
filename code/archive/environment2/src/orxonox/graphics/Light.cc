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

#include "Light.h"

#include <OgreSceneManager.h>
#include <OgreLight.h>
#include <boost/static_assert.hpp>

#include "util/StringUtils.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(Light);

    // Be sure we don't do bad conversions
    BOOST_STATIC_ASSERT((int)Ogre::Light::LT_POINT       == (int)Light::Point);
    BOOST_STATIC_ASSERT((int)Ogre::Light::LT_DIRECTIONAL == (int)Light::Directional);
    BOOST_STATIC_ASSERT((int)Ogre::Light::LT_SPOTLIGHT   == (int)Light::Spotlight);

    Light::Light(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Light);

        this->light_ = 0;
        this->diffuse_ = ColourValue::White;
        this->specular_ = ColourValue::White;
        this->type_ = Light::Point;
        this->attenuation_ = Vector4(100000, 1, 0, 0);
        this->spotlightRange_ = Vector3(40.0f, 30.0f, 1.0f);

        if (GameMode::showsGraphics())
        {
            if (!this->getScene())
                ThrowException(AbortLoading, "Can't create Light, no scene given.");
            if (!this->getScene()->getSceneManager())
                ThrowException(AbortLoading, "Can't create Light, no scene manager given.");

            if (this->getScene() && this->getScene()->getSceneManager())
            {
                this->light_ = this->getScene()->getSceneManager()->createLight("Light" + getUniqueNumberString());
                this->light_->setDirection(WorldEntity::FRONT);
                this->attachOgreObject(this->light_);

                this->updateType();
                this->updateDiffuseColour();
                this->updateSpecularColour();
                this->updateAttenuation();
                this->updateSpotlightRange();
            }
        }

        this->registerVariables();
    }

    Light::~Light()
    {
        if (this->isInitialized())
        {
            if (this->light_ && this->getScene() && this->getScene()->getSceneManager())
                this->getScene()->getSceneManager()->destroyLight(this->light_);
        }
    }

    void Light::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Light, XMLPort, xmlelement, mode);

        XMLPortParam(Light, "type",           setTypeString,     getTypeString,     xmlelement, mode).defaultValues("point");
        XMLPortParam(Light, "diffuse",        setDiffuseColour,  getDiffuseColour,  xmlelement, mode).defaultValues(ColourValue::White);
        XMLPortParam(Light, "specular",       setSpecularColour, getSpecularColour, xmlelement, mode).defaultValues(ColourValue::White);
        XMLPortParam(Light, "attenuation",    setAttenuation,    getAttenuation,    xmlelement, mode).defaultValues(Vector4(100000, 1, 0, 0));
        XMLPortParam(Light, "spotlightrange", setSpotlightRange, getSpotlightRange, xmlelement, mode).defaultValues(Vector3(40.0f, 30.0f, 1.0f));
    }

    void Light::registerVariables()
    {
        registerVariable((int&)this->type_,     VariableDirection::ToClient, new NetworkCallback<Light>(this, &Light::updateType));
        registerVariable(this->diffuse_,        VariableDirection::ToClient, new NetworkCallback<Light>(this, &Light::updateDiffuseColour));
        registerVariable(this->specular_,       VariableDirection::ToClient, new NetworkCallback<Light>(this, &Light::updateSpecularColour));
        registerVariable(this->attenuation_,    VariableDirection::ToClient, new NetworkCallback<Light>(this, &Light::updateAttenuation));
        registerVariable(this->spotlightRange_, VariableDirection::ToClient, new NetworkCallback<Light>(this, &Light::updateSpotlightRange));
    }

    void Light::updateDiffuseColour()
    {
        if (this->light_)
            this->light_->setDiffuseColour(this->diffuse_);
    }

    void Light::updateSpecularColour()
    {
        if (this->light_)
            this->light_->setSpecularColour(this->specular_);
    }

    void Light::updateAttenuation()
    {
        if (this->light_ && this->type_ != Light::Directional)
            this->light_->setAttenuation(this->attenuation_.x, this->attenuation_.y, this->attenuation_.z, this->attenuation_.w);
    }

    void Light::updateSpotlightRange()
    {
        if (this->light_ && this->type_ == Light::Spotlight)
            this->light_->setSpotlightRange(Degree(this->spotlightRange_.x), Degree(this->spotlightRange_.y), this->spotlightRange_.z);
    }

    void Light::setTypeString(const std::string& type)
    {
        if (type == "point")
            this->setType(Light::Point);
        else if (type == "directional")
            this->setType(Light::Directional);
        else if (type == "spotlight")
            this->setType(Light::Spotlight);
        else
            this->setType(Light::Point);
    }

    std::string Light::getTypeString() const
    {
        switch (this->type_)
        {
            case Light::Directional:
                return "directional";
            case Light::Spotlight:
                return "spotlight";
            case Light::Point:
            default:
                return "point";
        }
    }

    void Light::updateType()
    {
        if (this->light_)
        {
            this->light_->setType(static_cast<Ogre::Light::LightTypes>(this->type_));

            if (this->type_ != Light::Directional)
                this->updateAttenuation();
            if (this->type_ == Light::Spotlight)
                this->updateSpotlightRange();
        }
    }

    void Light::changedVisibility()
    {
        SUPER(Light, changedVisibility);

        if (this->light_)
            this->light_->setVisible(this->isVisible());
    }
}
