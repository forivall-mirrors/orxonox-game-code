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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file QuestEffectBeacon.h
    @brief Definition of the QuestEffectBeacon class.
    @ingroup Questsystem
*/

#ifndef _QuestEffectBeacon_H__
#define _QuestEffectBeacon_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    /**
    @brief
        The status of the @ref orxonox::QuestEffectBeacon "QuestEffectBeacon", can be either active or inactive.

    @ingroup Questsystem
    */
    namespace QuestEffectBeaconStatus
    {
        enum Value
        {
            Inactive, //!< The @ref orxonox::QuestEffectBeacon "QuestEffectBeacon" is inactive.
            Active //!< The @ref orxonox::QuestEffectBeacon "QuestEffectBeacon" is active.
        };
    }

    /**
    @brief
        A QuestEffectBeacon is a physical entity in the game which can (under some condition(s)) invoke a number of @ref orxonox::QuestEffect "QuestEffects" on players meeting the condition(s).
        The conditions under which the @ref orxonox::QuestEffect "QuestEffects" are invoked on the player are defined by @ref orxonox::Trigger "Triggers" (or really any kind of entity firing events, e.g. @ref orxonox::EventListener "EventListeners"). The trigger the event originates from, however has to be a @ref orxonox::PlayerTrigger PlayerTrigger.
        A QuestEffectBeacon can be executed a defined number of times.
        A QuestEffectBeacon can be inactive or active. While inactive it can't be executed.

        Creating a QuestEffectBeacon through XML goes as follows:
        @code
        <QuestEffectBeacon times=n> //Where 'n' is eighter a number >= 0, which means the QuestEffectBeacon can be executed n times. Or n = -1, which means the QuestEffectBeacon can be executed an infinite number of times.
            <effects>
                <QuestEffect /> //A list of QuestEffects, invoked when the QuestEffectBeacon is executed, see QuestEffect for the full XML representation.
                ...
                <QuestEffect />
            </effects>
            <events>
                <execute>
                    <EventListener event=eventIdString />
                </execute>
            </events>
            <attached>
                <PlayerTrigger name=eventIdString /> //A PlayerTrigger triggering the execution of the QuestEffectBeacon.
            </attached>
        </QuestEffectBeacon>
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup Questsystem
    */
    class _QuestsystemExport QuestEffectBeacon : public StaticEntity
    {
        public:
            QuestEffectBeacon(BaseObject* creator);
            virtual ~QuestEffectBeacon();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestEffectBeacon object through XML.
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            bool execute(bool bTriggered, BaseObject* trigger); //!< Executes the QuestEffects of the QuestEffectBeacon.

            /**
            @brief Tests whether the QuestEffectBeacon is active.
            @return Returns true if the QuestEffectBeacon is active, fals if not.
            */
            inline bool isActive(void)
            { return this->status_ == QuestEffectBeaconStatus::Active; }

            bool setActive(bool activate); //!< Set the status of the QuestEffectBeacon.

        protected:
            bool decrementTimes(void); //!< Decrement the number of times the QuestEffectBeacon can still be executed.

            /**
            @brief Returns the number of times the QUestEffectBeacon can still be executed.
            @return Returns the number of times the QUestEffectBeacon can still be executed.
            */
            inline const int & getTimes(void) const
                { return this->times_; }

        private:
            static const int INFINITE_TIME = -1; //!< Constant to avoid using magic numbers.

            std::list<QuestEffect*> effects_; //!< The list of QuestEffects to be invoked on the executing player.
            int times_; //!< Number of times the beacon can be exectued.
            QuestEffectBeaconStatus::Value status_; //!< The status of the QuestEffectBeacon, Can be eighter active or inactive.

            bool setTimes(const int & n); //!< Set the number of times the QuestEffectBeacon can be executed.
            bool addEffect(QuestEffect* effect); //!< Add a QuestEffect to the QuestEffectBeacon.

            const QuestEffect* getEffect(unsigned int index) const; //!< Get the QuestEffect at a given index.

    };

}

#endif /* _QuestEffectBeacon_H__ */
