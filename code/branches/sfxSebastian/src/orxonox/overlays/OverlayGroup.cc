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

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/command/ConsoleCommand.h"
#include "OrxonoxOverlay.h"

namespace orxonox
{
    RegisterClass(OverlayGroup);

    SetConsoleCommand("OverlayGroup", "toggleVisibility", &OverlayGroup::toggleVisibility);
    SetConsoleCommand("OverlayGroup", "show", &OverlayGroup::show);
    SetConsoleCommand("OverlayGroup", "scaleGroup",       &OverlayGroup::scaleGroup);
    SetConsoleCommand("OverlayGroup", "scrollGroup",      &OverlayGroup::scrollGroup);

    OverlayGroup::OverlayGroup(Context* context)
        : BaseObject(context)
    {
        RegisterObject(OverlayGroup);

        this->owner_ = 0;

        setScale(Vector2(1.0, 1.0));
        setScroll(Vector2(0.0, 0.0));
    }

    OverlayGroup::~OverlayGroup()
    {
        for (std::set< SmartPtr<OrxonoxOverlay> >::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->destroy();
        this->hudElements_.clear();
    }

    /**
    @brief
        Loads the group and all its children OrxonoxOverlays.
    @copydoc
        BaseObject::XMLPort()
    */
    void OverlayGroup::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(OverlayGroup, XMLPort, xmlelement, mode);

        XMLPortParam(OverlayGroup, "scale",  setScale,  getScale,  xmlelement, mode);
        XMLPortParam(OverlayGroup, "scroll", setScroll, getScroll, xmlelement, mode);
        // loads all the child elements
        XMLPortObject(OverlayGroup, OrxonoxOverlay, "", addElement, getElement, xmlelement, mode);
    }

    //! Scales every element in the set.
    void OverlayGroup::setScale(const Vector2& scale)
    {
        for (std::set< SmartPtr<OrxonoxOverlay> >::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->scale(scale / this->scale_);
        this->scale_ = scale;
    }

    //! Scrolls every element in the set.
    void OverlayGroup::setScroll(const Vector2& scroll)
    {
        for (std::set< SmartPtr<OrxonoxOverlay> >::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
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
        hudElements_.insert(SmartPtr<OrxonoxOverlay>(element));
        element->setOverlayGroup( this );
        if (this->owner_)
            element->setOwner(this->owner_);
    }

    /**
    @brief
        Removes an element from the map.
    @param element
        A pointer to the element that is removed.
    @return
        Returns true if there was such an element to remove, false if not.
    */
    bool OverlayGroup::removeElement(OrxonoxOverlay* element)
    {
        if(this->hudElements_.erase(SmartPtr<OrxonoxOverlay>(element)) == 0)
            return false;
        return true;
    }

    //! Returns a different element as long as index < hudElements_.size().
    OrxonoxOverlay* OverlayGroup::getElement(unsigned int index)
    {
        if (index < this->hudElements_.size())
        {
            std::set< SmartPtr<OrxonoxOverlay> >::const_iterator it = hudElements_.begin();
            for (unsigned int i = 0; i != index; ++it, ++i)
                ;
            return it->get();
        }
        else
            return 0;
    }

    //! Changes the visibility of all elements
    void OverlayGroup::changedVisibility()
    {
        SUPER( OverlayGroup, changedVisibility );

        for (std::set< SmartPtr<OrxonoxOverlay> >::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->changedVisibility(); //inform all Child Overlays that our visibility has changed
    }

    //! Changes the gametype of all elements
    void OverlayGroup::changedGametype()
    {
        SUPER( OverlayGroup, changedGametype );

        for (std::set< SmartPtr<OrxonoxOverlay> >::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it)->setGametype(this->getGametype());
    }

    void OverlayGroup::setOwner(BaseObject* owner)
    {
        this->owner_ = owner;

        for (std::set< SmartPtr<OrxonoxOverlay> >::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
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
        Shows an overlay group by its name.
    @param name
        The name of the group defined BaseObject::setName() (usually done with the "name" attribute in the xml file).
    */
    /*static*/ void OverlayGroup::show(const std::string& name)
    {
        for (ObjectList<OverlayGroup>::iterator it = ObjectList<OverlayGroup>::begin(); it; ++it)
        {
            if ((*it)->getName() == name)
            {
                if((*it)->isVisible())
                    (*it)->changedVisibility();
                else
                    (*it)->setVisible(!((*it)->isVisible()));
            }
        }
    }

    /**
    @brief
        Scales an overlay group by its name.
    @param name
        The name of the group defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    @param scale
        The scaling factor
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
    @param scroll
        The relative translation of the overlay group
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
