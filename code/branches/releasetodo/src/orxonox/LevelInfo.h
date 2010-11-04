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

#ifndef _LevelInfo_H__
#define _LevelInfo_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include <string>
#include "core/BaseObject.h"

namespace orxonox // tolua_export
{  // tolua_export
    class _OrxonoxExport LevelInfo  // tolua_export
        : public BaseObject
    { // tolua_export
    
        public:
            LevelInfo(BaseObject* creator);
            virtual ~LevelInfo();
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setDescription(const std::string& description)
                { this->description_ = description; }
            inline const std::string& getDescription() const // tolua_export
                { return this->description_; }
                
            void setTags(const std::string& tags);
            inline bool addTag(const std::string& tag)
                { bool success = this->tags_.insert(tag).second; if(success) this->tagsUpdated(); return success; }
            inline const std::string& getTags(void) const
                { return this->tagsString_; }
            bool hasTag(const std::string& tag) { return this->tags_.find(tag) != this->tags_.end(); } // tolua_export
            
        private:
            void tagsUpdated(void);
        
            std::string description_;
            std::set<std::string> tags_;
            std::string tagsString_;
            
    }; // tolua_export
} // tolua_export

#endif /* _Level_H__ */
