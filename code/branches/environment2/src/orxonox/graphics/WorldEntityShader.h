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
 *      Markus 'atok' Wegmann
 *   Co-authors:
 *      ...
 *
 */

/**
 @file
    Contains the header of the WorldEntityShader class.
 @brief
    WorldEntityShader will be used in the future for "non-material" shaders that depend on WorldEntity data of the creator (such as their world position, orientation, etc.). "Non-material" means that they are not depending on a polygon-mesh of the creator (but can), simply rendering them. Therefore its best purpose is to implement screenspace-effects (e.g. Compositor).
    Motivation & Advantages: 
        - WorldEntityShader's classes can be initalized and attached in every other WorldEntity object by XML. This makes leveldesign much more easy.
        - No id managment of the creators has to be done. Everything can be nested.
    
    Case scenarios would be coronas (position), godrays, etc.
 
    Some sort of implementation already exists: The GodrayShader class uses the same approach. It can be used as a (raw) template.
 **/


        