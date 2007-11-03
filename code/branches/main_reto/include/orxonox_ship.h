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


#ifndef ORXONOX_SHIP_H
#define ORXONOX_SHIP_H

#include "Ogre.h"

#include "bullet.h"

using namespace Ogre;

class OrxonoxShip
{
public:
	OrxonoxShip(SceneManager*, SceneNode*);
	virtual ~OrxonoxShip();

	virtual bool initialise();

	void setThrust(const Real);
	void setSideThrust(const Real);
	void turnUpAndDown(const Radian&);
	void turnLeftAndRight(const Radian&);

	Bullet* fire();

	bool tick(unsigned long, Real);

protected:


public:


protected:
	SceneManager *SceneMgr_;
	SceneNode *RootNode_;
	Entity *ShipEntity_;

	Vector3 currentSpeed_;
	float CurrentThrust_;
  float CurrentSideThrust_;
  float baseThrust_;
	int objectCounter_;
	Vector3 bulletSpeed_;

};


#endif /* ORXONOX_SHIP_H */
