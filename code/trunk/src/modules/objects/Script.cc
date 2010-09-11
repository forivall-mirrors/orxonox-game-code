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
 *      Benjamin Knecht
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

#include "Script.h"

#include "core/command/CommandExecutor.h"
#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/LuaState.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(Script);

    // Initializing constants.
    /*static*/ const std::string Script::NORMAL = "normal";
    /*static*/ const std::string Script::LUA = "lua";
    /*static*/ const int Script::INF = -1;

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param creator
        The creator of this object.
    */
    Script::Script(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Script);

        // Initialize variables.
        this->luaState_ = NULL;
        this->remainingExecutions_ = Script::INF;

    }

    /**
    @brief
        Destructor. Cleans up.
    */
    Script::~Script()
    {
        if(this->isInitialized() && this->luaState_ != NULL)
            delete this->luaState_;
    }

    /**
    @brief
        Method for creating a Script object through XML.
    @param xmlelement
        The element.
    @param mode
        The mode.
    */
    void Script::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Script, XMLPort, xmlelement, mode);

        XMLPortParam(Script, "code", setCode, getCode, xmlelement, mode);
        XMLPortParamTemplate(Script, "mode", setMode, getMode, xmlelement, mode, const std::string&).defaultValues(Script::NORMAL);
        XMLPortParam(Script, "onLoad", setOnLoad, isOnLoad, xmlelement, mode).defaultValues(true);
        XMLPortParam(Script, "times", setTimes, getTimes, xmlelement, mode).defaultValues(Script::INF);

        XMLPortEventSink(Script, BaseObject, "trigger", trigger, xmlelement, mode);

        if(this->isOnLoad()) // If the object is onLoad the code is executed at once.
            this->execute();
    }

    /**
    @brief
        Creates a port that can be used to channel events and react to them.
    @param xmlelement
        The element.
    @param mode
        The mode.
    */
    void Script::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Script, XMLEventPort, xmlelement, mode);

        XMLPortEventState(Script, BaseObject, "trigger", trigger, xmlelement, mode);
    }

    /**
    @brief
        Is called when an event comes in trough the event port.
    @param triggered
        Whether the event is triggering or un-triggering.
    */
    void Script::trigger(bool triggered)
    {
        if(triggered) // If the event is triggering (instead of un-triggering) the code of this Script  is executed.
            this->execute();
    }

    /**
    @brief
        Executes the Scripts code, depending on the mode.
    */
    void Script::execute()
    {
        if(this->times_ != Script::INF && this->remainingExecutions_ == 0)
            return;

        if(this->mode_ == ScriptMode::normal) // If the mode is 'normal'.
            CommandExecutor::execute(this->code_);
        else if(this->mode_ == ScriptMode::lua) // If it's 'lua'.
        {
            assert(this->luaState_);
            this->luaState_->doString(this->code_);
        }

        if(this->times_ != Script::INF)
            this->remainingExecutions_--;
    }

    /**
    @brief
        Sets the mode of the Script.
    @param mode
        The mode as a string.
    */
    void Script::setMode(const std::string& mode)
    {
        if(mode == Script::NORMAL)
            this->setMode(ScriptMode::normal);
        else if(mode == Script::LUA)
        {
            this->setMode(ScriptMode::lua);
            // Creates a new LuaState.
            if(this->luaState_ == NULL)
                this->luaState_ = new LuaState();
        }
        else
        {
            COUT(2) << "Invalid mode '" << mode << "' in Script object." << std::endl;
            this->setMode(ScriptMode::normal);
        }
    }

    /**
    @brief
        Get the mode of the Script.
    @return
        Returns the mode as a string.
    */
    const std::string& Script::getMode(void)
    {
        switch(this->mode_)
        {
            case ScriptMode::normal:
                return Script::NORMAL;
            case ScriptMode::lua:
                return Script::LUA;
            default: // This will never happen...
                return Script::NORMAL;
        }
    }

    /**
    @brief
        Set the number of times this Script is executed at the most.
        -1 denotes infinity.
    @param times
        The number of times to be set.
    */
    void Script::setTimes(int times)
    {
        if(times >= -1)
        {
            this->times_ = times;
            this->remainingExecutions_ = times;
        }
        else
        {
            COUT(2) << "Invalid times '" << times << "' in Script." << std::endl;
            this->times_ = Script::INF;
        }
    }

}
