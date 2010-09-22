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
#include "core/GameMode.h"
#include "core/LuaState.h"
#include "core/XMLPort.h"
#include "network/Host.h"
#include "network/NetworkFunction.h"

#include "PlayerManager.h"
#include "infos/PlayerInfo.h"
#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(Script);

    registerMemberNetworkFunction(Script, execute);

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
    Script::Script(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(Script);

        // Initialize variables.
        this->luaState_ = NULL;
        this->remainingExecutions_ = Script::INF;
        this->mode_ = ScriptMode::normal;
        this->onLoad_ = true;
        this->times_ = Script::INF;
        this->needsGraphics_ = false;

        this->counter_ = 0.0f;

        this->registerVariables();
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
        XMLPortParam(Script, "needsGraphics", setNeedsGraphics, getNeedsGraphics, xmlelement, mode).defaultValues(false);
        XMLPortParam(Script, "forAll", setForAll, isForAll, xmlelement, mode).defaultValues(false);

        XMLPortEventSink(Script, BaseObject, "trigger", trigger, xmlelement, mode);

        if(this->isOnLoad()) // If the object is onLoad the code is executed at once for all clients.
            this->execute(0);
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
        Register the variables that need to be synchronized.
    */
    void Script::registerVariables(void)
    {
        registerVariable(code_, VariableDirection::ToClient);
        registerVariable(needsGraphics_, VariableDirection::ToClient);
        registerVariable(modeStr_, VariableDirection::ToClient, new NetworkCallback<Script>(this, &Script::modeChanged));
    }

    void Script::modeChanged(void)
    {
        this->setMode(this->modeStr_);
    }

    void Script::tick(float dt)
    {
        SUPER(Script, tick, dt);

        if(!this->clientCallbacks_.empty())
        {
            if(this->counter_ < 2.0f)
            {
                this->counter_ += dt;
            }
            else
            {
                for(std::vector<unsigned int>::iterator it = this->clientCallbacks_.begin(); it != this->clientCallbacks_.end(); it++)
                {
                    this->execute(*it, true);
                }
                this->clientCallbacks_.clear();
                this->counter_ = 0.0f;
            }
        }
    }

    /**
    @brief
        Is called when an event comes in trough the event port.
    @param triggered
        Whether the event is triggering or un-triggering.
    @param trigger
        The object that caused the event to be fired.
    @return
        Returns true if successful.
    */
    bool Script::trigger(bool triggered, BaseObject* trigger)
    {
        if(!triggered || !this->isActive()) // If the Script is inactive it cannot be executed.
            return false;

        COUT(4) << "Script (&" << this << ") triggered." << std::endl;

        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        Pawn* pawn = NULL;

        // If the trigger is a PlayerTrigger.
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer())  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                return false;
            else
                pawn = pTrigger->getTriggeringPlayer();
        }
        else
            return false;

        if(pawn == NULL)  //TODO: Will this ever happen? If not, change in NotificationDispatcher as well.
        {
            COUT(4) << "The Script was triggered by an entity other than a Pawn. (" << trigger->getIdentifier()->getName() << ")" << std::endl;
            return false;
        }

        // Extract the PlayerInfo from the Pawn.
        PlayerInfo* player = pawn->getPlayer();

        if(player == NULL)
        {
            COUT(3) << "The PlayerInfo* is NULL." << std::endl;
            return false;
        }

        this->execute(player->getClientID());
        return true;
    }

    /**
    @brief
        Executes the Scripts code for the input client, depending on the mode.
    @param clientId
        The Id of the client the Script should be executed for.
    @param fromCallback
        Whether this method is executed in response to the connectedCallback().
    */
    void Script::execute(unsigned int clientId, bool fromCallback)
    {
        if(GameMode::isServer())
        {
            // If the number of executions have been used up.
            if(this->times_ != Script::INF && this->remainingExecutions_ == 0)
                return;

            // Decrement the number of remaining executions.
            if(this->times_ != Script::INF)
                this->remainingExecutions_--;
        }

        if(GameMode::isStandalone() || Host::getPlayerID() == clientId)
        {
            // If the code needs graphics to be executed but the GameMode doesn't show graphics the code isn't executed.
            if(this->needsGraphics_ && !GameMode::showsGraphics())
                return;

            if(this->mode_ == ScriptMode::normal) // If the mode is 'normal'.
                CommandExecutor::execute(this->code_);
            else if(this->mode_ == ScriptMode::lua) // If it's 'lua'.
            {
                if(this->luaState_ == NULL)
                    this->luaState_ = new LuaState();
                this->luaState_->doString(this->code_);
            }
        }
        if(!GameMode::isStandalone() && GameMode::isServer() && Host::getPlayerID() != clientId)
        {
            if(!fromCallback && this->isForAll())
            {
                const std::map<unsigned int, PlayerInfo*> clients = PlayerManager::getInstance().getClients();
                for(std::map<unsigned int, PlayerInfo*>::const_iterator it = clients.begin(); it != clients.end(); it++)
                {
                    callMemberNetworkFunction(Script, execute, this->getObjectID(), it->first, it->first, false);
                }
            }
            else
            {
                callMemberNetworkFunction(Script, execute, this->getObjectID(), clientId, clientId, false);
            }
        }
    }

    void Script::clientConnected(unsigned int clientId)
    {
        if(!GameMode::isStandalone() && GameMode::isServer() && this->isOnLoad())
        {
            if(clientId != 0)
                //TODO: Do better. This is only a temporary fix.
                this->clientCallbacks_.push_back(clientId);
        }
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
        {
            this->setMode(ScriptMode::normal);
            this->modeStr_ = Script::NORMAL;
        }
        else if(mode == Script::LUA)
        {
            this->setMode(ScriptMode::lua);
            this->modeStr_ = Script::LUA;
            // Creates a new LuaState.
            if(this->luaState_ == NULL)
                this->luaState_ = new LuaState();
        }
        else
        {
            COUT(2) << "Invalid mode '" << mode << "' in Script object. Setting to 'normal'." << std::endl;
            this->setMode(ScriptMode::normal);
            this->modeStr_ = Script::NORMAL;
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
            COUT(2) << "Invalid times '" << times << "' in Script. Setting to infinity." << std::endl;
            this->times_ = Script::INF;
            this->remainingExecutions_ = Script::INF;
        }
    }

}
