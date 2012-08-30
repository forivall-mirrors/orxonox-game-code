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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#ifndef _FormationController_h__
#define _FormationController_h__

#include "OrxonoxPrereqs.h"

#include <vector>
#include "core/Super.h"

#include "util/Math.h"
#include "core/OrxonoxClass.h"
#include "controllers/Controller.h"
#include "worldentities/ControllableEntity.h"


namespace orxonox {

  class _OrxonoxExport FormationController : public Controller
  {

      public:
      FormationController(BaseObject* creator);

      virtual ~FormationController();

      virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);


      static void formationflight(const bool form);
      static void masteraction(const int action);
      static void followme();
      static void passivebehaviour(const bool passive);
      static void formationsize(const int size);

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

      inline void setTeam(int team)
           { this->team_ = team; }
      inline int getTeam() const
           { return this->team_; }

      /**
        @brief Mode of the formation, behaviour of slaves
               Normal-normal behaviour
               Defend-just defend the master
               Attack-leave formation, attack every target
      */
      enum FormationMode {NORMAL,DEFEND,ATTACK};

      /**
        @brief Sets the new mode. If master, set it for all slaves.
      */
      void setFormationMode(FormationMode val);
      inline FormationMode getFormationMode() const
           { return this->formationMode_; }

      virtual void hit(Pawn* originator, btManifoldPoint& contactpoint, float damage);

  protected:
      bool formationFlight_;
      bool passive_;
      int team_;
      unsigned int maxFormationSize_;
      int freedomCount_;
      enum State {SLAVE, MASTER, FREE};

      State state_;
      std::vector<FormationController*> slaves_;
      FormationController* myMaster_;

      FormationMode formationMode_;

      enum SpecificMasterAction {NONE, HOLD, SPIN, TURN180, FOLLOW};
      SpecificMasterAction specificMasterAction_;
      int specificMasterActionHoldCount_;
      float speedCounter_; //for speed adjustment when following

      void moveToPosition(const Vector3& target);
      void moveToTargetPosition();
      void absoluteMoveToPosition(const Vector3& target);
      void copyOrientation(const Quaternion& orient);
      void copyTargetOrientation();

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

      void takeLeadOfFormation();
      void masterAttacked(Pawn* originator);

      void specificMasterActionHold();
      void turn180Init();
      void spinInit();
      void spin();
      void turn180();
      void followInit(Pawn* pawn, const bool always = false, const int secondsToFollow = 100);
      void followRandomHumanInit();
      void follow();

      void setTargetPosition(const Vector3& target);
      void searchRandomTargetPosition();

      void setTargetOrientation(const Quaternion& orient);
      void setTargetOrientation(Pawn* target);

      virtual void positionReached() {}

      static bool sameTeam(ControllableEntity* entity1, ControllableEntity* entity2, Gametype* gametype); // hack


      void setTarget(Pawn* target);
      void searchNewTarget();
      void forgetTarget();

      void targetDied();

      bool bHasTargetPosition_;
      Vector3 targetPosition_;
      bool bHasTargetOrientation_;
      Quaternion targetOrientation_;

      WeakPtr<Pawn> target_;
      bool bShooting_;
  };


}
#endif /* _FormationController_h__ */


