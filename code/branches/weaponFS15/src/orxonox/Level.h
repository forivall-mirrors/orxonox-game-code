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

#ifndef _Level_H__
#define _Level_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <string>
#include <map>
#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"
#include "graphics/MeshLodInformation.h"

namespace orxonox
{
    class _OrxonoxExport Level : public BaseObject, public Synchronisable, public Context
    {
        public:
            Level(Context* context);
            virtual ~Level();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void playerEntered(PlayerInfo* player);
            void playerLeft(PlayerInfo* player);

            MeshLodInformation* getLodInfo(std::string meshName) const;


        private:
            void registerVariables();
            void addObject(BaseObject* object);
            BaseObject* getObject(unsigned int index) const;

            void addLodInfo(MeshLodInformation* object);
            void networkCallbackTemplatesChanged();
//            const MeshLodInformation* getLodInfo(std::string meshName) const;
//            MeshLodInformation* getLodInfo(unsigned int index) const;

            void setGametypeString(const std::string& gametype);
            inline const std::string& getGametypeString() const
                { return this->gametype_; }

            void networkcallback_applyXMLFile();

            std::string                    gametype_;
            std::string                    xmlfilename_;
            XMLFile*                       xmlfile_;
            std::list<BaseObject*>         objects_;
            std::map<std::string,MeshLodInformation*>  lodInformation_;
    };
}

#endif /* _Level_H__ */
