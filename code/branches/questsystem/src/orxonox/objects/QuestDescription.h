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

#ifndef _QuestDescription_H__
#define _QuestDescription_H__

#include <string>

#include "core/BaseObject.h"

namespace orxonox {

    /**
    @brief
        This class is a description of a QuestItem.
        It holds a title and a description.
    @author
        Damian 'Mozork' Frick
    */
    class QuestDescription : public BaseObject {
    
	public:
            QuestDescription();
	    QuestDescription(std::string title, std::string description = "");
	    ~QuestDescription();
	    
	    inline const std::string & getTitle(void) const //!< Returns the title.
                { return this->title_; }
	    inline const std::string & getDescription(void) const //!< Returns the description text.
                { return this->description_; }
	    
	private:
            void initialize(void);
		
            std::string title_;
            std::string description_;
    
    };

}

#endif /* _QuestDescription_H__ */
