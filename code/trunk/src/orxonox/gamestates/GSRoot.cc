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
#include "core/ConsoleCommand.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/NetworkFunction.h"
#include "tools/Timer.h"
#include "tools/interfaces/TimeFactorListener.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    DeclareGameState(GSRoot, "root", false, false);
    SetConsoleCommandShortcut(GSRoot, printObjects);

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

        // time factor console command
        ConsoleCommand* command = createConsoleCommand(createFunctor(&GSRoot::setTimeFactor, this), "setTimeFactor");
        CommandExecutor::addConsoleCommandShortcut(command).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0);

        // time factor console command
        command = createConsoleCommand(createFunctor(&GSRoot::pause, this), "pause");
        CommandExecutor::addConsoleCommandShortcut(command).accessLevel(AccessLevel::Offline);
    }

    void GSRoot::deactivate()
    {
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

    float GSRoot::getTimeFactor()
    {
        return TimeFactorListener::getTimeFactor();
    }
}
