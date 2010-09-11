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

#ifndef _Script_H__
#define _Script_H__

#include "objects/ObjectsPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{

    namespace ScriptMode
    {
        //! Modes of the Script class.
        enum Value
        {
            normal,
            lua
        };
    }

    /**
    @brief
        The Script class lets you execute a piece of code, either the normal way or in lua, through XML. It can be specified whether the code is executed upon loading (creation) of the object. Additionally the code is executed each time a trigger event comes in.
        There are three parameters:
        'code': The code that should be executed.
        'mode': The mode, specifying whether the set code should be executed the normal way ('normal') or in lua ('lua'). Default is 'normal'.
        'onLoad': Whether the code is executed upon loading (creation) of this object. Default is true.

        Here are two examples illustrating the usage:
        @code
        <Script code="showGUI QuestGUI" />
        @endcode
        This would show the QuestGUI opon creation of the object. The mode is 'normal', not specified here since that is the default, also onLoad is true, also not specified, since it is the default as well.

        @code
        <Script code="hideGUI QuestGUI" mode="normal" onLoad="false">
            <events>
                <trigger>
                    <DistanceTrigger distance=10 target="Pawn" />
                </trigger>
            </events>
        </Script>
        @endcode
        This would hide the QuestGUI as soon as a Pawn got in range of the DistanceTrigger. The mode is 'normal', it is specified here, but could be ommitted as well, since it is the default. OnLoad is false, that is why it can't be ommitted.
    @author
        Benjamin Knecht
        Damian 'Mozork' Frick
    */
    class _ObjectsExport Script : public BaseObject
    {
        public:
            Script(BaseObject* creator);
            virtual ~Script();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Script object through XML.
            virtual void XMLEventPort(Element& xmlElement, XMLPort::Mode mode); //!< Creates a port that can be used to channel events and react to them.

            void trigger(bool triggered); //!< Is called when an event comes in trough the event port.
            void execute(); //!< Executes the Scripts code, depending on the mode.

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

        private:
            //! Static variables to avoid magic strings.
            static const std::string NORMAL;
            static const std::string LUA;
            static const int INF = -1;

            std::string code_; //!< The code that is executed by this Script.
            ScriptMode::Value mode_; //!< The mode the Script is in. Determines whether the code is executed the normal way or in lua.
            bool onLoad_; //!< Whether the Scripts code is executed upon loading (creation) of this Script.
            int times_; //!< The number of times the Scripts code is executed at the most. -1 denotes infinity.

            LuaState* luaState_; //!< The LuaState to execute the code in lua.
            int remainingExecutions_; //!< The number of remainign executions. -1 denotes infinity.

            /**
            @brief Sets the mode of the Script.
            @param mode The mode of the Script.
            */
            inline void setMode(ScriptMode::Value mode)
                { this->mode_ = mode; }
    };
}

#endif /* _Script_H__ */
