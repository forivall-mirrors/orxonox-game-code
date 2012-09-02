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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 */

/**
@file
@brief
    std::sring to Ogre::UTFString conversion functions
*/

#ifndef _DisplayStringConversions_H__
#define _DisplayStringConversions_H__

#include "UtilPrereqs.h"
#include <OgreOverlayElement.h>

#if OGRE_UNICODE_SUPPORT
    #include <OgreUTFString.h>

    namespace orxonox
    {
        template <>
        struct ConverterExplicit<std::string, Ogre::DisplayString>
        {
            //! Converts an std::string into an Ogre::UTFString
            inline static bool convert(Ogre::DisplayString* output, const std::string& input)
            {
                Ogre::UTFString::code_point cp;
                for (unsigned int i = 0; i < input.size(); ++i)
                {
                  cp = input[i];
                  cp &= 0xFF;
                  output->append(1, cp);
                }
                return true;
            }
        };
    }
#else
    namespace orxonox
    {
        template <>
        struct ConverterExplicit<std::string, Ogre::DisplayString>
        {
            //! Converts an std::string into an Ogre::String
            inline static bool convert(Ogre::DisplayString* output, const std::string& input)
            {
                *output = input;
                return true;
            }
        };
    }
#endif

#endif /* _DisplayStringConversions_H__ */
