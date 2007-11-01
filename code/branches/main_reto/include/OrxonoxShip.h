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


#ifndef __OrxonoxShip__
#define __OrxonoxShip__

#include "Ogre.h"
#include "Bullet.h"

using namespace Ogre;

class OrxonoxShip
{
public:
	Vector3 speed;
	float thrust, sideThrust, baseThrust;
	Vector3 bulletSpeed;

	OrxonoxShip(SceneManager*, SceneNode*);
	virtual ~OrxonoxShip();

	virtual bool initialise();

	void setThrust(const Real);
	void setSideThrust(const Real);
	void setYaw(const Radian);
	void setPitch(const Radian);
	void setRoll(const Radian);

	Real getThrust();

	Bullet* fire();

	bool tick(unsigned long, Real);

	SceneNode *mRootNode;

protected:
	SceneManager *mSceneMgr;
	Entity *mShip;

	int n;

};


#endif
