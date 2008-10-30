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

#ifndef _Light_H__
#define _Light_H__

#include "OrxonoxPrereqs.h"
#include "PositionableEntity.h"

#include <string>
#include <OgreLight.h>

#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport Light : public PositionableEntity
    {
        public:
            Light(BaseObject* creator);
            virtual ~Light();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void changedVisibility();

            inline Ogre::Light* getLight()
                { return this->light_; }

            const std::string& getName() const;

            inline void setType(Ogre::Light::LightTypes type)
                { this->type_ = type; this->changedType(); }
            Ogre::Light::LightTypes getType() const
                { return this->type_; }

            void setDiffuseColour(const ColourValue& colour);
            const ColourValue& getDiffuseColour() const;

            void setSpecularColour(const ColourValue& colour);
            const ColourValue& getSpecularColour() const;

            void setDirection(const Vector3& direction);
            const Vector3& getDirection() const;

        private:
            void setTypeString(const std::string& type);
            std::string getTypeString() const;

            void changedType();

            static unsigned int lightCounter_s;
            Ogre::Light* light_;
            Ogre::Light::LightTypes type_;

    };
}

#endif /* _Light_H__ */
