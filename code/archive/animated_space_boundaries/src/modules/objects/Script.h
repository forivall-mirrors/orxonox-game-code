
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

/**
    @file Script.h
    @brief Definition of the Script class.
    @ingroup Objects
*/

#ifndef _Script_H__
#define _Script_H__

#include "objects/ObjectsPrereqs.h"

#include <string>
#include <vector>

#include "core/BaseObject.h"
#include "network/ClientConnectionListener.h"

namespace orxonox
{

    /**
    @brief The mode a specific @ref orxonox::Script "Script" is in.
    */
    namespace ScriptMode
    {
        enum Value
        {
            normal, //!< The @ref orxonox::Script "Scripts'" code is executed through the @ref orxonox::CommandExecutor "CommandExecutor".
            lua //!< The @ref orxonox::Script "Scripts'" code is executed through lua.
        };
    }

    /**
    @brief
        The Script class lets you execute a piece of code, either the normal way or in lua, through XML. It can be specified whether the code is executed upon loading (creation) of the object. Additionally the code is executed each time a trigger event comes in.
        There are three parameters:
        - @b code The code that should be executed.
        - @b mode The mode, specifying whether the set code should be executed the normal way (<em>normal</em>) or in lua (<em>lua</em>). Default is <em>normal</em>.
        - @b onLoad Whether the code is executed upon loading (creation) of this object. If this is set the code is executed ofr all players, regardless of the value of parameter <em>forAll</em>. Default is true.
        - @b needsGraphics Whether the code needs graphics to be executed or not. Default is false.
        - @b forAll Whether the code is executed for all players each time the Script is triggered or jut for the player triggering the Script. If forAll is false, which is default, the event that triggers the Script must come from a @ref orxonox::PlayerTrigger "PlayerTrigger".

        Here are two examples illustrating the usage:
        @code
        <Script code="showGUI QuestGUI" needsGraphics=true />
        @endcode
        This would show the QuestGUI opon creation of the object. The <em>mode</em> is <em>normal</em>, not specified here since that is the default, also <em>onLoad</em> is true, also not specified, since it is the default as well. Also <em>needsGraphics</em> is set to true because showGUI needs graphics to work.

        @code
        <Script code="hideGUI QuestGUI" mode="normal" onLoad="false" needsGraphics=true >
            <events>
                <trigger>
                    <DistanceTrigger distance=10 target="Pawn" />
                </trigger>
            </events>
        </Script>
        @endcode
        This would hide the QuestGUI as soon as a @ref orxonox::Pawn "Pawn" got in range of the @ref orxonox::DistanceTrigger "DistanceTrigger". The mode is <em>normal</em>, it is specified here, but could be ommitted as well, since it is the default. <em>OnLoad</em> is false, that is why it can't be ommitted. Also <em>needsGraphics</em> is set to true because showGUI needs graphics to work.

    @author
        Benjamin Knecht
    @author
        Damian 'Mozork' Frick
    */
    class _ObjectsExport Script : public BaseObject, public ClientConnectionListener
    {
        public:
            Script(BaseObject* creator);
            virtual ~Script();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Script object through XML.
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode); //!< Creates a port that can be used to channel events and react to them.

            bool trigger(bool triggered, BaseObject* trigger); //!< Is called when an event comes in trough the event port.
            void execute(unsigned int clientId, bool onLoad = false); //!< Executes the Scripts code for the input client, depending on the mode.
            static void executeHelper(const std::string& code, const std::string& mode, bool needsGraphics); //!< Helper method that is used to reach this Script object on other clients.

            /**
            @brief Sets the code that is executed by this Script.
            @param code  The code that is executed by this Script.
            */
            inline void setCode(const std::string& code)
                { code_ = code; }
            /**
            @brief Get the code that is executed by this Script.
            @return Returns the code that is executed by this Script.
            */
            inline const std::string& getCode() const
                { return code_; }

            void setMode(const std::string& mode); //!< Sets the mode of the Script.
            const std::string& getMode(void); //!< Get the mode of the Script.

            /**
            @brief Set whether this Script is executed onLoad or not.
            @param onLoad if true the Script is executed onLoad, if false it's not.
            */
            inline void setOnLoad(bool onLoad)
                { this->onLoad_ = onLoad; }
            /**
            @brief Get whether this Script is executed onLoad.
            @return Returns true if this Script is executed onLoad, false if not.
            */
            inline bool isOnLoad(void)
                { return this->onLoad_; }

            void setTimes(int times); //!< Set the number of times this Script is executed at the most.
            /**
            @brief Get the number of times this Script is executed at the most.
            @return Returns the number of times this Script is executed at the most. -1 denotes infinity.
            */
            inline int getTimes(void)
                { return this->times_; }

            /**
            @brief Set whether the code to be executed needs graphics to work.
            @param needsGraphics True if the cde needs graphics to be executed properly.
            */
            void setNeedsGraphics(bool needsGraphics)
                { this->needsGraphics_ = needsGraphics; }
            /**
            @brief Get whether the code to be executed needs graphics to work.
            @return Returns true if the code needs graphic, false if not.
            */
            bool getNeedsGraphics(void)
                { return this->needsGraphics_; }

            /**
            @brief Set whether the code is executed for all players or just for the player triggering the Script.
            @param forAll If true the code is executed for all players.
            */
            void setForAll(bool forAll)
                { this->forAll_ = forAll; }
            /**
            @brief Get whether the Script executes its code for all players or just for the player triggering the Script.
            @return Returns true if the code is executed for all players, false if not.
            */
            bool isForAll(void)
                { return this->forAll_; }

            virtual void clientConnected(unsigned int clientId); //!< Callback that is called when a new client has connected.
            virtual void clientDisconnected(unsigned int clientid) {}

        private:
            //! Static variables to avoid magic strings.
            static const std::string NORMAL;
            static const std::string LUA;
            static const int INF;

            std::string code_; //!< The code that is executed by this Script.
            ScriptMode::Value mode_; //!< The mode the Script is in. Determines whether the code is executed the normal way or in lua.
            std::string modeStr_; //!< The mode the Script is in, as a string. Is used for networking purposes.
            bool onLoad_; //!< Whether the Scripts code is executed upon loading (creation) of this Script.
            int times_; //!< The number of times the Scripts code is executed at the most. -1 denotes infinity.
            bool needsGraphics_; //!< Whether the code to be executed needs graphics.
            bool forAll_; //!< Whether the code is executed for all players (in a multiplayer setup) or just for the one triggering the Script.

            int remainingExecutions_; //!< The number of remainign executions. -1 denotes infinity.

            void modeChanged(); //!< Sets the mode to the mode specified in this->modeStr_.

            /**
            @brief Sets the mode of the Script.
            @param mode The mode of the Script.
            */
            inline void setMode(ScriptMode::Value mode)
                { this->mode_ = mode; }
    };
}

#endif /* _Script_H__ */
