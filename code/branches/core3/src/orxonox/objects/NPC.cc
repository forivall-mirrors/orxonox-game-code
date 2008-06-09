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
 *      Benjamin Knecht, beni_at_orxonox.net
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "NPC.h"

#include "core/CoreIncludes.h"

namespace orxonox {

  CreateFactory(NPC);

  NPC::NPC() :
    movable_(true)
  {
    RegisterObject(NPC);
    registerAllVariables();
  }

  NPC::~NPC()
  {
  }

  void NPC::loadParams(TiXmlElement* xmlElem)
  {
    Model::loadParams(xmlElem);
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

  void NPC::registerAllVariables(){
    Model::registerAllVariables();
    registerVar(&movable_, sizeof(movable_), network::DATA);
  }


  /**
   * calculates the distance between the element and an other point given by temp
   */
  float NPC::getDistance(WorldEntity* temp)
  {
    Vector3 distance = temp->getPosition() - this->getPosition();
    return distance.length();
  }

  /**
   * updates the data of an element
   */
  void NPC::update()
  {

    //if element is movable, calculate acceleration
    if (this->movable_ == true) calculateAcceleration();

  }

  /**
   * tick this NPC
   */
  void NPC::tick(float dt)
  {
    update();
    this->setVelocity(0.995*this->getVelocity() + this->getAcceleration()*dt);
    this->translate(this->getVelocity()*dt);
    this->setAcceleration(Vector3(0,0,0));
  }

  /**
   * calculates the new acceleration of an element
   */
  void NPC::calculateAcceleration()
  {
    //acceleration consisting of flocking-functions
    this->setAcceleration(separation() + alignment() + cohesion());
  }

  /**
   * separation-function (keep elements separated, avoid crashs)
   */
  Vector3 NPC::separation()
  {
    Vector3 steering = Vector3(0,0,0); //steeringvector
    Vector3 inverseDistance = Vector3(0,0,0);  //vector pointing away from possible collisions
    int numberOfNeighbour = 0;  //number of observed neighbours
    float distance = 0;  // distance to the actual element
    for(Iterator<WorldEntity> it = ObjectList<WorldEntity>::begin(); it; ++it) {  //go through all elements
      distance = getDistance(*it);  //get distance between this and actual
      if ((distance > 0) && (distance < SEPERATIONDISTANCE)) {  //do only if actual is inside detectionradius
        inverseDistance = Vector3(0,0,0);
        inverseDistance = this->getPosition() - it->getPosition();  //calculate the distancevector heading towards this
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
  Vector3 NPC::alignment()
  {
    Vector3 steering = Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //float distance = 0;
    //go through all elements
    for(Iterator<NPC> it = ObjectList<NPC>::begin(); it; ++it) {  //just working with 3 elements at the moment
      float distance = getDistance(*it);  //get distance between this and actual
      if ((distance > 0) && (distance < ALIGNMENTDISTANCE)) {  //check if actual element is inside detectionradius
        steering = steering + it->getVelocity();  //add up all speedvectors inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) { steering = steering / (float)numberOfNeighbour; }  //devide the sum of steeringvectors by the number of elements -> alignment steeringvector
    return steering;
  }

  /**
   * cohseion-function (keep elements close to each other)
   */
  Vector3 NPC::cohesion()
  {
    Vector3 steering = Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //float distance = 0;
    //go through all elements
    for(Iterator<NPC> it = ObjectList<NPC>::begin(); it; ++it) {  //just working with 3 elements at the moment
      float distance = getDistance(*it);  //get distance between this and actual
      if ((distance > 0) && (distance < COHESIONDISTANCE)) {  //check if actual element is inside detectionradius
        steering = steering + it->getPosition();  //add up all locations of elements inside the detectionradius
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
