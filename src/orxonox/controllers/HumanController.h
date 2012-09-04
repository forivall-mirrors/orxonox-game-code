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

#ifndef _HumanController_H__
#define _HumanController_H__

#include "OrxonoxPrereqs.h"

#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "FormationController.h"

// tolua_begin
namespace orxonox
{
    class _OrxonoxExport HumanController
// tolua_end
        : public FormationController, public Tickable
    { // tolua_export
        public:
            HumanController(BaseObject* creator);
            virtual ~HumanController();

            virtual void tick(float dt);

            static void moveFrontBack(const Vector2& value);
            static void moveRightLeft(const Vector2& value);
            static void moveUpDown(const Vector2& value);

            static void rotateYaw(const Vector2& value);
            static void rotatePitch(const Vector2& value);
            static void rotateRoll(const Vector2& value);

            virtual void frontback(const Vector2& value);
            virtual void yaw(const Vector2& value);
            virtual void pitch(const Vector2& value);

            static void fire(unsigned int firemode);
            virtual void doFire(unsigned int firemode);
            static void reload();

            static void keepBoost(); // Static method, keeps boosting.
            /**
            @brief Check whether the HumanController is in boosting mode.
            @return Returns true if it is, false if not.
            */
            inline bool isBoosting(void)
                { return this->boosting_; }
            void keepBoosting(void);
            void terminateBoosting(void);


            static void greet();
            static void switchCamera();
            static void mouseLook();
            static void mouseLookOFF();
            static void cycleNavigationFocus();
            static void releaseNavigationFocus();

            static void suicide();
            static void toggleGodMode();
            static void myposition();

            static void toggleFormationFlight();
            static void FFChangeMode();

            static void addBots(unsigned int amount);
            static void killBots(unsigned int amount = 0);

            static void pauseControl(); // tolua_export
            static void resumeControl(); // tolua_export
            virtual void doPauseControl() {};
            virtual void doResumeControl() {};

            static inline HumanController* getLocalControllerSingleton()
                { return HumanController::localController_s; }
            static Pawn* getLocalControllerEntityAsPawn();
            //friend class, for mouselook
            friend class Map;

        protected:
            static HumanController* localController_s;
            bool controlPaused_;

        private:
            bool boosting_; // Whether the HumanController is in boosting mode or not.
            Timer boostingTimeout_; // A timer to check whether the player is no longer boosting.
            static const float BOOSTING_TIME; // The time after it is checked, whether the player is no longer boosting.

    }; // tolua_export
} // tolua_export

#endif /* _HumanController_H__ */
