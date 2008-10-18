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

#ifndef _LevelInfo_H__
#define _LevelInfo_H__

#include "OrxonoxPrereqs.h"

#include "Info.h"
#include "util/Math.h"
#include "core/Identifier.h"

#include "objects/gametypes/Gametype.h"

namespace orxonox
{
    class _OrxonoxExport LevelInfo : public Info
    {
        public:
            LevelInfo();
            virtual ~LevelInfo() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline void setDescription(const std::string& description)
                { this->description_ = description; }
            inline const std::string& getDescription() const
                { return this->description_; }

            void setSkybox(const std::string& skybox);
            inline const std::string& getSkybox() const
                { return this->skybox_; }

            void setAmbientLight(const ColourValue& colour);
            inline const ColourValue& getAmbientLight() const
                { return this->ambientLight_; }

            void setGametype(const std::string& gametype);
            inline const std::string& getGametype() const
                { return this->gametype_; }

        private:
            void applySkybox()
                { this->setSkybox(this->skybox_); }
            void applyAmbientLight()
                { this->setAmbientLight(this->ambientLight_); }

            std::string description_;
            std::string skybox_;
            ColourValue ambientLight_;
            std::string gametype_;
            SubclassIdentifier<Gametype> gametypeIdentifier_;
            Gametype* gametypeInstance_;
    };
}

#endif /* _LevelInfo_H__ */
