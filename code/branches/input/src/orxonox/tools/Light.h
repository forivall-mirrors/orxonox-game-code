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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Light_H__
#define _Light_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include <OgreLight.h>

#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport Light
    {
        public:
            Light();
            ~Light();
            void setLight(Ogre::Light::LightTypes type = Ogre::Light::LT_POINT, const ColourValue& diffuse = ColourValue(1.0, 1.0, 1.0), const ColourValue& specular = ColourValue(1.0, 1.0, 1.0));

            inline Ogre::Light* getLight()
                { return this->light_; }

            inline const std::string& getName() const
                { return this->light_->getName(); }

        private:
            static unsigned int lightCounter_s;
            Ogre::Light* light_;
    };
}

#endif /* _Light_H__ */
