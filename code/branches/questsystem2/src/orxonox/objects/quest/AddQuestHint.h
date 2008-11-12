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
    @file AddQuestHint.h
    @brief
	Definition of the AddQuestHint class.
*/

#ifndef _AddQuestHint_H__
#define _AddQuestHint_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include "QuestEffect.h"

namespace orxonox {

    /**
    @brief
        Adds a QuestHint, resp. activates the QuestHint of the given id for the player the QuestEffect is invoked on.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport AddQuestHint : public QuestEffect
    {
        public:
            AddQuestHint(BaseObject* creator);
            virtual ~AddQuestHint();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a AddQuestHint object through XML.

            virtual bool invoke(ControllableEntity* player); //!< Invokes the QuestEffect.

        private:
            std::string hintId_; //!< The id of the QuestHint.

            /**
            @brief Returns the id of the QuestHint.
            @return Returns the id of the QuestHint.
            */
            inline const std::string & getHintId(void) const
                { return this->hintId_; }
            bool setHintId(const std::string & id); //!< Sets the id of the QuestHint.

    };

}

#endif /* _AddQuestHint_H__ */
