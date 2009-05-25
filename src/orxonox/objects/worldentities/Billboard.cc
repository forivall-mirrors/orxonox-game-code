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
#include "Billboard.h"

#include <OgreBillboardSet.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/GameMode.h"
#include "objects/Scene.h"

namespace orxonox
{
    CreateFactory(Billboard);

    Billboard::Billboard(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Billboard);

        this->material_ = "";
        this->colour_ = ColourValue::White;
//        this->rotation_ = 0;

        this->registerVariables();
    }

    Billboard::~Billboard()
    {
        if (this->isInitialized())
        {
            if (this->isInitialized() && this->billboard_.getBillboardSet())
                this->detachOgreObject(this->billboard_.getBillboardSet());
        }
    }

    void Billboard::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Billboard, XMLPort, xmlelement, mode);

        XMLPortParam(Billboard, "material", setMaterial, getMaterial, xmlelement, mode);
        XMLPortParam(Billboard, "colour",   setColour,   getColour,   xmlelement, mode).defaultValues(ColourValue::White);
//        XMLPortParam(Billboard, "rotation", setRotation, getRotation, xmlelement, mode).defaultValues(0);
    }

    void Billboard::registerVariables()
    {
        registerVariable(this->material_, variableDirection::toclient, new NetworkCallback<Billboard>(this, &Billboard::changedMaterial));
        registerVariable(this->colour_,   variableDirection::toclient, new NetworkCallback<Billboard>(this, &Billboard::changedColour));
//        registerVariable(this->rotation_, variableDirection::toclient, new NetworkCallback<Billboard>(this, &Billboard::changedRotation));
    }

    void Billboard::changedMaterial()
    {
        if (this->material_ == "")
            return;

        if (!this->billboard_.getBillboardSet())
        {
            if (this->getScene() && GameMode::showsGraphics())
            {
                this->billboard_.setBillboardSet(this->getScene()->getSceneManager(), this->material_, this->colour_, 1);
                if (this->billboard_.getBillboardSet())
                     this->attachOgreObject(this->billboard_.getBillboardSet());
                this->billboard_.setVisible(this->isVisible());
//                this->changedRotation();
            }
        }
        else
            this->billboard_.setMaterial(this->material_);
    }

    void Billboard::changedColour()
    {
        if (!this->billboard_.getBillboardSet())
        {
/*
            if (this->getScene() && GameMode::showsGraphics() && (this->material_ != ""))
            {
                this->billboard_.setBillboardSet(this->getScene()->getSceneManager(), this->material_, this->colour_, 1);
                if (this->billboard_.getBillboardSet())
                    this->attachOgreObject(this->billboard_.getBillboardSet());
                this->billboard_.setVisible(this->isVisible());
            }
*/
        }
        else
            this->billboard_.setColour(this->colour_);
    }
    
/*
    void Billboard::changedRotation()
    {
        if (this->billboard_.getBillboardSet())
            this->billboard_.getBillboardSet()->setRotation(this->rotation_);
    }
*/

    void Billboard::changedVisibility()
    {
        SUPER(Billboard, changedVisibility);

        this->billboard_.setVisible(this->isVisible());
    }
}
