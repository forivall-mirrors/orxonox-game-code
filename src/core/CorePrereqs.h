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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
  @file
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _CorePrereqs_H__
#define _CorePrereqs_H__

#include "OrxonoxPlatform.h"

#include <string>

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
#ifndef _XMLPort_Mode__
#define _XMLPort_Mode__
  namespace XMLPort
  {
    enum Mode
    {
      LoadObject,
      SaveObject
    };
  }
#endif

  typedef std::string LanguageEntryLabel;

  class BaseFactory;
  class BaseMetaObjectListElement;
  class BaseObject;
  template <class T>
  class ClassFactory;
  template <class T>
  class ClassIdentifier;
  template <class T>
  class ClassManager;
  class ClassTreeMask;
  class ClassTreeMaskIterator;
  class ClassTreeMaskNode;
  class CommandEvaluation;
  class CommandExecutor;
  class ConfigFile;
  class ConfigFileEntry;
  class ConfigFileEntryComment;
  class ConfigFileEntryValue;
  class ConfigFileManager;
  class ConfigFileSection;
  class ConfigValueContainer;
  class CoreSettings;
  class Error;
  class Executor;
  template <class T>
  class ExecutorMember;
  class ExecutorStatic;
  class Factory;
  class Functor;
  template <class T>
  class FunctorMember;
  class FunctorStatic;
  class Identifier;
  class IdentifierDistributor;
  class InputBuffer;
  class InputBufferListener;
  class InputHandlerGame;
  class InputHandlerGUI;
  class InputManager;
  template <class T>
  class Iterator;
  class Language;
  class LanguageEntry;
  class Level;
  class Loader;
  class MetaObjectList;
  template <class T>
  class MetaObjectListElement;
  class Namespace;
  class NamespaceNode;
  template <class T>
  class ObjectList;
  template <class T>
  class ObjectListElement;
  class OrxonoxClass;
  class OutputHandler;
  class Shell;
  template <class T>
  class SubclassIdentifier;
  class TclBind;
  class Tickable;
  template <class T, class O>
  class XMLPortClassObjectContainer;
  template <class T>
  class XMLPortClassParamContainer;
  class XMLPortObjectContainer;
  class XMLPortParamContainer;
}

#endif /* _CorePrereqs_H__ */
