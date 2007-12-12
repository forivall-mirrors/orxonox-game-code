/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OgreSceneNode.h"
#include "OgreVector3.h"

#include "inertial_node.h"


namespace orxonox {
  using namespace Ogre;

  InertialNode::InertialNode(SceneNode *node, Vector3 speed)
        : node_(node), speed_(speed), childListSize_(4), childListIndex_(0),
        parentNode_(NULL)
  {
    childList_ = new InertialNode*[childListSize_];
  }


  InertialNode::InertialNode(orxonox::InertialNode *parentNode, Vector3 speed)
        : parentNode_(parentNode), childListSize_(4), childListIndex_(0), speed_(speed)
  {
    childList_ = new InertialNode*[childListSize_];
    node_ = parentNode_->getSceneNode()->createChildSceneNode();
  }


  InertialNode::~InertialNode()
  {
    if (childList_)
    {
      for (int i = 0; i < childListIndex_; i++)
        if (childList_[i])
          delete childList_[i];
      delete childList_;
    }
  }


  InertialNode* InertialNode::createChildNode()
  {
    InertialNode *temp = new InertialNode(this);
    addChild(temp);
    return temp;
  }


  InertialNode* InertialNode::getParentNode()
  {
    return parentNode_;
  }


  SceneNode* InertialNode::getSceneNode()
  {
    return node_;
  }


  Vector3 InertialNode::getSpeed()
  {
    return speed_;
  }


  Vector3 InertialNode::getWorldSpeed()
  {
    // go through the tree of Inertial nodes
    Vector3 speed = speed_;
    InertialNode *temp = parentNode_;
    while (temp != NULL)
    {
      speed = temp->getSceneNode()->getOrientation() * speed + temp->getSpeed();
      temp = temp->getParentNode();
    }
    return speed;
  }


  void InertialNode::addSpeed(Vector3 speed)
  {
    speed_ += speed;
  }


  void InertialNode::addChild(orxonox::InertialNode *node)
  {
    if (childListIndex_ == childListSize_)
    {
      // redimension the array
      InertialNode **tempArray = new InertialNode*[2*childListSize_];
      for (int i = 0; i < childListSize_; i++)
        tempArray[i] = childList_[i];
      childListSize_ *= 2;
      delete childList_;
      childList_ = tempArray;
    }
    childList_[childListIndex_++] = node;
  }

}
