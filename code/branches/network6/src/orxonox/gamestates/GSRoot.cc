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

#include "GSRoot.h"

#include "util/Clock.h"
#include "core/BaseObject.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/command/ConsoleCommand.h"
#include "network/NetworkFunction.h"
#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    DeclareGameState(GSRoot, "root", false, false);

    static const std::string __CC_setTimeFactor_name = "setTimeFactor";
    static const std::string __CC_pause_name = "pause";

    SetConsoleCommand("printObjects", &GSRoot::printObjects).hide();
    SetConsoleCommand(__CC_setTimeFactor_name, &GSRoot::setTimeFactor).accessLevel(AccessLevel::Master).defaultValues(1.0);
    SetConsoleCommand(__CC_pause_name,         &GSRoot::pause        ).accessLevel(AccessLevel::Master);

    registerStaticNetworkFunction(&TimeFactorListener::setTimeFactor);

    GSRoot::GSRoot(const GameStateInfo& info)
        : GameState(info)
        , bPaused_(false)
        , timeFactorPauseBackup_(1.0f)
    {
    }

    GSRoot::~GSRoot()
    {
        NetworkFunctionBase::destroyAllNetworkFunctions();
    }

    void GSRoot::printObjects()
    {
        unsigned int nr=0;
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it; ++it)
        {
            if (dynamic_cast<Synchronisable*>(*it))
                COUT(0) << "object: " << it->getIdentifier()->getName() << " id: " << dynamic_cast<Synchronisable*>(*it)->getObjectID() << std::endl;
            else
                COUT(0) << "object: " << it->getIdentifier()->getName() << std::endl;
            nr++;
        }
        COUT(0) << "currently got " << nr << " objects" << std::endl;
    }

    void GSRoot::activate()
    {
        // reset game speed to normal
        TimeFactorListener::setTimeFactor(1.0f);

        ModifyConsoleCommand(__CC_setTimeFactor_name).setObject(this);
        ModifyConsoleCommand(__CC_pause_name).setObject(this);
    }

    void GSRoot::deactivate()
    {
        ModifyConsoleCommand(__CC_setTimeFactor_name).setObject(0);
        ModifyConsoleCommand(__CC_pause_name).setObject(0);
    }

    void GSRoot::update(const Clock& time)
    {
        for (ObjectList<Timer>::iterator it = ObjectList<Timer>::begin(); it; )
        {
            Timer* object = *it;
            ++it;
            object->tick(time);
        }

        /*** HACK *** HACK ***/
        // Call the Tickable objects
        float leveldt = time.getDeltaTime();
        if (leveldt > 1.0f)
        {
            // just loaded
            leveldt = 0.0f;
        }
        float realdt = leveldt * TimeFactorListener::getTimeFactor();
        for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; )
        {
            Tickable* object = *it;
            ++it;
            object->tick(realdt);
        }
        /*** HACK *** HACK ***/
    }

    /**
    @brief
        Changes the speed of Orxonox
    @remark
        This function is a hack when placed here!
        Timefactor should be related to the scene (level or so), not the game
    */
    void GSRoot::setTimeFactor(float factor)
    {
        if (GameMode::isMaster())
        {
            if (!this->bPaused_)
            {
                TimeFactorListener::setTimeFactor(factor);
            }
            else
                this->timeFactorPauseBackup_ = factor;
        }
    }

    void GSRoot::pause()
    {
        if (GameMode::isMaster())
        {
            if (!this->bPaused_)
            {
                this->timeFactorPauseBackup_ = TimeFactorListener::getTimeFactor();
                this->setTimeFactor(0.0f);
                this->bPaused_ = true;
            }
            else
            {
                this->bPaused_ = false;
                this->setTimeFactor(this->timeFactorPauseBackup_);
            }
        }
    }

    void GSRoot::changedTimeFactor(float factor_new, float factor_old)
    {
        if (!GameMode::isStandalone())
            callStaticNetworkFunction(&TimeFactorListener::setTimeFactor, NETWORK_PEER_ID_BROADCAST, factor_new);
    }
}
