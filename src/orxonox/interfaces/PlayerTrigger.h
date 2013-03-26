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
    @file PlayerTrigger.h
    @brief Definition of the PlayerTrigger class.
    @ingroup Triggers
*/

#ifndef _PlayerTrigger_H__
#define _PlayerTrigger_H__

#include "OrxonoxPrereqs.h"

#include "core/class/OrxonoxInterface.h"
#include "core/object/WeakPtr.h"

namespace orxonox
{
    /**
    @brief
        PlayerTrigger is an interface if implemented by a specific trigger can be used to recover the Player (or the @ref orxonox::Pawn "Pawn") that triggered it.

    @author
        Damian 'Mozork' Frick

    @ingroup Triggers
    */
    class _OrxonoxExport PlayerTrigger : virtual public OrxonoxInterface
    {
    public:
        PlayerTrigger();
        virtual ~PlayerTrigger() {}

        /**
        @brief Returns the Pawn that triggered the PlayerTrigger.
        @return Returns a pointer to the Pawn that triggered the PlayerTrigger.
        */
        inline Pawn* getTriggeringPawn(void) const
            { return this->pawn_.get(); }

        /**
        @brief Returns the player that triggered the PlayerTrigger.
        @return Returns a pointer to the PlayerInfo that triggered the PlayerTrigger.
        */
        inline PlayerInfo* getTriggeringPlayer(void) const
            { return this->player_; }

        /**
        @brief Checks whether the PlayerTrigger normally returns a Pawn/PlayerInfo.
        @return Returns true if the PlayerTrigger normally returns a Pawn/PlayerInfo.
        */
        inline bool isForPlayer(void) const
           { return this->isForPlayer_; }

    protected:
        /**
        @brief Set the player that triggered the PlayerTrigger. This is normally done by classes inheriting vom PlayerTrigger.
        @param player A pointer to the Pawn that triggered the PlayerTrigger.
        */
        void setTriggeringPawn(Pawn* pawn);

        /**
        @brief Set whether the PlayerTrigger normally is triggered by Pawns.
        @param isForPlayer Should be true when the PlayerTrigger should be set to normally be triggered by Pawns, false if not.
        */
        inline void setForPlayer(bool isForPlayer)
           { this->isForPlayer_ = isForPlayer; }

    private:
        WeakPtr<PlayerInfo> player_; //!< The player that triggered the PlayerTrigger.
        WeakPtr<Pawn> pawn_; //!< The Pawn that triggered the PlayerTrigger.
        bool isForPlayer_; //!< Is true when the PlayerTrigger should be set to normally be triggered by Pawns.

    };

}

#endif /* _PlayerTrigger_H__ */
