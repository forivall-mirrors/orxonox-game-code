/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

/**
    @file BaseObject.h
    @brief Definition of the BaseObject class.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "CorePrereqs.h"
#include "util/XMLIncludes.h"

#include "CoreIncludes.h"

namespace orxonox
{
    //! The BaseObject is the parent of all classes representing an instance in the game.
    class _CoreExport BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual Element& XMLPort(Element& xmlelement, bool loading);

            /** @brief Sets the name of the object. @param name The name */
            inline void setName(const std::string& name) { this->name_ = name; this->changedName(); }
            /** @brief Returns the name of the object. @return The name */
            inline const std::string& getName() const { return this->name_; }
            /** @brief This function gets called if the name of the object changes. */
            virtual void changedName() {}

            /** @brief Sets the state of the objects activity. @param bActive True = active */
            inline void setActivity(bool bActive) { this->bActive_ = bActive; this->changedActivity(); }
            /** @brief Returns the state of the objects activity. @return The state of the activity */
            inline const bool isActive() const { return this->bActive_; }
            /** @brief This function gets called if the activity of the object changes. */
            virtual void changedActivity() {}

            /** @brief Sets the state of the objects visibility. @param bVisible True = visible */
            inline void setVisibility(bool bVisible) { this->bVisible_ = bVisible; this->changedVisibility(); }
            /** @brief Returns the state of the objects visibility. @return The state of the visibility */
            inline const bool isVisible() const { return this->bVisible_; }
            /** @brief This function gets called if the visibility of the object changes. */
            virtual void changedVisibility() {}

            /** @brief Sets a pointer to the level that loaded this object. @param level The pointer to the level */
            inline void setLevel(const Level* level) { this->level_ = level; }
            /** @brief Returns a pointer to the level that loaded this object. @return The level */
            inline const Level* getLevel() const { return this->level_; }

        private:
            std::string name_;                          //!< The name of the object
            bool bActive_;                              //!< True = the object is active
            bool bVisible_;                             //!< True = the object is visible
            const Level* level_;                        //!< The level that loaded this object
    };
}

#endif /* _BaseObject_H__ */
