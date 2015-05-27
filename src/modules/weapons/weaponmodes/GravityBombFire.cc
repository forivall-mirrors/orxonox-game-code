/*
 * GravityBombFire.cc
 *
 *  Created on: Apr 16, 2015
 *      Author: Manuel Eggimann
 */
#include "GravityBombFire.h"

#include "core/CoreIncludes.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/GravityBomb.h"

namespace orxonox
{
	RegisterClass(GravityBombFire);

	const float GravityBombFire::BOMB_VELOCITY = 600.0; ///< The velocity of the bomb after launch

	GravityBombFire::GravityBombFire(Context* context) : WeaponMode(context)
	{
		RegisterObject(GravityBombFire);

		this->reloadTime_ = 0.50f; 
		this->bParallelReload_ = false;
		this->damage_ = 20.0f;  ///< The damage of the Bomb if it hits a pawn.

		this->setMunitionName("GravityBombMunition");
		this->setDefaultSoundWithVolume("sounds/Rocket_launch.ogg",0.8);	///< sets sound of the bomb as it is fired.
	}

	GravityBombFire::~GravityBombFire(){};

	void GravityBombFire::fire()
	{
		GravityBomb* bomb = new GravityBomb(this->getContext());
        //Create a new Bomb in 3D-Space and set the right direction speed and orientation.
		this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        bomb->setOrientation(this->getMuzzleOrientation());
        bomb->setPosition(this->getMuzzlePosition());
        bomb->setVelocity(this->getMuzzleDirection() * (this->BOMB_VELOCITY+this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getVelocity().length()));

		//Set the shooter of the bomb so it is possible to determine the pawn that killed another one and define damage to shield and healt the bomb does.
        bomb->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
		bomb->setDamage(this->getDamage());
		bomb->setShieldDamage(this->getShieldDamage());
		bomb->setHealthDamage(this->getHealthDamage());
	}


}


