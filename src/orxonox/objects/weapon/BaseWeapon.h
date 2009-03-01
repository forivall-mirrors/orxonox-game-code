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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#ifndef _BaseWeapon_H__
#define _BaseWeapon_H__

#include <OgrePrerequisites.h>

#include "../../OrxonoxPrereqs.h"

#include "../Model.h"


namespace orxonox {
  class _OrxonoxExport BaseWeapon : public Model
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
    virtual void primaryFire() = 0;

    virtual void primaryFiring(float) = 0;

    virtual void secondaryFire() = 0;

    virtual void secondaryFiring(float) = 0;

    virtual void registerAllVariables() { };

  public:

  protected:
    Ogre::SceneManager *sceneMgr_;

    int bulletCounter_;
    BulletManager *bulletManager_;

    AmmunitionDump *ammoDump_;

    bool primaryFireRequest_;
    bool secondaryFireRequest_;

    float totalTime_;
    float actionStartTime_;

    State currentState_;
    bool secondaryFired_;

    Action nextAction_;
    bool actionAdded_;
    float timeSinceNextActionAdded_;
    static float nextActionValidityPeriod_s;

    // weapon properties
    int leftAmmo_;
    float primaryFirePower_;
    float secondaryFirePower_;
    float primaryFiringRate_;
    float secondaryFiringRate_;
    Real primaryBulletSpeed_;
    Real secondaryBulletSpeed_;

    int magazineSize_;

  };
  ExportAbstractClass(BaseWeapon, Orxonox);
}

#endif /* _BaseWeapon_H__ */
