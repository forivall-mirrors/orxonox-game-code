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


#ifndef BASE_WEAPON_H
#define BASE_WEAPON_H

#include <OgrePrerequisites.h>

#include "orxonox/OrxonoxPrereqs.h"

#include "../Model.h"


namespace orxonox {
  class BaseWeapon : public Model
  {
  public:
    enum Action {
      NOTHING  = 0,
      RELOAD   = 1,
      CHANGE_AMMO  = 2,
      SPECIAL  = 3
    };

  protected:
    enum State {
      IDLE = 0,
      PRIMARY_FIRE = 1,
      SECONDARY_FIRE = 2,
      RELOADING = 3,
      CHANGING_AMMO = 4,
    };

  public:
    BaseWeapon();
	  virtual ~BaseWeapon();

    bool addAction(const Action);

    void primaryFireRequest();

    void secondaryFireRequest();

    int getAmmoState();

    void setAmmoDump(AmmunitionDump*);

    inline virtual void loadParams(TiXmlElement* xmlElem) { Model::loadParams(xmlElem); };

    virtual void tick(float dt);

  protected:
    virtual void primaryFire() { };

    virtual void primaryFiring(unsigned int) { };

    virtual void secondaryFire() { };

    virtual void secondaryFiring(unsigned int) { };

    inline void registerAllVariables() { }//Model::registerAllVariables(); }

  public:

  protected:
    Ogre::SceneManager *sceneMgr_;

    int bulletCounter_;
    BulletManager *bulletManager_;

    AmmunitionDump *ammoDump_;

    bool primaryFireRequest_;
    bool secondaryFireRequest_;

    float totalTime_;
    unsigned long actionStartTime_;

    State currentState_;
    bool secondaryFired_;

    Action nextAction_;
    bool actionAdded_;
    unsigned long timeSinceNextActionAdded_;
    static const unsigned long nextActionValidityPeriod_ = 500;

    // weapon properties
    int leftAmmo_;
    int primaryFirePower_;
    int secondaryFirePower_;
    int primaryFiringRate_;
    int secondaryFiringRate_;
    Ogre::Real primaryBulletSpeed_;
    Ogre::Real secondaryBulletSpeed_;

    int magazineSize_;

  };
}

#endif /* BASE_WEAPON_H */
