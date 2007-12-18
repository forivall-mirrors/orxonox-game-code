/**
  @file NPC.h
  @brief AI using WorldEntity
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef MODULE_NPC_H
#define MODULE_NPC_H

// includes
#include "WorldEntity.h"
#include "Model.h"

namespace orxonox {

  class NPC : public Model
  {
    public:

      NPC();
      virtual ~NPC();
      void tick(float dt);
      void setValues(Vector3 location, Vector3 speed, Vector3 acceleration, bool movable);

    private:
      float getDistance(NPC* temp);
      void calculateAcceleration(NPC** arrayOfElements);
      Vector3 separation(NPC** arrayOfElements);
      Vector3 alignment(NPC** arrayOfElements);
      Vector3 cohesion(NPC** arrayOfElements);


    private:
      bool movable_;  // movability of the element, (false) gives the possiblity that an object can`t be moved by flocking but still gets into the calculation

      // those values should maybe be controlled by a higher power and should maybe be parameters that can be changed
      static int const SEPERATIONDISTANCE = 300;  //detectionradius of seperation
      static int const ALIGNMENTDISTANCE = 300;  //detectionradius of alignment
      static int const COHESIONDISTANCE = 5000;  //detectionradius of cohesion
      static int const ANZELEMENTS = 9;  //number of elements
  };

}
#endif
