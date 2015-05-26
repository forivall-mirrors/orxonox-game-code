/*
* GravityBombFire.h
*
*  Created on: Apr 16, 2015
*      Author: Manuel Eggimann
*/

/**
    @file GravityBombFire.h
    @brief Definition of the GravityBombFire class.
*/

#ifndef _GravityBombFire_H__
#define _GravityBombFire_H__

#include "weapons/WeaponsPrereqs.h"
#include "weaponsystem/WeaponMode.h"

namespace orxonox
{

    /**
    *@brief
    *   Fires the GravityBomb. This class implements everything needed to fire the BasicProjectile GravityBomb.
    *   Everything that has to do with the bombs behaviour after launching it is implemented in GravityBomb and GravityBombField.
    *@author
    *    Manuel Eggimann
    *@ingroup WeaponsWeaponModes
    */
    class _WeaponsExport GravityBombFire : public WeaponMode
    {
        public:
            GravityBombFire(Context* context);
            virtual ~GravityBombFire();

            virtual void fire();

        private:
            static const float BOMB_VELOCITY; //!< The initial speed of the bomb when it is launched.
    };
}

#endif /* _GravityBombFire_H__ */
