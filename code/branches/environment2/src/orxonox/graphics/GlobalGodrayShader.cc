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
 
 @brief Implementation of the GlobalGodrayShader.
 */

#include "GlobalShader.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(GlobalGodrayShader);
    
    GlobalGodrayShader::GlobalGodrayShader(BaseObject* creator, WorldEntity blurObject) : BaseObject(creator)
    {
        RegisterObject(GlobalGodrayShader);
        
        if (!this->getScene())
            ThrowException(AbortLoading, "Can't create GlobalGodrayShader, no scene given.");
        //        if (!this->getScene()->getSceneManager())
        //            ThrowException(AbortLoading, "Can't create GlobalGodrayShader, no scene manager given.");
        
        if (this->getScene()->getSceneManager())
            this- setSceneManager(this->getScene()->getSceneManager());
    }
    
    GlobalGodrayShader::~GlobalShader()
    {
    }
    
    void GlobalGodrayShader::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GlobalGodrayShader, XMLPort, xmlelement, mode);
        
        XMLPortParamExtern(GlobalShader, Shader, &this->shader_, "compositor", setCompositorName, getCompositorName, xmlelement, mode);
    }
}
