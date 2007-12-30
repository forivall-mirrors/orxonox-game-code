/**
  @file NPC.h
  @brief AI using WorldEntity
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _NPC_H__
#define _NPC_H__

#include "../OrxonoxPrereqs.h"

#include "Model.h"

class TiXmlElement; // Forward declaration

namespace orxonox {

  class NPC : public Model
  {
    public:

      NPC();
      virtual ~NPC();
      virtual void loadParams(TiXmlElement* xmlElem);
      void tick(float dt);
      void update();
      void setValues(Vector3 location, Vector3 speed, Vector3 acceleration, bool movable);

    private:
      float getDistance(WorldEntity* temp);
      void calculateAcceleration();
      Vector3 separation();
      Vector3 alignment();
      Vector3 cohesion();


    private:
      bool movable_;  // movability of the element, (false) gives the possiblity that an object can`t be moved by flocking but still gets into the calculation

      // those values should maybe be controlled by a higher power and should maybe be parameters that can be changed
      static int const SEPERATIONDISTANCE = 300;  //detectionradius of seperation
      static int const ALIGNMENTDISTANCE = 300;  //detectionradius of alignment
      static int const COHESIONDISTANCE = 5000;  //detectionradius of cohesion
      static int const NUMELEMENTS = 9;  //number of elements
  };

}
#endif /* _NPC_H__ */
