/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#ifndef _LoaderPlatform_H__
#define _LoaderPlatform_H__


#if defined( __WIN32__ ) || defined( _WIN32 )
  #ifdef LOADER_BUILD
    #define _LoaderExport __declspec( dllexport )
  #elif defined(LOADER_STATIC)
    #define _LoaderExport
  #else
    #define _LoaderExport __declspec( dllimport )
  #endif
#else
  #define _LoaderExport
#endif


#endif /* _LoaderPlatform_H__ */
