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

#ifndef _KeyBinderManager_H__
#define _KeyBinderManager_H__

#include "InputPrereqs.h"

#include <map>
#include <string>

#include "util/Singleton.h"
#include "core/class/OrxonoxClass.h"

namespace orxonox //tolua_export
{ //tolua_export
    /**
    @brief
        Handles the KeyBinders and supplies them throughout the game.

        This interface merely serves to provide a static "keybind" command that always
        maps to the currently active KeyBinder. You can set that with setCurrent().
        There is also a default one, retrieved with getDefault(). The idea is that
        mostly the default KeyBinder is active except for special situations (mini-game for inst).
    @remarks
        You are not forced to use the KeyBinder imposed by getCurrent(). But be aware that "keybind"
        will not work as expected!
    */
    class _CoreExport KeyBinderManager //tolua_export
        : public Singleton<KeyBinderManager>, public OrxonoxClass
    { //tolua_export
        friend class Singleton<KeyBinderManager>;
    public:
        KeyBinderManager();
        ~KeyBinderManager();
        void setConfigValues();

        static KeyBinderManager& getInstance() { return Singleton<KeyBinderManager>::getInstance(); } //tolua_export
        //! Returns the currently selected KeyBinder
        KeyBinder* getCurrent() { return this->currentBinder_; } //tolua_export
        //! Like getCurrent(), but returns it as InputHandler* (so you don't have to include KeyBinder.h)
        InputHandler* getCurrentAsHandler();
        //! Selects the current KeyBinder and creates it if not yet loaded.
        void setCurrent(const std::string& filename);

        //! Returns the default KeyBinder
        KeyBinder* getDefault()
            { return binders_[this->defaultFilename_]; }
        //! Returns the default KeyBinder as InputHandler* (so you don't have to include KeyBinder.h)
        InputHandler* getDefaultAsHandler();
        //! Returns the filename of the default key bindings
        const std::string& getDefaultFilename()
            { return defaultFilename_; }
        //! Selects the default KeyBinder as current one
        void setToDefault()
            { this->setCurrent(this->defaultFilename_); }

        //! Returns a pointer to a KeyBinder (creates it if not yet loaded)
        KeyBinder* get(const std::string& name);
        //! Like get() but return value is of type InputHandler* (so you don't have to include KeyBinder.h)
        InputHandler* getAsHandler(const std::string& name);

        //! Loads a KeyBinder by creating it (no different from get() except for the return value)
        void load(const std::string& filename);
        //! Destroys a KeyBinder completely (does nothing if not yet loaded)
        void unload(const std::string& filename);

        //! Bind 'command' to any key pressed after this call (use with care!)
        inline void keybind(const std::string& command) { this->keybindInternal(command, false); } //tolua_export
        //! Bind 'command' to any key pressed after this call (use with care!), but temporarily (no file save)
        inline void tkeybind(const std::string& command)
            { this->keybindInternal(command, true); }
        void unbind(const std::string& binding); //tolua_export
        void tunbind(const std::string& binding);
        void registerKeybindCallback(LuaFunctor* function); //tolua_export

    private:
        KeyBinderManager(const KeyBinderManager&);
        void keybindInternal(const std::string& command, bool bTemporary);
        void keybindKeyPressed(const std::string& keyName);
        void defaultFilenameChanged();

        // KeyBinder management
        KeyBinder* currentBinder_;                   //! Currently selected KeyBinder (never NULL!)
        std::map<std::string, KeyBinder*> binders_;  //! All loaded KeyBinders
        bool bDefaultFileLoaded_;                    //! Tells whether the default one is loaded
        std::string defaultFilename_;                //! Name of the file with the default key bindings

        // keybind command related
        SharedPtr<LuaFunctor> callbackFunction_;     //! Function to be called when key was pressed after "keybind" command
        bool bBinding_;                              //! Tells whether a key binding process is active
        bool bTemporary_;                            //! Stores tkeybind/keybind value
        std::string command_;                        //! Stores the command received by (t)keybind

        static KeyBinderManager* singletonPtr_s;
    }; //tolua_export
} //tolua_export

#endif /* _KeyBinderManager_H__ */
