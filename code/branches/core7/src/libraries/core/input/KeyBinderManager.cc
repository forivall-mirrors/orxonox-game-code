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

#include "KeyBinderManager.h"

#include "util/Output.h"
#include "util/Exception.h"
#include "core/singleton/ScopedSingletonManager.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/LuaState.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "InputManager.h"
#include "KeyDetector.h"

namespace orxonox
{
    ManageScopedSingleton(KeyBinderManager, ScopeID::Graphics, false);

    static const std::string __CC_keybind_name = "keybind";
    static const std::string __CC_tkeybind_name = "tkeybind";
    static const std::string __CC_unbind_name = "unbind";
    static const std::string __CC_tunbind_name = "tunbind";

    SetConsoleCommand(__CC_keybind_name,  &KeyBinderManager::keybind).defaultValues("").argumentCompleter(0, autocompletion::command());
    SetConsoleCommand(__CC_tkeybind_name, &KeyBinderManager::tkeybind).defaultValues("").argumentCompleter(0, autocompletion::command());
    SetConsoleCommand(__CC_unbind_name,   &KeyBinderManager::unbind).defaultValues("");
    SetConsoleCommand(__CC_tunbind_name,  &KeyBinderManager::tunbind).defaultValues("");

    RegisterAbstractClass(KeyBinderManager).inheritsFrom<Configurable>();

    KeyBinderManager::KeyBinderManager()
        : currentBinder_(NULL)
        , bDefaultFileLoaded_(true)
        , bBinding_(false)
    {
        RegisterObject(KeyBinderManager);
        this->setConfigValues();

        // keybind console commands
        ModifyConsoleCommand(__CC_keybind_name ).setObject(this);
        ModifyConsoleCommand(__CC_tkeybind_name).setObject(this);
        ModifyConsoleCommand(__CC_unbind_name  ).setObject(this);
        ModifyConsoleCommand(__CC_tunbind_name ).setObject(this);

        // Load default key binder
        this->setCurrent(defaultFilename_);
    }

    KeyBinderManager::~KeyBinderManager()
    {
        // Delete all remaining KeyBinders
        for (std::map<std::string, KeyBinder*>::const_iterator it = this->binders_.begin(); it != this->binders_.end(); ++it)
            delete it->second;

        // Reset console commands
        ModifyConsoleCommand(__CC_keybind_name ).setObject(0);
        ModifyConsoleCommand(__CC_tkeybind_name).setObject(0);
        ModifyConsoleCommand(__CC_unbind_name  ).setObject(0);
        ModifyConsoleCommand(__CC_tunbind_name ).setObject(0);
    }

    void KeyBinderManager::setConfigValues()
    {
        SetConfigValue(defaultFilename_, "keybindings.ini")
            .description("Filename for the default keybindings file.")
            .callback(this, &KeyBinderManager::defaultFilenameChanged);
    }

    void KeyBinderManager::defaultFilenameChanged()
    {
        if (this->bDefaultFileLoaded_)
            this->setCurrent(this->defaultFilename_);
    }

    void KeyBinderManager::setCurrent(const std::string& filename)
    {
        this->currentBinder_ = this->get(filename);
        if (filename == this->defaultFilename_)
            this->bDefaultFileLoaded_ = true;
        else
            this->bDefaultFileLoaded_ = false;
    }

    inline void KeyBinderManager::unbind(const std::string& binding)
    {
        this->currentBinder_->setBinding("", binding, false);
    }

    inline void KeyBinderManager::tunbind(const std::string& binding)
    {
        this->currentBinder_->setBinding("", binding, true);
    }

    void KeyBinderManager::load(const std::string& filename)
    {
        std::map<std::string, KeyBinder*>::const_iterator it = this->binders_.find(filename);
        if (it != this->binders_.end())
            return;

        KeyBinder* binder = new KeyBinder(filename);
        this->binders_[filename] = binder;
    }

    void KeyBinderManager::unload(const std::string& filename)
    {
        if (filename == this->defaultFilename_)
            ThrowException(General, "KeyBinderManager: Cannot unload the default file");
        if (filename == this->currentBinder_->getBindingsFilename())
        {
            // unloading current file --> set default file
            this->setCurrent(this->defaultFilename_);
        }
        std::map<std::string, KeyBinder*>::iterator it = this->binders_.find(filename);
        if (it != this->binders_.end())
        {
            delete it->second;
            this->binders_.erase(it);
        }
    }

    KeyBinder* KeyBinderManager::get(const std::string& name)
    {
        this->load(name);
        return this->binders_[name];
    }

    InputHandler* KeyBinderManager::getCurrentAsHandler()
    {
        return this->getCurrent();
    }

    InputHandler* KeyBinderManager::getDefaultAsHandler()
    {
        return this->getDefault();
    }

    InputHandler* KeyBinderManager::getAsHandler(const std::string& name)
    {
        return this->get(name);
    }

    void KeyBinderManager::keybindInternal(const std::string& command, bool bTemporary)
    {
        if (!this->bBinding_)
        {
            orxout(message) << "Press any button/key or move a mouse/joystick axis" << endl;
            KeyDetector::getInstance().setCallback(createFunctor(&KeyBinderManager::keybindKeyPressed, this));
            InputManager::getInstance().enterState("detector");
            this->command_ = command;
            this->bTemporary_ = bTemporary;
            this->bBinding_ = true;
        }
        // else: We're still in a keybind command. Ignore this call.
    }

    // Gets called by the KeyDetector (registered with a Functor)
    void KeyBinderManager::keybindKeyPressed(const std::string& keyName)
    {
        if (this->bBinding_)
        {
            if (keyName == "Keys.KeyEscape")
            {
                orxout(message) << "Keybinding aborted." << endl;
            }
            else
            {
                orxout(message) << "Binding string \"" << command_ << "\" on key '" << keyName << "'" << endl;
                this->currentBinder_->setBinding(command_, keyName, bTemporary_);
            }
            InputManager::getInstance().leaveState("detector");
            // inform whatever was calling the command
            if (this->callbackFunction_)
                (*this->callbackFunction_)();
            this->bBinding_ = false;
        }
        // else: A key was probably pressed within the same tick, ignore it.
    }

    void KeyBinderManager::registerKeybindCallback(LuaFunctor* function)
    {
        this->callbackFunction_ = function;
    }
}
