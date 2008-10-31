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
 
#ifndef _QuestItem_H__
#define _QuestItem_H__

#include <string>

#include "core/BaseObject.h"
#include "core/XMLPort.h"
#include "QuestDescription.h"

namespace orxonox {

    
    /**
    @brief
	Functions as a base class for Quest classes such as Quest or QuestHint.
	Has a unique identifier and a description.
    @author
	Damian 'Mozork' Frick
    */
    class QuestItem : public BaseObject
    {
    
	public:
            QuestItem();
	    virtual ~QuestItem();
	    
	    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	    
	    inline const std::string & getId(void) const //!< Returns the id of this quest.
                { return this->id_; }
	    inline const QuestDescription* getDescription(void) const //!< Returns the description of the QuestItem.
                { return this->description_; }
	    //const QuestDescription* getDescription(unsigned int index) const; //!< Returns the description of the QuestItem.
                
	    static bool isId(const std::string & id); //!< Checks whether a given id is valid.
	    
	protected:
            inline void setId(const std::string & id)
                { id_ = id; }
	    inline void setDescription(QuestDescription* description)
                { this->description_ = description; }
	    
	private:
	    std::string id_; //!< Identifier. Should be of GUID form: http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure
	    QuestDescription* description_; //!< The description of the QuestItem.
	    
	    void initialize(void); //!< Initializes the object.
    
    };

}

#endif /* _QuestItem_H__ */
