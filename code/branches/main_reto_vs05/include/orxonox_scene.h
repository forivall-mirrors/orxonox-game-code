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


#ifndef ORXONOX_SCENE_H
#define ORXONOX_SCENE_H

#include "Ogre.h"

using namespace Ogre;

class OrxonoxScene
{
public:
	OrxonoxScene(SceneManager*);

	virtual ~OrxonoxScene();

	virtual bool initialise();

	virtual bool tick(unsigned long, Real);

protected:
	/// method where you can perform resource group loading
	virtual void loadResources();

	/// Define what is in the scene
	virtual void createScene();

protected:
	SceneManager* sceneMgr_;

	//specific variables for test purposes
	Light        *light_;
	SceneNode    *lightNode_;
	BillboardSet *bbs_;
	Real distance_;
  Real radius_;
};

#endif /* ORXONOX_SCENE_H */
