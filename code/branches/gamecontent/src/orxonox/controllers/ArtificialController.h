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
 *      Dominik Solenicki
 *
 */

#ifndef _ArtificialController_H__
#define _ArtificialController_H__

#include "OrxonoxPrereqs.h"

#include <map>

#include "util/Math.h"
#include "Controller.h"
#include "controllers/NewHumanController.h"
#include "weaponsystem/WeaponSystem.h"

namespace orxonox
{
    class _OrxonoxExport ArtificialController : public Controller
    {
        public:
            ArtificialController(BaseObject* creator);
            virtual ~ArtificialController();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void abandonTarget(Pawn* target);

            inline void setTeam(int team) //TODO: write through to controllable entity.
                { this->team_ = team; }
            inline int getTeam() const
                { return this->team_; }

            inline void setFormationFlight(bool formation)
                { this->formationFlight_ = formation; }
            inline bool getFormationFlight() const
                { return this->formationFlight_; }

            inline void setFormationSize(int size)
                { this->maxFormationSize_ = size; }
            inline int getFormationSize() const
                { return this->maxFormationSize_; }

            inline void setPassive(bool passive)
                { this->passive_ = passive; }
            inline bool getPassive() const
                { return this->passive_; }

            virtual void changedControllableEntity();

            static void formationflight(const bool form);
            static void masteraction(const int action);
            static void followme();
            static void passivebehaviour(const bool passive);
            static void formationsize(const int size);

            virtual void doFire();
            void setBotLevel(float level=1.0f);
            inline float getBotLevel() const
                { return this->botlevel_; }
            static void setAllBotLevel(float level);
            //WAYPOINT FUNCTIONS
            void addWaypoint(WorldEntity* waypoint);
            WorldEntity* getWaypoint(unsigned int index) const;

            inline void setAccuracy(float accuracy)
                { this->squaredaccuracy_ = accuracy*accuracy; }
            inline float getAccuracy() const
                { return sqrt(this->squaredaccuracy_); }
            void updatePointsOfInterest(std::string name, float distance);
            void manageWaypoints();

        protected:

            int team_;
            bool formationFlight_;
            bool passive_;
            unsigned int maxFormationSize_;
            int freedomCount_;
            enum State {SLAVE, MASTER, FREE};
            State state_;
            std::vector<ArtificialController*> slaves_;
            ArtificialController *myMaster_;
            enum SpecificMasterAction {NONE, HOLD, SPIN, TURN180, FOLLOW};
            SpecificMasterAction specificMasterAction_;
            int specificMasterActionHoldCount_;
            float speedCounter_; //for speed adjustment when following

            void moveToPosition(const Vector3& target);
            void moveToTargetPosition();
            void absoluteMoveToPosition(const Vector3& target);

            virtual void positionReached() {}

            void removeFromFormation();
            void unregisterSlave();
            void searchNewMaster();
            void commandSlaves();
            void setNewMasterWithinFormation();

            void freeSlaves();
            void forceFreeSlaves();
            void loseMasterState();
            void forceFreedom();
            bool forcedFree();

            void specificMasterActionHold();
            void turn180Init();
            void turn180();
            void spinInit();
            void spin();
            void followInit(Pawn* pawn, const bool always = false, const int secondsToFollow = 100);
            void followRandomHumanInit();
            void follow();
            void followForSlaves(const Vector3& target);

            void setTargetPosition(const Vector3& target);
            void searchRandomTargetPosition();

            void setTarget(Pawn* target);
            void searchNewTarget();
            void forgetTarget();
            void aimAtTarget();

            bool isCloseAtTarget(float distance) const;
            bool isLookingAtTarget(float angle) const;

            void targetDied();

            static bool sameTeam(ControllableEntity* entity1, ControllableEntity* entity2, Gametype* gametype); // hack
            void boostControl(); //<! Sets and resets the boost parameter of the spaceship. Bots alternate between boosting and saving boost.

            bool bHasTargetPosition_;
            Vector3 targetPosition_;
            WeakPtr<Pawn> target_;
            bool bShooting_;

            float botlevel_; //<! Makes the level of a bot configurable.
            enum Mode {DEFAULT, ROCKET, DEFENCE, MOVING};//TODO; implement DEFENCE, MOVING modes
            Mode mode_; //TODO: replace single value with stack-like implementation: std::vector<Mode> mode_;
            void setPreviousMode();

            //WEAPONSYSTEM DATA
            std::map<std::string, int> weaponModes_; //<! Links each "weapon" to it's weaponmode- managed by setupWeapons()
            //std::vector<int> projectiles_; //<! Displays amount of projectiles of each weapon. - managed by setupWeapons()
            float timeout_; //<! Timeout for rocket usage. (If a rocket misses, a bot should stop using it.)
            void setupWeapons(); //<! Defines which weapons are available for a bot. Is recalled whenever a bot was killed.
            bool bSetupWorked; //<! If false, setupWeapons() is called.
            int getFiremode(std::string name);

            //WAYPOINT DATA
            std::vector<WeakPtr<WorldEntity> > waypoints_;
            size_t currentWaypoint_;
            float squaredaccuracy_;
            WorldEntity* defaultWaypoint_;
    };
}

#endif /* _ArtificialController_H__ */
