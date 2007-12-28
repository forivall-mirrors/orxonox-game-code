
// Arrival Class


#ifndef _Arrival_H__
#define _Arrival_H__

#include "misc/Vector3.h"
#include "misc/Quaternion.h"

namespace orxonox
{
  class Arrival
  {
  public:
    Vector3 location;           //!< locationvector of the element
    Vector3 speed;              //!< speedvector of the element
    Vector3 acceleration;       //!< accelerationvector of the element
    Vector3 target;             //!< target to arrive
    int accelerationForwards;   //!< from steering-interface
    int MaxSpeed;               //!< from steering-interface


    Arrival() {
      acceleration = (0,0,0);
      speed = (0,0,0);
      location = (0,0,0);
      target = (0,0,0);
    }

    Arrival(Vector3 location_, Vector3 speed_, Vector3 acceleration_, Vector3 target_) {
      acceleration = acceleration_;
      speed = speed_;
      location = location_;
      target = target_;
    }

    void setValues(Vector3 location_, Vector3 speed_, Vector3 acceleration_, Vector3 target_) {
      acceleration = acceleration_;
      speed = speed_;
      location = location_;
      target = target_;
    }

    void setTarget(Vector3 target_) {
      setValues(this->location, this->speed, this->acceleration, target_);
    }

    Vector3 getDirection() {
      Vector3 direction = target-location;
    }

    double relativeDirectApproach() {
      // Maxspeed / accelerationForwards = time needed to break with max acceleration
      // 2*getDistance()length/(MaxSpeed/accelerationForwards)^2 = required acceleration to arrive at the target with speed = 0
      return (accelerationForwards / (2*getDirection().length() / ((MaxSpeed/accelerationForwards)*(MaxSpeed/accelerationForwards))) );
    }

    void Approach() {
      Quaternion rotation = Quaternion(0,0,0,0);
      if (relativeDirectApproach() > 1)
      {
        float length = speed.length();
        speed = (speed+getDirection());
        speed.normalise();
        speed = speed*length;
        if (relativeDirectApproach() > 4)
        {
          //accelerate
        }
        else
        {
          // speed will stay constant
        }
      }
      else {
      }
    }

  };
}

#endif /* _Arrival_H__ */