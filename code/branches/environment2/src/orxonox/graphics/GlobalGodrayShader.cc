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
 *      Markus 'atok' Wegmann
 *   Co-authors:
 *      ...
 *
 */

/**
 @file
 
 @brief Implementation of the GlobalGodrayShader class.
 */

#include "GlobalGodrayShader.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(GlobalGodrayShader);
    
    GlobalGodrayShader::GlobalGodrayShader(BaseObject* creator) : BaseObject(creator), globalShader_(creator)
    {
        RegisterObject(GlobalGodrayShader);
        
        if (!this->getScene())
            ThrowException(AbortLoading, "Can't create GlobalGodrayShader, no scene given.");
        //        if (!this->getScene()->getSceneManager())
        //            ThrowException(AbortLoading, "Can't create GlobalGodrayShader, no scene manager given.");
        
        this->skyColor_ = ColourValue::Black;
        this->exposure_ = 1.0f;
        this->decay_ = 0.1f;
        this->density_ = 0.7f;
        
        //// Init ////
    }
    
    GlobalGodrayShader::~GlobalGodrayShader()
    {
        this->setVisible(false);
    }
    
    void GlobalGodrayShader::tick(float dt)
    {
        // To-Do
    }
    
    void GlobalGodrayShader::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GlobalGodrayShader, XMLPort, xmlelement, mode);
        
        XMLPortParamTemplate(GlobalGodrayShader, "sunPosition", setSunPosition, getSunPosition, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(GlobalGodrayShader, "skyColor", setSkyColor, getSkyColor, xmlelement, mode, const ColourValue&);
        XMLPortParamVariable(GlobalGodrayShader, "exposure", exposure_, xmlelement, mode);
        XMLPortParamVariable(GlobalGodrayShader, "decay", decay_, xmlelement, mode);
        XMLPortParamVariable(GlobalGodrayShader, "density", density_, xmlelement, mode);
    }
    
    void GlobalGodrayShader::setSunPosition(const Vector3& position)
    {
        this->sunPosition_ = position;
    }
    void GlobalGodrayShader::setSkyColor(const ColourValue& color)
    {
        this->skyColor_ = color;
    }
    const Vector3& GlobalGodrayShader::getSunPosition() const
    {
        return this->sunPosition_;
    }
    const ColourValue& GlobalGodrayShader::getSkyColor() const
    {
        return this->skyColor_;
    }
    
    void GlobalGodrayShader::changedVisibility()
    {
        SUPER(GlobalGodrayShader, changedVisibility);
        
        this->globalShader_.setVisible(this->isVisible());
    }
}
