
//Headerfile: Flocking.h

#ifndef Flocking_Class
#define Flocking_Class

// #include <Ogre.h>
#include <OgreVector3.h>


#include <iostream>


#endif


class Element // An element that flocks
{

  public:
    Ogre::Vector3 location;                       //!< locationvector of the element
    Ogre::Vector3 speed;                          //!< speedvector of the element
    Ogre::Vector3 acceleration;                   //!< accelerationvector of the element
    bool movable;                                 //!< movability of the element, (false) gives the possiblity that an object can`t be moved by flocking but still gets into the calculation
    static int const SEPERATIONDISTANCE = 300;    //!< detectionradius of seperation
    static int const ALIGNMENTDISTANCE = 300;     //!< detectionradius of alignment
    static int const COHESIONDISTANCE = 5000;     //!< detectionradius of cohesion
    static int const ANZELEMENTS = 9;             //!< number of elements

  //! default constructor
  Element() {
    acceleration = Ogre::Vector3(0,0,0);
    speed = Ogre::Vector3(0,0,0);
    location = Ogre::Vector3(0,0,0);
    movable = true;
  }

  /** constructor
   *  @param location_ sets locationvector of the element
   *  @param speed_ sets speedvector of the element
   *  @param acceleration_ sets accelerationvector of the element
   *  @param movable_ sets movability of the element
   */
  Element(Ogre::Vector3 location_, Ogre::Vector3 speed_, Ogre::Vector3 acceleration_, bool movable_) {
    acceleration = acceleration_;
    speed = speed_;
    location = location_;
    movable = movable_;
  }

  //! function to chance values of an element
  void setValues(Ogre::Vector3 location_, Ogre::Vector3 speed_, Ogre::Vector3 acceleration_, bool movable_) {
    acceleration = acceleration_;
    speed = speed_;
    location = location_;
    movable = movable_;
  }

  /** calculates the distance between the element and an other point given by temp
   * @param e remote object to calculate distance to
   */
  float getDistance(Element e) {
    Ogre::Vector3 distance = e.location - location;
    return distance.length();
  }

  //! updates the data of an element
  void update(Element arrayOfElements[]) {
    if (this->movable == true) {calculateAcceleration(arrayOfElements);} //if element is movable, calculate acceleration
  }

  //! calculates the new acceleration of an element
  void calculateAcceleration(Element arrayOfElements[]) {
    acceleration = separation(arrayOfElements) + alignment(arrayOfElements) + cohesion(arrayOfElements);  //acceleration consisting of flocking-functions
  }

  //! separation-function (keep elements separated, avoid crashs)
  Ogre::Vector3 separation(Element arrayOfElements[]) {
    using namespace Ogre;
    Vector3 steering = Vector3(0,0,0); //steeringvector
    Vector3 inverseDistance = Vector3(0,0,0);  //vector pointing away from possible collisions
    int numberOfNeighbour = 0;  //number of observed neighbours
    float distance = 0;  // distance to the actual element
    for(int i=0; i<ANZELEMENTS; i++) {  //go through all elements
      Element actual = arrayOfElements[i];  //get the actual element
      distance = getDistance(actual);  //get distance between this and actual
      if ((distance > 0) && (distance < SEPERATIONDISTANCE)) {  //do only if actual is inside detectionradius
        inverseDistance = (0,0,0);
        inverseDistance = location-actual.location;  //calculate the distancevector heading towards this
        //adaptation of the inverseDistance to the distance
        if ((distance < 200) && (distance >= 120)) {inverseDistance = 2*inverseDistance;}
        if ((distance < 120) && (distance >= 80)) {inverseDistance = 5*inverseDistance;}
        if ((distance < 80) && (distance >= 40)) {inverseDistance = 10*inverseDistance;}
        if ((distance < 40) && (distance > 0)) {inverseDistance = 10*inverseDistance;}
        steering = steering + inverseDistance;  //add up all significant steeringvectors
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) { steering = steering / (float)numberOfNeighbour; }  //devide the sum of steeringvectors by the number of elements -> separation steeringvector
    return steering;
  }

  //! alignment-function (lead elements to the same heading)
  Ogre::Vector3 alignment(Element arrayOfElements[]) {
    using namespace Ogre;
    Vector3 steering = Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    float distance = 0;
    //go through all elements
    for(int i=0; i<ANZELEMENTS; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
      if ((distance > 0) && (distance < ALIGNMENTDISTANCE)) {  //check if actual element is inside detectionradius
        steering = steering + actual.speed;  //add up all speedvectors inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) { steering = steering / (float)numberOfNeighbour; }  //devide the sum of steeringvectors by the number of elements -> alignment steeringvector
    return steering;
  }

  //! cohseion-function (keep elements close to each other)
  Ogre::Vector3 cohesion(Element arrayOfElements[]) {
    using namespace Ogre;
    Vector3 steering = Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    float distance = 0;
    //go through all elements
    for(int i=0; i<ANZELEMENTS; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
      if ((distance > 0) && (distance < COHESIONDISTANCE)) {  //check if actual element is inside detectionradius
        steering = steering + actual.location;  //add up all locations of elements inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) {
      steering = steering  / (float)numberOfNeighbour;  //devide the sum steeringvector by the number of elements -> cohesion steeringvector
      steering = steering - this->location;  //transform the vector for the ship
    }
    return steering;
  }
};     //End of class Element
