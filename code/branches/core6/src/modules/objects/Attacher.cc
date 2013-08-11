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

#include "Attacher.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(Attacher);

    Attacher::Attacher(Context* context) : StaticEntity(context)
    {
        RegisterObject(Attacher);

        this->target_ = 0;
    }

    void Attacher::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Attacher, XMLPort, xmlelement, mode);

        XMLPortParam(Attacher, "target", setTarget, getTarget, xmlelement, mode);
        XMLPortObject(Attacher, WorldEntity, "", addObject, getObject, xmlelement, mode);
    }

    void Attacher::processEvent(Event& event)
    {
        if (this->target_)
            this->target_->processEvent(event);
    }

    void Attacher::changedActivity()
    {
        SUPER(Attacher, changedActivity);

        for (std::list<WorldEntity*>::iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
            (*it)->setActive(this->isActive());
    }

    void Attacher::changedVisibility()
    {
        SUPER(Attacher, changedVisibility);

        for (std::list<WorldEntity*>::iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
            (*it)->setVisible(this->isVisible());
    }

    void Attacher::addObject(WorldEntity* object)
    {
        this->objects_.push_back(object);

        this->attach(object);
    }

    WorldEntity* Attacher::getObject(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<WorldEntity*>::const_iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
        {
            if (i == index)
                return (*it);

            ++i;
        }
        return 0;
    }

    void Attacher::setTarget(const std::string& target)
    {
        this->targetname_ = target;
        this->target_ = 0;

        if (this->targetname_.empty())
            return;

        for (ObjectList<WorldEntity>::iterator it = ObjectList<WorldEntity>::begin(); it != ObjectList<WorldEntity>::end(); ++it)
        {
            if (it->getName() == this->targetname_)
            {
                this->target_ = *it;
                this->attachToParent(*it);
            }
        }
    }

    void Attacher::loadedNewXMLName(BaseObject* object)
    {
        if (this->target_ || this->targetname_.empty())
            return;

        WorldEntity* entity = orxonox_cast<WorldEntity*>(object);
        if (entity && entity->getName() == this->targetname_)
        {
            this->target_ = entity;
            this->attachToParent(entity);
        }
    }
}
