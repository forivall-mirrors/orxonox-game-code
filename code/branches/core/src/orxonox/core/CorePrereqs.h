/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  CorePrereq.h
 @brief Contains all the necessary forward declarations for all classes, structs and enums.
 */

#ifndef _CorePrereqs_H__
#define _CorePrereqs_H__

#include "orxonox/OrxonoxPlatform.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if (ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32) && !defined( CORE_STATIC_BUILD )
#  ifdef CORE_SHARED_BUILD
#    define _CoreExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _CoreExport
#    else
#      define _CoreExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _CoreExport  __attribute__ ((visibility("default")))
#else
#  define _CoreExport
#endif


//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------
class SignalHandler;

namespace orxonox
{
  class ArgReader;
  class BaseFactory;
  class BaseMetaObjectListElement;
  template <class T>
  class ClassFactory;
  template <class T>
  class ClassIdentifier;
  class ConfigValueContainer;
  class DebugLevel;
  class Error;
  class Factory;
  class Identifier;
  class IdentifierList;
  class IdentifierListElement;
  template <class T>
  class Iterator;
  class Language;
  class LanguageEntry;
  class Level;
  class Loader;
  class MetaObjectList;
  template <class T>
  class MetaObjectListElement;
  template <class T>
  class ObjectList;
  template <class T>
  class ObjectListElement;
  class OrxonoxClass;
  class OutputHandler;
  template <class T>
  class SubclassIdentifier;
}

#endif /* _CorePrereqs_H__ */
