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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief Definition of the OverlayGroup class.
*/

#include "OverlayGroup.h"

#include "util/Debug.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/ObjectList.h"
#include "core/XMLPort.h"
#include "OrxonoxOverlay.h"

namespace orxonox
{
    CreateFactory(OverlayGroup);

    SetConsoleCommand(OverlayGroup, toggleVisibility, false).accessLevel(AccessLevel::User);
    SetConsoleCommand(OverlayGroup, scaleGroup, false).accessLevel(AccessLevel::User);
    SetConsoleCommand(OverlayGroup, scrollGroup, false).accessLevel(AccessLevel::User);

    OverlayGroup::OverlayGroup(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(OverlayGroup);

        this->owner_ = 0;

        setScale(Vector2(1.0, 1.0));
        setScroll(Vector2(0.0, 0.0));
    }

    OverlayGroup::~OverlayGroup()
    {
        for (std::set<OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            delete (*it);
    }

    /**
    @brief
        Loads the group and all its children OrxonoxOverlays.
    @copydoc
        BaseObject::XMLPort()
    */
    void OverlayGroup::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(OverlayGroup, XMLPort, xmlElement, mode);

        XMLPortParam(OverlayGroup, "scale",  setScale,  getScale,  xmlElement, mode);
        XMLPortParam(OverlayGroup, "scroll", setScroll, getScroll, xmlElement, mode);
        // loads all the child elements
        XMLPortObject(OverlayGroup, OrxonoxOverlay, "", addElement, getElement, xmlElement, mode);
    }

    //! Scales every element in the set.
    void OverlayGroup::setScale(const Vector2& scale)
    {
        for (std::set<OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->scale(scale / this->scale_);
        this->scale_ = scale;
    }

    //! Scrolls every element in the set.
    void OverlayGroup::setScroll(const Vector2& scroll)
    {
        for (std::set<OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->scroll(scroll - this->scroll_);
        this->scroll_ = scroll;
    }

    /**
    @brief
        Adds an element to the set (used when loading with XMLPort).
    @remarks
        The names of the OrxonoxOverlays have to be unique!
    */
    void OverlayGroup::addElement(OrxonoxOverlay* element)
    {
        hudElements_.insert(element);
        element->setVisible(this->isVisible());
        if (this->owner_)
            element->setOwner(this->owner_);
    }

    /**
    @brief
        Removes an element from the map.
    @param name
        The name of the element that is removed.
    @return
        Returns true if there was such an element to remove, false if not.
    */
    bool OverlayGroup::removeElement(OrxonoxOverlay* element)
    {
        if(this->hudElements_.erase(element) == 0)
            return false;
        return true;
    }

    //! Returns a different element as long as index < hudElements_.size().
    OrxonoxOverlay* OverlayGroup::getElement(unsigned int index)
    {
        if (index < this->hudElements_.size())
        {
            std::set<OrxonoxOverlay*>::const_iterator it = hudElements_.begin();
            for (unsigned int i = 0; i != index; ++it, ++i)
                ;
            return (*it);
        }
        else
            return 0;
    }

    //! Changes the visibility of all elements
    void OverlayGroup::changedVisibility()
    {
        for (std::set<OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->setVisible(this->isVisible());
    }

    void OverlayGroup::setOwner(BaseObject* owner)
    {
        this->owner_ = owner;

        for (std::set<OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->setOwner(owner);
    }

    //########### Console commands ############

    /**
    @brief
        Hides/shows an overlay group by its name.
    @param name
        The name of the group defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    */
    /*static*/ void OverlayGroup::toggleVisibility(const std::string& name)
    {
        for (ObjectList<OverlayGroup>::iterator it = ObjectList<OverlayGroup>::begin(); it; ++it)
        {
            if ((*it)->getName() == name)
                (*it)->setVisible(!((*it)->isVisible()));
        }
    }

    /**
    @brief
        Scales an overlay group by its name.
    @param name
        The name of the group defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    */
    /*static*/ void OverlayGroup::scaleGroup(const std::string& name, float scale)
    {
        for (ObjectList<OverlayGroup>::iterator it = ObjectList<OverlayGroup>::begin(); it; ++it)
        {
            if ((*it)->getName() == name)
                (*it)->scale(Vector2(scale, scale));
        }
    }

    /**
    @brief
        Scrolls an overlay group by its name.
    @param name
        The name of the group defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    */
    /*static*/ void OverlayGroup::scrollGroup(const std::string& name, const Vector2& scroll)
    {
        for (ObjectList<OverlayGroup>::iterator it = ObjectList<OverlayGroup>::begin(); it; ++it)
        {
            if ((*it)->getName() == name)
                (*it)->scroll(scroll);
        }
    }
}
