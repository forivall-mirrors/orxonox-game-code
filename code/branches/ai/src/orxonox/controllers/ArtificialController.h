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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ArtificialController_H__
#define _ArtificialController_H__

#include "OrxonoxPrereqs.h"

#include "util/Math.h"
#include "Controller.h"

namespace orxonox
{
    class _OrxonoxExport ArtificialController : public Controller
    {
        public:
            ArtificialController(BaseObject* creator);
            virtual ~ArtificialController();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);//new

            void abandonTarget(Pawn* target);

            inline void setTeam(int team)
                { this->team_ = team; }
            inline int getTeam() const
                { return this->team_; }
            virtual void changedControllableEntity();


        protected:

            int team_;
            int freedomCount_;
            enum State {SLAVE, MASTER, FREE};
            State state_;
            std::list<ArtificialController*> slaves_;

            void targetDied();

            void moveToPosition(const Vector3& target);
            //void speedToTargetPosition(const Vector3& target);
            void moveToTargetPosition();

            int getState();

            void unregisterSlave();
            void searchNewMaster();
            void commandSlaves();
            void setNewMasterWithinFormation();
            void freeSlaves();
            void forceFreeSlaves();
            void loseMasterState();
            void forceFreedom();
            bool forcedFree();

            ArtificialController *myMaster_;

            void setTargetPosition(const Vector3& target);
            void searchRandomTargetPosition();

            void setTarget(Pawn* target);
            void searchNewTarget();
            void forgetTarget();
            void aimAtTarget();

            bool isCloseAtTarget(float distance) const;
            bool isLookingAtTarget(float angle) const;

            static bool sameTeam(ControllableEntity* entity1, ControllableEntity* entity2, Gametype* gametype); // hack

            bool bHasTargetPosition_;
            Vector3 targetPosition_;
            WeakPtr<Pawn> target_;
            bool bShooting_;

        private:
    };
}

#endif /* _ArtificialController_H__ */
