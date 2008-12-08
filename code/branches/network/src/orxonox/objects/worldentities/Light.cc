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

#include "OrxonoxStableHeaders.h"
#include "Light.h"

#include <sstream>
#include <cassert>

#include <OgreSceneManager.h>

#include "util/String.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/Scene.h"

namespace orxonox
{
    unsigned int Light::lightCounter_s = 0;

    CreateFactory(Light);

    Light::Light(BaseObject* creator) : PositionableEntity(creator)
    {
        RegisterObject(Light);

        if (this->getScene() && this->getScene()->getSceneManager())
        this->light_ = this->getScene()->getSceneManager()->createLight("Light" + convertToString(Light::lightCounter_s++));
        this->getNode()->attachObject(this->light_);

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

        XMLPortParam(Light, "type", setTypeString, getTypeString, xmlelement, mode).defaultValues("point");
        XMLPortParamExternTemplate(Light, Ogre::Light, this->light_, "diffuse",   setDiffuseColour,  getDiffuseColour,  xmlelement, mode, const ColourValue&);
        XMLPortParamExternTemplate(Light, Ogre::Light, this->light_, "specular",  setSpecularColour, getSpecularColour, xmlelement, mode, const ColourValue&);
        XMLPortParamExternTemplate(Light, Ogre::Light, this->light_, "direction", setDirection,      getDirection,      xmlelement, mode, const Vector3&);
    }

    void Light::registerVariables()
    {
        registerVariable((int &)this->type_, variableDirection::toclient, new NetworkCallback<Light>(this, &Light::changedType));
        registerVariable(this->light_->getDiffuseColour(), variableDirection::toclient);
        registerVariable(this->light_->getSpecularColour(), variableDirection::toclient);
        registerVariable(this->light_->getDirection(), variableDirection::toclient);
    }

    const std::string& Light::getName() const
    {
        if (this->light_)
            return this->light_->getName();
        else
            return BLANKSTRING;
    }

    void Light::setDiffuseColour(const ColourValue& colour)
    {
        if (this->light_)
            this->light_->setDiffuseColour(colour);
    }

    const ColourValue& Light::getDiffuseColour() const
    {
        if (this->light_)
            return this->light_->getDiffuseColour();
        else
            return ColourValue::White;
    }

    void Light::setSpecularColour(const ColourValue& colour)
    {
        if (this->light_)
            this->light_->setSpecularColour(colour);
    }

    const ColourValue& Light::getSpecularColour() const
    {
        if (this->light_)
            return this->light_->getSpecularColour();
        else
            return ColourValue::White;
    }

    void Light::setDirection(const Vector3& direction)
    {
        if (this->light_)
            this->light_->setDirection(direction);
    }

    const Vector3& Light::getDirection() const
    {
        if (this->light_)
            return this->light_->getDirection();
        else
            return Vector3::ZERO;
    }

    void Light::setTypeString(const std::string& type)
    {
        if (type == "point")
            this->setType(Ogre::Light::LT_POINT);
        else if (type == "directional")
            this->setType(Ogre::Light::LT_DIRECTIONAL);
        else if (type == "spotlight")
            this->setType(Ogre::Light::LT_SPOTLIGHT);
        else
            this->setType(Ogre::Light::LT_POINT);
    }

    std::string Light::getTypeString() const
    {
        switch (this->type_)
        {
            case Ogre::Light::LT_DIRECTIONAL:
                return "directional";
            case Ogre::Light::LT_SPOTLIGHT:
                return "spotlight";
            case Ogre::Light::LT_POINT:
            default:
                return "poinT";
        }
    }

    void Light::changedType()
    {
        this->light_->setType(this->type_);
    }

    void Light::changedVisibility()
    {
        SUPER(Light, changedVisibility);

        this->light_->setVisible(this->isVisible());
    }
}
