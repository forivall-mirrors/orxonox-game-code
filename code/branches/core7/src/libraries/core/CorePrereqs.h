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
@brief
    Shared library macros, enums, constants and forward declarations for the core library
*/

#ifndef _CorePrereqs_H__
#define _CorePrereqs_H__

#include "OrxonoxConfig.h"
#include <boost/version.hpp>

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined( CORE_STATIC_BUILD )
#  ifdef CORE_SHARED_BUILD
#    define _CoreExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _CoreExport
#    else
#      define _CoreExport __declspec(dllimport)
#    endif
#  endif
#  define _CorePrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _CoreExport  __attribute__ ((visibility("default")))
#  define _CorePrivate __attribute__ ((visibility("hidden")))
#else
#  define _CoreExport
#  define _CorePrivate
#endif

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

namespace orxonox
{
    static const uint32_t OBJECTID_UNKNOWN = static_cast<uint32_t>(-1);
}

//-----------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------

namespace orxonox
{
    namespace XMLPort
    {
        enum Mode
        {
            NOP,
            LoadObject,
            SaveObject,
            ExpandObject
        };
    }

    namespace ConfigFileType
    {
        enum Value
        {
            Settings,
            JoyStickCalibration,
            CommandHistory
            // Don't forget to adjust the array size in the ConfigFileManager when adding a new entry here!
        };
    }

    namespace KeybindMode
    {
        enum Value
        {
            OnPress,
            OnHold,
            OnRelease,
            OnPressAndRelease,
            None
        };
    };
}

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    typedef std::string LanguageEntryLabel;

    template <class T, class U>
    T orxonox_cast(U*);

    class BaseObject;
    template <class T>
    class ClassFactory;
    template <class T>
    class ClassFactoryWithContext;
    template <class T>
    class ClassIdentifier;
    class ClassTreeMask;
    class ClassTreeMaskIterator;
    class ClassTreeMaskNode;
    class ClassTreeMaskObjectIterator;
    class CommandLineParser;
    class CommandLineArgument;
    class ConfigFile;
    class ConfigFileEntry;
    class ConfigFileEntryComment;
    class ConfigFileEntryValue;
    class ConfigFileManager;
    class ConfigFileSection;
    class Configurable;
    class ConfigValueContainer;
    class Context;
    class Core;
    class Destroyable;
    class DestructionListener;
    class DynLib;
    class DynLibManager;
    struct Event;
    class EventState;
    class Factory;
    class Game;
    class GameState;
    struct GameStateInfo;
    struct GameStateTreeNode;
    class GraphicsManager;
    class GUIManager;
    class Identifiable;
    class Identifier;
    template <class T>
    class Iterator;
    class Language;
    class Listable;
    class Loader;
    class LuaFunctor;
    class LuaState;
    class MemoryArchive;
    class MemoryArchiveFactory;
    class ModuleInstance;
    class Namespace;
    class NamespaceNode;
    template <class T>
    class ObjectList;
    class ObjectListBase;
    class ObjectListBaseElement;
    template <class T>
    class ObjectListElement;
    template <class T>
    class ObjectListIterator;
    class OgreWindowEventListener;
    class OrxonoxClass;
    class OrxonoxInterface;
    class PathConfig;
    struct ResourceInfo;
    class SettingsConfigFile;
    template <class T>
    class SmartPtr;
    class StaticallyInitializedInstance;
    template <class T>
    class SubclassIdentifier;
    class Template;
    class Thread;
    class ThreadPool;
    class ViewportEventListener;
    template <class T>
    class WeakPtr;
    class WindowEventListener;
    class XMLFile;
    class XMLNameListener;
    template <class T, class O>
    class XMLPortClassObjectContainer;
    template <class T>
    class XMLPortClassParamContainer;
    class XMLPortObjectContainer;
    class XMLPortParamContainer;

    // Command
    class ArgumentCompleter;
    class ArgumentCompletionListElement;
    class CommandEvaluation;
    class ConsoleCommand;
    class ConsoleCommandManager;
    class Executor;
    template <class T>
    class ExecutorMember;
    class ExecutorStatic;
    class Functor;
    template <class O>
    class FunctorMember;
    typedef FunctorMember<void> FunctorStatic;
    template <class F, class O>
    class FunctorPointer;
    class IOConsole;
    class IRC;
    class Shell;
    class ShellListener;
    class TclBind;
    struct TclInterpreterBundle;
    template <class T>
    class TclThreadList;
    class TclThreadManager;

    // Input
    class BaseCommand;
    class BufferedParamCommand;
    class Button;
    class HalfAxis;
    class InputBuffer;
    class InputDevice;
    template <class Traits>
    class InputDeviceTemplated;
    class InputHandler;
    class InputManager;
    class InputState;
    struct InputStatePriority;
    class JoyStickQuantityListener;
    class JoyStick;
    class KeyBinder;
    class KeyBinderManager;
    class Keyboard;
    class KeyDetector;
    class KeyEvent;
    class Mouse;
    class ParamCommand;
    class SimpleCommand;
}

#include "command/FunctorPtr.h"
#include "command/ExecutorPtr.h"

// CppTcl
namespace Tcl
{
    class interpreter;
    class object;
}

// Boost
namespace boost
{
#if (BOOST_VERSION < 104400)

    namespace filesystem
    {
        struct path_traits;
        template <class String, class Traits> class basic_path;
        typedef basic_path<std::string, path_traits> path;
    }

#elif (BOOST_VERSION < 105000)

# if BOOST_FILESYSTEM_VERSION == 2
    namespace filesystem2
    {
        struct path_traits;
        template <class String, class Traits> class basic_path;
        typedef basic_path<std::string, path_traits> path;
    }
    namespace filesystem
    {
        using filesystem2::basic_path;
        using filesystem2::path_traits;
        using filesystem2::path;
    }
# elif BOOST_FILESYSTEM_VERSION == 3
    namespace filesystem3
    {
        class path;
    }
    namespace filesystem
    {
        using filesystem3::path;
    }
# endif

#else

    namespace filesystem
    {
        class path;
    }

#endif

    class thread;
    class mutex;
    class shared_mutex;
    class condition_variable;
}

// Ogre
namespace Ogre
{
    class DataStream;
    template <class T> class SharedPtr;
    typedef SharedPtr<DataStream> DataStreamPtr;
}
namespace orxonox
{
    // Import the Ogre::DataStream
    using Ogre::DataStream;
    using Ogre::DataStreamPtr;
}

// CEGUI
namespace CEGUI
{
    class DefaultLogger;
    class Logger;
    class LuaScriptModule;

#if CEGUI_VERSION_MAJOR < 1 && CEGUI_VERSION_MINOR < 7
    class OgreCEGUIRenderer;
    class OgreCEGUIResourceProvider;
    class OgreCEGUITexture;
#else
    class OgreRenderer;
    class OgreResourceProvider;
    class OgreImageCodec;
#endif
}

// Lua
struct lua_State;

// TinyXML and TinyXML++
class TiXmlString;
class TiXmlOutStream;
class TiXmlNode;
class TiXmlHandle;
class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;
namespace ticpp
{
    class Document;
    class Element;
    class Declaration;
    class StylesheetReference;
    class Text;
    class Comment;
    class Attribute;
}
namespace orxonox
{
    using ticpp::Element;
}

#endif /* _CorePrereqs_H__ */
