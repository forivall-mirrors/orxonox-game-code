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

#include "GlobalShader.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "Scene.h"

namespace orxonox
{
    CreateFactory(GlobalShader);

    GlobalShader::GlobalShader(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(GlobalShader);

        if (!this->getScene())
            ThrowException(AbortLoading, "Can't create GlobalShader, no scene given.");
//        if (!this->getScene()->getSceneManager())
//            ThrowException(AbortLoading, "Can't create GlobalShader, no scene manager given.");

        if (this->getScene()->getSceneManager())
            this->shader_.setSceneManager(this->getScene()->getSceneManager());

        this->registerVariables();
    }

    GlobalShader::~GlobalShader()
    {
    }

    void GlobalShader::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GlobalShader, XMLPort, xmlelement, mode);

        XMLPortParamExtern(GlobalShader, Shader, &this->shader_, "compositor", setCompositorName, getCompositorName, xmlelement, mode);
    }

    void GlobalShader::registerVariables()
    {
        registerVariable(this->bVisible_,                                         VariableDirection::ToClient, new NetworkCallback<GlobalShader>(this, &GlobalShader::changedVisibility));
        registerVariable(const_cast<std::string&>(this->shader_.getCompositorName()), VariableDirection::ToClient, new NetworkCallback<Shader>(&this->shader_, &Shader::changedCompositorName));
    }

    void GlobalShader::changedVisibility()
    {
        SUPER(GlobalShader, changedVisibility);

        this->shader_.setVisible(this->isVisible());
    }
}
