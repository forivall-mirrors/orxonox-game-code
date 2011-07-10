
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

    registerStaticNetworkFunction(Script::executeHelper);

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
        this->remainingExecutions_ = Script::INF;
        this->mode_ = ScriptMode::normal;
        this->onLoad_ = true;
        this->times_ = Script::INF;
        this->needsGraphics_ = false;
    }

    /**
    @brief
        Destructor. Cleans up.
    */
    Script::~Script()
    {

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

        if(this->isOnLoad()) // If the object is onLoad the code is executed at once for the server.
            this->execute(0, true);
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
        PlayerInfo* player = NULL;

        // If the trigger is a PlayerTrigger.
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer())  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                return false;
            else
                player = pTrigger->getTriggeringPlayer();
        }
        else
            return false;

        if(player == NULL)  //TODO: Will this ever happen? If not, change in NotificationDispatcher as well.
        {
            COUT(4) << "The Script was triggered by an entity other than a Pawn. (" << trigger->getIdentifier()->getName() << ")" << std::endl;
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
    @param onLoad
        Whether this method is executed as a result of the onLoad parameter being set to true. Default is false.
    */
    void Script::execute(unsigned int clientId, bool onLoad)
    {
        // If this is the server or we're in standalone mode we check whether we still want to execute the code and if so decrease the number of remaining executions.
        if(GameMode::isMaster())
        {
            // If the number of executions have been used up.
            if(this->times_ != Script::INF && this->remainingExecutions_ <= 0)
                return;
        }

        // If this is either the standalone mode or we're on the client we want to be.
        if(GameMode::isStandalone() || Host::getPlayerID() == clientId)
        {
            this->executeHelper(this->getCode(), this->getMode(), this->getNeedsGraphics());
            if(GameMode::isMaster() && !onLoad && this->times_ != Script::INF) // Decrement the number of remaining executions.
                this->remainingExecutions_--;
        }

        // If this is the server and we're not on the client we want to be.
        if(GameMode::isServer() && Host::getPlayerID() != clientId)
        {
            // If we want to execute the code for all clients and the server.
            if(this->isForAll())
            {
                const std::map<unsigned int, PlayerInfo*> clients = PlayerManager::getInstance().getClients();
                for(std::map<unsigned int, PlayerInfo*>::const_iterator it = clients.begin(); it != clients.end(); it++)
                {
                    callStaticNetworkFunction(Script::executeHelper, it->first, this->getCode(), this->getMode(), this->getNeedsGraphics());
                    if(this->times_ != Script::INF) // Decrement the number of remaining executions.
                    {
                        this->remainingExecutions_--;
                        if(this->remainingExecutions_ <= 0)
                            break;
                    }
                }
            }
            // Else we execute the code just for the specified client.
            else
            {
                callStaticNetworkFunction(Script::executeHelper, clientId, this->getCode(), this->getMode(), this->getNeedsGraphics());
                if(this->times_ != Script::INF) // Decrement the number of remaining executions.
                    this->remainingExecutions_--;
            }
        }
    }

    /**
    @brief
        Helper method that is used to reach this Script object on other clients.
    */
    /*static*/ void Script::executeHelper(const std::string& code, const std::string& mode, bool needsGraphics)
    {
        // If the code needs graphics to be executed but the GameMode doesn't show graphics the code isn't executed.
        if(needsGraphics && !GameMode::showsGraphics())
            return;

        if(mode == Script::NORMAL) // If the mode is 'normal'.
            CommandExecutor::execute(code);
        else if(mode == Script::LUA) // If it's 'lua'.
        {
            LuaState* luaState = new LuaState();
            luaState->doString(code);
            delete luaState;
        }
    }

    /**
    @brief
        Callback that is called when a new client has connected.
    @param clientId
        The clientId of the new client that has connected.
    */
    void Script::clientConnected(unsigned int clientId)
    {
        // If this is the server and the Script is specified as being 'onLoad'.
        if(GameMode::isServer() && this->isOnLoad())
        {
            callStaticNetworkFunction(Script::executeHelper, clientId, this->getCode(), this->getMode(), this->getNeedsGraphics());
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
        Sets the mode to the mode specified in this->modeStr_.
        This is used internally for networking purposes.
    */
    void Script::modeChanged(void)
    {
        this->setMode(this->modeStr_);
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
