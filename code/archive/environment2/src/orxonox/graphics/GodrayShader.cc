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
 
 @brief Implementation of the GodrayShader class.
 */


#include "GodrayShader.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(GodrayShader);
    
    GodrayShader::GodrayShader(BaseObject* creator) : BaseObject(creator), globalShader_(creator)
    {
        RegisterObject(GodrayShader);
        
        // Default parameters are initialized
        this->sunPosition_ = Vector3::Zero;
        this->sunColor_ = ColourValue::White;
        this->skyColor_ = ColourValue::Black;
        this->exposure_ = 1.0f; 
        this->decay_ = 0.1f;
        this->density_ = 0.7f;
        
        // Godrays.compositor is getting initialized
        this->globalShader_.getMutableShader().setCompositorName("Godrays");
        this->compositor_ = this->globalShader_.getMutableShader().getMutableCompositorInstance();
    }
    
    GodrayShader::~GodrayShader()
    {
        this->setVisible(false);
    }
    
    void GodrayShader::tick(float dt)
    {
        if(this->isVisible())
        {
            
            //Reset sunPosition in the compositor instance every tick
            this->compositor_->getTechnique()->getTargetPass(2)
        }
    }
    
    void GodrayShader::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GodrayShader, XMLPort, xmlelement, mode);
        
        XMLPortParamTemplate(GodrayShader, "sunColor", setSunColor, getSunColor, xmlelement, mode, const ColourValue&);
        XMLPortParamTemplate(GodrayShader, "skyColor", setSkyColor, getSkyColor, xmlelement, mode, const ColourValue&);
        XMLPortParamVariable(GodrayShader, "exposure", exposure_, xmlelement, mode);
        XMLPortParamVariable(GodrayShader, "decay", decay_, xmlelement, mode);
        XMLPortParamVariable(GodrayShader, "density", density_, xmlelement, mode);
    }
    
    void GodrayShader::setWorldEntity(WorldEntity* worldEntity)
    {
        this->worldEntity_ = worldEntity;
        this->setSunPosition(worldEntity->getWorldPosition());
    }
    
    void GodrayShader::setSunPosition(const Vector3& position)
    {
        this->sunPosition_ = position;
    }
    void GodrayShader::setSunColor(const ColourValue& color)
    {
        this->skyColor_ = color;
    }
    void GodrayShader::setSkyColor(const ColourValue& color)
    {
        this->skyColor_ = color;
    }
    const Vector3& GodrayShader::getSunPosition() const
    {
        return this->sunPosition_;
    }
    const ColourValue& GodrayShader::getSunColor() const
    {
        return this->sunColor_;
    }
    const ColourValue& GodrayShader::getSkyColor() const
    {
        return this->skyColor_;
    }
    
    void GodrayShader::changedVisibility()
    {
        SUPER(GodrayShader, changedVisibility);
        
        this->globalShader_.setVisible(this->isVisible());
    }
}
