
//My Flocking Class

#ifndef Flocking_Class
#define Flocking_Class

#include <Ogre.h>
#include <OgreVector3.h>


#endif

using namespace std;
using namespace Ogre;

class Element // An element that flocks
{

  public:
    Vector3 location;  // locationvector of the element
    Vector3 speed;  // speedvector of the element
    Vector3 acceleration;  // accelerationvector of the element


  Element(Vector3 location_, Vector3 speed_, Vector3 acceleration_) {
    acceleration = acceleration_;
    speed = speed_;
    location = location_;
  }

  //calculates the distance between the element and an other point given by temp
  float getDistance(Element temp) {
    Vector3 distance = temp.location-location;  //this doesn't work
    return distance.length();
  }

//EINF[GEN DES ELEMENTS
  void update(Element* arrayOfElements) {
    calculateAcceleration(arrayOfElements);  //updates the acceleration
    calculateSpeed();  //updates the speed
    calculateLocation();  //updates the location
  }

//EINF[GEN DES ELEMENTS
  void calculateAcceleration(Element* arrayOfElements) {
  //calculates the accelerationvector based on the steeringvectors of
  //separtion, alignment and cohesion.
  acceleration = acceleration + separation(arrayOfElements) + 2*alignment(arrayOfElements) + 2*cohesion(arrayOfElements);
  }

  void calculateSpeed() {
  speed = speed + acceleration;
  //speed = speed.normalise();
  }

  void calculateLocation() {
  location = location + speed;
  acceleration = (0,0,0);  //set acceleration to zero for the next calculation
  }

  Vector3 separation(Element* arrayOfElements) {
    Vector3 steering; //steeringvector
    int numberOfNeighbour;  //number of observed neighbours
    //go through all elements
    for(int i=1; i<3; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
//DUMMY SEPERATION DETECTION DISTANCE = 25
      if ((distance > 0) && (distance<1)) {  //do only if actual is inside detectionradius
        Vector3 inverseDistance = actual.location-location;  //calculate the distancevector heading towards this
        inverseDistance = inverseDistance.normalise(); //does this work correctly?  //normalise the distancevector
        inverseDistance = inverseDistance/*/distance;*/ ;  //devide distancevector by distance (the closer the bigger gets the distancevector -> steeringvector)
        steering = steering + inverseDistance;  //add up all significant steeringvectors
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) {
    steering = steering / (float)numberOfNeighbour;  //devide the sum of steeringvectors by the number of elements -> separation steeringvector
    }
    return steering;
  }

  Vector3 alignment(Element* arrayOfElements) {
    Vector3 steering; //steeringvector
    int numberOfNeighbour;  //number of observed neighbours
    //go through all elements
    for(int i=1; i<3; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
//DUMMY ALIGNMENT DETECTION DISTANCE = 50
      if ((distance > 0) && (distance<1000)) {  //check if actual element is inside detectionradius
        steering = steering + actual.speed;  //add up all speedvectors inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) {
    steering = steering / (float)numberOfNeighbour;  //devide the sum of steeringvectors by the number of elements -> alignment steeringvector
    }
    return steering;
  }

  Vector3 cohesion(Element* arrayOfElements) {
    Vector3 steering; //steeringvector
    int numberOfNeighbour;  //number of observed neighbours
    //go through all elements
    for(int i=1; i<3; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
// DUMMY COHESION DETECTION DISTANCE = 50
      if ((distance > 0) && (distance<1000)) {  //check if actual element is inside detectionradius
        steering = steering + actual.location;  //add up all locations of elements inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
     }
    if(numberOfNeighbour > 0) {
    steering = steering  / (float)numberOfNeighbour;  //devide the sum steeringvector by the number of elements -> cohesion steeringvector
    }
  }

};



//End of My Flocking Class
