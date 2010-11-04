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

#include "LevelInfo.h"

#include <sstream>
#include <vector>

#include "util/SubString.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "LevelManager.h"

namespace orxonox
{
    CreateFactory(LevelInfo);

    LevelInfo::LevelInfo(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(LevelInfo);

    }

    LevelInfo::~LevelInfo()
    {
        
    }

    void LevelInfo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LevelInfo, XMLPort, xmlelement, mode);

        XMLPortParam(LevelInfo, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(LevelInfo, "tags", setTags, getTags, xmlelement, mode).defaultValues("");
    }
    
    void LevelInfo::setTags(const std::string& tags)
    {
        const std::vector<std::string>& strings = SubString(tags, ",", " ").getAllStrings();
        for(std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); it++)
            this->addTag(*it);
            
        this->tagsUpdated();
    }
    
    void LevelInfo::tagsUpdated(void)
    {
        std::stringstream stream;
        std::set<std::string>::const_iterator temp;
        for(std::set<std::string>::const_iterator it = this->tags_.begin(); it != this->tags_.end(); )
        {
            temp = it;
            if(++it == this->tags_.end()) // If this is the last tag.
                stream << *temp;
            else
                stream << *temp << ", ";
        }
            
        this->tagsString_ = stream.str();
    }
    
}
    
