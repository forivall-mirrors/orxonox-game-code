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

#ifndef _Masterable_h__
#define _Masterable_h__

#include "OrxonoxPrereqs.h"

#include <vector>
#include "core/Super.h"

#include "util/Math.h"
#include "core/OrxonoxClass.h"
#include "controllers/Controller.h"
#include "worldentities/ControllableEntity.h"


namespace orxonox {

  class _OrxonoxExport Masterable : public Controller
  {

      public:
      Masterable(BaseObject* creator);

      virtual ~Masterable();

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
           { this->team_ = team; 
             orxout(debug_output) << "Set team to: "<<team<<" in "<<this<< endl;} 
      inline int getTeam() const
           { return this->team_; }

      /**
        @brief Mode of the formation, behaviour of slaves
               Normal-normal behaviour
               Defend-just defend the master
               Attack-leave formation, attack every target
      */ 
      enum Mode {NORMAL,DEFEND,ATTACK};
      
      /**
        @brief Sets the new mode. If master, set it for all slaves.
      */
      void setMode(Mode val);
      inline Mode getMode() const
           { return this->mode_; }

    protected:
      bool formationFlight_;
      bool passive_;
      int team_;
      unsigned int maxFormationSize_;
      int freedomCount_;
      enum State {SLAVE, MASTER, FREE};
      
      State state_;
      std::vector<Masterable*> slaves_;
      Masterable* myMaster_;

      Mode mode_;

      enum SpecificMasterAction {NONE, HOLD, SPIN, TURN180, FOLLOW};
      SpecificMasterAction specificMasterAction_;
      int specificMasterActionHoldCount_;
      float speedCounter_; //for speed adjustment when following

      void moveToPosition(const Vector3& target);
      void moveToTargetPosition();
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
#endif /* _Masterable_h__ */


