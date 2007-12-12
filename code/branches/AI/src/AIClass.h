#ifndef AI_Class
#define AI_Class

#include <Ogre.h>
#include <OgreVector3.h>
#include <OgreMath.h>

#include <iostream>


#endif

using namespace std;
using namespace Ogre;

class AIPilot {


  /*
  TASKS:                                                                         Requirements:

    - Change actual Speed to required Speed                                          - Speed as Vector3 , spaceship(?) -> pilot attached to it or vice versa (?)
    - Open fire, if allowed                                                          - Status of the fleet commander -> allowed to fire , fly etc.
    - differ friend, ally and enemy                                                  - Lists of ships + who controls them
    - report ship status to fleet commander                                          - Call own ship status -> ammo, damage etc.



  */

  public:
    Vector3 speed;



  //default constructor
  AIPilot() {
     speed = (1,0,0);
  }

  Vector3 steer(Vector3 reqSpeed) {
    Quaternion sRotation = speed.getRotationTo(reqSpeed);
    Radian rollarc = sRotation.getRoll();
    Radian pitcharc = sRotation.getPitch();
    Radian yawarc = sRotation.getYaw();
    return Vector3((float)rollarc.valueRadians(), (float)pitcharc.valueRadians(), (float)yawarc.valueRadians());
  }



};

class AIFleetCommander : public  AIPilot {


  /*
  TASKS:                                                                       Requirements:

    - Same tasks as AI_Pilot (only if shipcontrol)                                   - Same as AI_Pilot
    - Calculate required Speed for any ship in Fleet                                 - Knows all calculation functions
    - Request fleet status and report to commander                                   - Better list of all ships -> more data for fleet ships



  */

  public:


  //default constructor
  AIFleetCommander() {

  }



};

class AICommander : public  AIFleetCommander {


  /*
  TASKS:                                                                     Requirements:

    - Decide on general tactics                                                      - List of general fleet status + all possible information on enemies
    - Save and load tactical data (?)                                                - files with tactical data, e.g. success of strategy etc.



  */

  //default constructor
  AICommander() {

  }



};
