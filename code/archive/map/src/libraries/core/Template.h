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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Template_H__
#define _Template_H__

#include "CorePrereqs.h"

#include <map>
#include <string>
#include "BaseObject.h"

namespace orxonox
{
    class _CoreExport Template : public BaseObject
    {
        public:
            Template(BaseObject* creator);
            virtual ~Template();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedName();

            inline void setLink(const std::string& link)
                { this->link_ = link; this->bIsLink_ = !link.empty(); }
            inline const std::string& getLink() const
                { return this->link_; }

            inline void setLoadDefaults(bool bLoadDefaults)
                { this->bLoadDefaults_ = bLoadDefaults; }
            inline bool getLoadDefaults() const
                { return this->bLoadDefaults_; }

            void setXMLElement(const TiXmlElement& xmlelement);
            const TiXmlElement& getXMLElement() const;

            void setBaseclass(const std::string& baseclass);
            inline const std::string& getBaseclass() const
                { return this->baseclass_; }
            inline Identifier* getBaseclassIdentifier() const
                { return this->baseclassIdentifier_; }

            void applyOn(BaseObject* object);

            static std::map<std::string, Template*>& getTemplateMap();
            static Template* getTemplate(const std::string& name);
            static void apply(const std::string& name, BaseObject* object);

        private:
            TiXmlElement* xmlelement_;
            std::string link_;
            std::string baseclass_;
            Identifier* baseclassIdentifier_;
            bool bIsLink_;
            bool bLoadDefaults_;
            mutable bool bIsReturningXMLElement_;
    };
}

#endif /* _Template_H__ */
