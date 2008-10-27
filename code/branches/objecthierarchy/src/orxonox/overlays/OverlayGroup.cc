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

#include "OrxonoxStableHeaders.h"
#include "OverlayGroup.h"

#include "util/Debug.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/Iterator.h"
#include "core/XMLPort.h"
#include "OrxonoxOverlay.h"

namespace orxonox
{
    CreateFactory(OverlayGroup);

    SetConsoleCommand(OverlayGroup, toggleVisibility, false).accessLevel(AccessLevel::User);
    SetConsoleCommand(OverlayGroup, scaleGroup, false).accessLevel(AccessLevel::User);
    SetConsoleCommand(OverlayGroup, scrollGroup, false).accessLevel(AccessLevel::User);

    OverlayGroup::OverlayGroup(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(OverlayGroup);
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

        XMLPortParam(OverlayGroup, "scale",  setScale,  getScale,  xmlElement, mode).defaultValues(Vector2(1.0, 1.0));
        XMLPortParam(OverlayGroup, "scroll", setScroll, getScroll, xmlElement, mode).defaultValues(Vector2(0.0, 0.0));
        // loads all the child elements
        XMLPortObject(OverlayGroup, OrxonoxOverlay, "", addElement, getElement, xmlElement, mode);
    }

    //! Scales every element in the map.
    void OverlayGroup::setScale(const Vector2& scale)
    {
        for (std::map<std::string, OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it).second->scale(scale / this->scale_);
        this->scale_ = scale;
    }

    //! Scrolls every element in the map.
    void OverlayGroup::setScroll(const Vector2& scroll)
    {
        for (std::map<std::string, OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it).second->scroll(scroll - this->scroll_);
        this->scroll_ = scroll;
    }

    /**
    @brief
        Adds an element to the map (used when loading with XMLPort).
    @remarks
        The names of the OrxonoxOverlays have to be unique!
    */
    void OverlayGroup::addElement(OrxonoxOverlay* element)
    {
        if (hudElements_.find(element->getName()) != hudElements_.end())
        {
            COUT(1) << "Ambiguous names encountered while load the HUD overlays" << std::endl;
        }
        else
        {
            hudElements_[element->getName()] = element;
            element->setVisible(this->isVisible());
        }
    }

    //! Returns a different element as long as index < hudElements_.size().
    OrxonoxOverlay* OverlayGroup::getElement(unsigned int index)
    {
        if (index < this->hudElements_.size())
        {
            std::map<std::string, OrxonoxOverlay*>::const_iterator it = hudElements_.begin();
            for (unsigned int i = 0; i != index; ++it, ++i)
                ;
            return (*it).second;
        }
        else
            return 0;
    }

    //! Changes the visibility of all elements
    void OverlayGroup::changedVisibility()
    {
        for (std::map<std::string, OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
            (*it).second->setVisible(this->isVisible());
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
