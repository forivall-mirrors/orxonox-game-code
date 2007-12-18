/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Benjamin Knecht, beni_at_orxonox.net
 *   Co-authors:
 *      ...
 *
 */

#include "NPC.h"
#include "../core/Iterator.h"
#include "../core/ObjectList.h"
//#include "../Flocking.h"

namespace orxonox {

  NPC::NPC()
  {
    movable_ = true;
  }

  NPC::~NPC()
  {
  }

  /**
   * function to chance values of an element
   */
  void NPC::setValues(Vector3 location, Vector3 speed, Vector3 acceleration, bool movable) {
    this->setAcceleration(acceleration);
    this->setVelocity(speed);
    this->translate(location);
    movable_ = movable;
  }

  /**
   * calculates the distance between the element and an other point given by temp
   */
  float NPC::getDistance(NPC* temp)
  {
    Vector3 distance = temp->getPosition() - this->getPosition();
    return distance.length();
  }

  /**
   * updates the data of an element
   */
  void NPC::tick(float dt)
  {

    // find out about this arrayOfElements
    NPC* arrayOfElements[ANZELEMENTS];

    //if element is movable, calculate acceleration
    if (this->movable_ == true) calculateAcceleration(arrayOfElements);

  }

  /**
   * calculates the new acceleration of an element
   */
  void NPC::calculateAcceleration(NPC** arrayOfElements)
  {
    //acceleration consisting of flocking-functions
    this->setAcceleration(separation(arrayOfElements) + alignment(arrayOfElements) + cohesion(arrayOfElements));
  }

  /**
   * separation-function (keep elements separated, avoid crashs)
   */
  Vector3 NPC::separation(NPC** arrayOfElements)
  {
    Vector3 steering = Vector3(0,0,0); //steeringvector
    Vector3 inverseDistance = Vector3(0,0,0);  //vector pointing away from possible collisions
    int numberOfNeighbour = 0;  //number of observed neighbours
    float distance = 0;  // distance to the actual element
    for(int i=0; i<ANZELEMENTS; i++) {  //go through all elements
      NPC* actual = arrayOfElements[i];  //get the actual element
      distance = getDistance(actual);  //get distance between this and actual
      if ((distance > 0) && (distance < SEPERATIONDISTANCE)) {  //do only if actual is inside detectionradius
        inverseDistance = Vector3(0,0,0);
        inverseDistance = this->getPosition() - actual->getPosition();  //calculate the distancevector heading towards this
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

  /**
   * alignment-function (lead elements to the same heading)
   */
  Vector3 NPC::alignment(NPC** arrayOfElements)
  {
    Vector3 steering = Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //float distance = 0;
    //go through all elements
    for(int i=0; i<ANZELEMENTS; i++) {  //just working with 3 elements at the moment
      NPC* actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
      if ((distance > 0) && (distance < ALIGNMENTDISTANCE)) {  //check if actual element is inside detectionradius
        steering = steering + actual->getVelocity();  //add up all speedvectors inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) { steering = steering / (float)numberOfNeighbour; }  //devide the sum of steeringvectors by the number of elements -> alignment steeringvector
    return steering;
  }

  /**
   * cohseion-function (keep elements close to each other)
   */
  Vector3 NPC::cohesion(NPC** arrayOfElements)
  {
    Vector3 steering = Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //float distance = 0;
    //go through all elements
    for(int i=0; i<ANZELEMENTS; i++) {  //just working with 3 elements at the moment
      NPC* actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
      if ((distance > 0) && (distance < COHESIONDISTANCE)) {  //check if actual element is inside detectionradius
        steering = steering + actual->getPosition();  //add up all locations of elements inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) {
      steering = steering  / (float)numberOfNeighbour;  //devide the sum steeringvector by the number of elements -> cohesion steeringvector
      steering = steering - this->getPosition();  //transform the vector for the ship
    }
    return steering;
  }

} // end of class NPC
