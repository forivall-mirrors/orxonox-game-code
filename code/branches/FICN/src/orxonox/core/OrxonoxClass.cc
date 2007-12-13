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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/*!
    @file OrxonoxClass.cc
    @brief Implementation of the OrxonoxClass Class.
*/

#include "OrxonoxClass.h"

namespace orxonox
{
    /** @brief Constructor: Sets the default values. */
    OrxonoxClass::OrxonoxClass()
    {
        this->identifier_ = 0;
        this->parents_ = 0;

        this->bActive_ = true;
        this->bVisible_ = true;
    }

    /** @brief Destructor: Deletes, if existing, the list of the parents. */
    OrxonoxClass::~OrxonoxClass()
    {
        // parents_ exists only if isCreatingHierarchy() of the associated Identifier returned true while creating the class
        if (this->parents_)
            delete this->parents_;
    }
}
