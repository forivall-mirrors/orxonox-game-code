#ifndef SPACESHIP_STEERING_H
#define SPACESHIP_STEERING_H

#include "OgrePrerequisites.h"

class SpaceshipSteering
{
  public:
    SpaceshipSteering(float maxSpeedForward, float maxSpeedRotateUpDown,
    float maxSpeedRotateRightLeft, float maxSpeedLoopRightLeft);
    void tick(float time);
    void moveForward(float moveForward);
    void brake(float brake);
    void rotateUp(float rotateUp);
    void rotateDown(float rotateDown);
    void rotateRight(float rotateRight);
    void rotateLeft(float rotateLeft);
    void loopRight(float loopRight);
    void loopLeft(float loopLeft);
    void maxSpeedForward(float maxSpeedForward);
    void maxSpeedRotateUpDown(float maxSpeedRotateUpDown);
    void maxSpeedRotateRightLeft(float maxSpeedRotateRightLeft);
    void maxSpeedLoopRightLeft(float maxSpeedLoopRightLeft);
    void addNode(Ogre::SceneNode *steeringNode);

  protected:
    Ogre::SceneNode *steeringNode_;

  private:
    float moveForward_;
    float brake_;
    float rotateUp_;
    float rotateDown_;
    float rotateRight_;
    float rotateLeft_;
    float loopRight_;
    float loopLeft_;

    float speedForward_;
    float speedRotateUpDown_;
    float speedRotateRightLeft_;
    float speedLoopRightLeft_;

    float maxSpeedForward_;
    float maxSpeedRotateUpDown_;
    float maxSpeedRotateRightLeft_;
    float maxSpeedLoopRightLeft_;

    float accelerationForward_;
    float accelerationRotateUpDown_;
    float accelerationRotateRightLeft_;
    float accelerationLoopRightLeft_;

};

#endif
