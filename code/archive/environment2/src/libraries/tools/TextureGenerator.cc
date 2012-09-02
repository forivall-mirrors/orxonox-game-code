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

/**
    @file
    @brief Implementation of the TextureGenerator.
*/

#include "TextureGenerator.h"

#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include "util/Convert.h"
#include "util/Math.h"

namespace std
{
    template <>
    inline bool less<orxonox::ColourValue>::operator()(const orxonox::ColourValue& __x, const orxonox::ColourValue& __y) const
    {
        if (__x.r == __y.r)
        {
            if (__x.g == __y.g)
            {
                if (__x.b == __y.b)
                {
                    return __x.a < __y.a;
                }
                return __x.b < __y.b;
            }
            return __x.g < __y.g;
        }
        return __x.r < __y.r;
    }
}

namespace orxonox
{
    std::map<std::string, std::map<ColourValue, std::string> > TextureGenerator::materials_s;
    unsigned int TextureGenerator::materialCount_s = 0;

    /*static*/ const std::string& TextureGenerator::getMaterialName(const std::string& textureName, const ColourValue& colour)
    {
        std::map<ColourValue, std::string>& colourMap = materials_s[textureName];
        std::map<ColourValue, std::string>::const_iterator it = colourMap.find(colour);

        if (it == colourMap.end())
        {
            const std::string& materialName = textureName + "_Material_" + multi_cast<std::string>(materialCount_s++);
            Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialName, "General"));
            material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            Ogre::TextureUnitState* textureUnitState = material->getTechnique(0)->getPass(0)->createTextureUnitState();
            textureUnitState->setTextureName(textureName);
            textureUnitState->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour);
            return (colourMap[colour] = materialName);
        }
        else
        {
            return it->second;
        }
    }
}
