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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "Ogre.h"
#include "SpaceshipSteering.h"
#include <iostream>
#include "core/Debug.h"

using namespace Ogre;

SpaceshipSteering::SpaceshipSteering(float maxSpeedForward, float
maxSpeedRotateUpDown, float maxSpeedRotateRightLeft, float
maxSpeedLoopRightLeft) {

  COUT(3) << "Info: Steering was loaded" << std::endl;
  moveForward_ = 0;
  rotateUp_ = 0;
  rotateDown_ = 0;
  rotateRight_ = 0;
  rotateLeft_ = 0;
  loopRight_ = 0;
  loopLeft_ = 0;
  brakeForward_ = 0;
  brakeRotate_ = 0;
  brakeLoop_ = 0;
  speedForward_ = 0;
  speedRotateUpDown_ = 0;
  speedRotateRightLeft_ = 0;
  speedLoopRightLeft_ = 0;
  maxSpeedForward_ = maxSpeedForward;
  maxSpeedRotateUpDown_ = maxSpeedRotateUpDown;
  maxSpeedRotateRightLeft_ = maxSpeedRotateRightLeft;
  maxSpeedLoopRightLeft_ = maxSpeedLoopRightLeft;
  accelerationForward_ = 0;
  accelerationRotateUpDown_ = 0;
  accelerationRotateRightLeft_ = 0;
  accelerationLoopRightLeft_ = 0;
}

void SpaceshipSteering::tick(float time) {

  if(moveForward_ > 0) {
    accelerationForward_ = moveForward_;
    if(speedForward_ < maxSpeedForward_)
      speedForward_ += accelerationForward_*time;
    if(speedForward_ > maxSpeedForward_)
      speedForward_ = maxSpeedForward_;
  }
  if(moveForward_ <= 0) {
    accelerationForward_ = brakeForward_;
    if(speedForward_ > 0)
      speedForward_ -= accelerationForward_*time;
    if(speedForward_ < 0)
      speedForward_ = 0;
  }

  if(rotateUp_ > 0) {
    accelerationRotateUpDown_ = rotateUp_;
    if(speedRotateUpDown_ < maxSpeedRotateUpDown_)
      speedRotateUpDown_ += accelerationRotateUpDown_*time;
    if(speedRotateUpDown_ > maxSpeedRotateUpDown_)
      speedRotateUpDown_ = maxSpeedRotateUpDown_;
  }
  if(rotateDown_ > 0) {
    accelerationRotateUpDown_ = rotateDown_;
    if(speedRotateUpDown_ > -maxSpeedRotateUpDown_)
      speedRotateUpDown_ -= accelerationRotateUpDown_*time;
    if(speedRotateUpDown_ < -maxSpeedRotateUpDown_)
      speedRotateUpDown_ = -maxSpeedRotateUpDown_;
  }
  if(rotateUp_ == 0 && rotateDown_ == 0) {
    accelerationRotateUpDown_ = brakeRotate_;
    if(speedRotateUpDown_ > 0)
      speedRotateUpDown_ -= accelerationRotateUpDown_*time;
    if(speedRotateUpDown_ < 0)
      speedRotateUpDown_ += accelerationRotateUpDown_*time;
    if(abs(speedRotateUpDown_)<accelerationRotateUpDown_*time)
      speedRotateUpDown_ = 0;
  }

  if(rotateRight_ > 0) {
    accelerationRotateRightLeft_ = rotateRight_;
    if(speedRotateRightLeft_ > -maxSpeedRotateRightLeft_)
      speedRotateRightLeft_ -= accelerationRotateRightLeft_*time;
    if(speedRotateRightLeft_ < -maxSpeedRotateRightLeft_)
      speedRotateRightLeft_ = -maxSpeedRotateRightLeft_;
  }
  if(rotateLeft_ > 0) {
    accelerationRotateRightLeft_ = rotateLeft_;
    if(speedRotateRightLeft_ < maxSpeedRotateRightLeft_)
      speedRotateRightLeft_ += accelerationRotateRightLeft_*time;
    if(speedRotateRightLeft_ > maxSpeedRotateRightLeft_)
      speedRotateRightLeft_ = maxSpeedRotateRightLeft_;
  }
  if(rotateRight_ == 0 && rotateLeft_ == 0) {
    accelerationRotateRightLeft_ = brakeRotate_;
    if(speedRotateRightLeft_ > 0)
      speedRotateRightLeft_ -= accelerationRotateRightLeft_*time;
    if(speedRotateRightLeft_ < 0)
      speedRotateRightLeft_ += accelerationRotateRightLeft_*time;
    if(abs(speedRotateRightLeft_)<accelerationRotateRightLeft_*time)
      speedRotateRightLeft_ = 0;
  }

  if(loopRight_ > 0) {
    accelerationLoopRightLeft_ = loopRight_;
    if(speedLoopRightLeft_ < maxSpeedLoopRightLeft_)
      speedLoopRightLeft_ += accelerationLoopRightLeft_*time;
    if(speedLoopRightLeft_ > maxSpeedLoopRightLeft_)
      speedLoopRightLeft_ = maxSpeedLoopRightLeft_;
  }
  if(loopLeft_ > 0) {
    accelerationLoopRightLeft_ = loopLeft_;
    if(speedLoopRightLeft_ > -maxSpeedLoopRightLeft_)
      speedLoopRightLeft_ -= accelerationLoopRightLeft_*time;
    if(speedLoopRightLeft_ < -maxSpeedLoopRightLeft_)
      speedLoopRightLeft_ = -maxSpeedLoopRightLeft_;
  }
  if(loopLeft_ == 0 && loopRight_ == 0) {
    accelerationLoopRightLeft_ = brakeLoop_;
    if(speedLoopRightLeft_ > 0)
      speedLoopRightLeft_ -= accelerationLoopRightLeft_*time;
    if(speedLoopRightLeft_ < 0)
      speedLoopRightLeft_ += accelerationLoopRightLeft_*time;
    if(abs(speedLoopRightLeft_)<accelerationLoopRightLeft_*time)
      speedLoopRightLeft_ = 0;
  }

  Vector3 transVector = Vector3::ZERO;
  transVector.z = 1;
  steeringNode_->translate(transVector*speedForward_*time,
  Node::TS_LOCAL);
  steeringNode_->pitch(Degree(speedRotateUpDown_*time),
  Node::TS_LOCAL);
  steeringNode_->yaw(Degree(speedRotateRightLeft_*time),
  Node::TS_LOCAL);
  steeringNode_->roll(Degree(speedLoopRightLeft_*time),
  Node::TS_LOCAL);

}

void SpaceshipSteering::moveForward(float moveForward) {
  moveForward_ = moveForward;
}

void SpaceshipSteering::rotateUp(float rotateUp) {
  rotateUp_ = rotateUp;
}

void SpaceshipSteering::rotateDown(float rotateDown) {
  rotateDown_ = rotateDown;
}

void SpaceshipSteering::rotateLeft(float rotateLeft) {
  rotateLeft_ = rotateLeft;
}

void SpaceshipSteering::rotateRight(float rotateRight) {
  rotateRight_ = rotateRight;
}

void SpaceshipSteering::loopLeft(float loopLeft) {
  loopLeft_ = loopLeft;
}

void SpaceshipSteering::loopRight(float loopRight) {
  loopRight_ = loopRight;
}

void SpaceshipSteering::brakeForward(float brakeForward) {
  brakeForward_ = brakeForward;
}

void SpaceshipSteering::brakeRotate(float brakeRotate) {
  brakeRotate_ = brakeRotate;
}

void SpaceshipSteering::brakeLoop(float brakeLoop) {
  brakeLoop_ = brakeLoop;
}

void SpaceshipSteering::maxSpeedForward(float maxSpeedForward) {
  maxSpeedForward_ = maxSpeedForward;
}

void SpaceshipSteering::maxSpeedRotateUpDown(float maxSpeedRotateUpDown) {
  maxSpeedRotateUpDown_ = maxSpeedRotateUpDown;
}

void SpaceshipSteering::maxSpeedRotateRightLeft(float maxSpeedRotateRightLeft) {
  maxSpeedRotateRightLeft_ = maxSpeedRotateRightLeft;
}

void SpaceshipSteering::maxSpeedLoopRightLeft(float maxSpeedLoopRightLeft) {
  maxSpeedLoopRightLeft_ = maxSpeedLoopRightLeft;
}

void SpaceshipSteering::addNode(Ogre::SceneNode *steeringNode) {
  steeringNode_ = steeringNode;
}
