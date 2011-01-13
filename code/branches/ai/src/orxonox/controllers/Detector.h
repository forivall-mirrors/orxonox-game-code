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
 *   GNU General Public License for more detls.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Detector.h
    @brief Definition of the Dectector class.
*/

#ifndef _Detector_H__
#define _Detector_H__

#include "OrxonoxPrereqs.h"

//#include "core/BaseObject.h"
#include "ArtificialController.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport Detector : public ArtificialController, public Tickable
    {
        public:
            Detector(BaseObject* creator);
            virtual ~Detector();
            
            virtual void tick(float dt);//< Each tick the actual situation is compared to the previous situation.
            void checkHealth();//< Calls analyseAttack, if life is lost. TODO: what about damage of non-enemy sources?
            bool checkUnderAttack(); //< Returns false if the enemy's attack (lifeloss) is temporarly over.
            
            
            void analyseAttack(); //
            
        protected:
            float botLevel;
            WeakPtr<Pawn> bot_;
            
            float healthLost;
            float previousHealth;
            float notUnderAttackTime;
            bool bUnderAttack;
            WeakPtr<Pawn> attacker_;

    };
}

#endif /* _Detector_H__ */
