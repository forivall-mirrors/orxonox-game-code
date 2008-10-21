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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

#include "core/CoreInculdes.h"
#include "QuestDescription.h"

namespace orxonox {

    CreateFactory(QuestDescription);
    
    QuestDescription(std::string title, std::string description = "") : OrxonoxClass()
    {
        initialize();
        this->title_ = title;
        this->description_ = description;
    }
    
    QuestDescription::~QuestDescription()
    {
        
    }
    
    /**
    @brief
        Initializes the object. Has to be called first in every constructor of this class.
    */
    void QuestDescription::initialize(void)
    {
        RegisterRootObject(QuestDescription);
    }


}
