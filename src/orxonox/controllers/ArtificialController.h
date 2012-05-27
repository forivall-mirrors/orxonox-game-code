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
#include "controllers/FormationController.h"


namespace orxonox
{
    class _OrxonoxExport ArtificialController : public FormationController
    {
        public:
            ArtificialController(BaseObject* creator);
            virtual ~ArtificialController();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void abandonTarget(Pawn* target);

            virtual void changedControllableEntity();

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

            void aimAtTarget();

            bool isCloseAtTarget(float distance) const;
            bool isLookingAtTarget(float angle) const;

            float botlevel_; //<! Makes the level of a bot configurable.
            enum Mode {DEFAULT, ROCKET, DEFENCE, MOVING};//TODO; implement DEFENCE, MOVING modes
            Mode mode_; //TODO: replace single value with stack-like implementation: std::vector<Mode> mode_;
            void setPreviousMode();


            //WEAPONSYSTEM DATA
            std::map<std::string, int> weaponModes_; //<! Links each "weapon" to it's weaponmode - managed by setupWeapons()
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

            void boostControl(); //<! Sets and resets the boost parameter of the spaceship. Bots alternate between boosting and saving boost.

        private:
    };
}

#endif /* _ArtificialController_H__ */
