/*
 * GravityBombFire.cc
 *
 *  Created on: Apr 16, 2015
 *      Author: meggiman
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

	const float GravityBombFire::BOMB_VELOCITY = 400.0;

	GravityBombFire::GravityBombFire(Context* context) : WeaponMode(context)
	{
		RegisterObject(GravityBombFire);

		this->reloadTime_ = 0.50f;
		this->bParallelReload_ = false;
		this->damage_ = 0.0f;
		this->speed_ = BOMB_VELOCITY;

		this->setMunitionName("GravityBombMunition");
		// The firing sound of the Rocket is played in Rocket.cc (because of OpenAl sound positioning)
	}

	GravityBombFire::~GravityBombFire(){};

	void GravityBombFire::fire()
	{
		GravityBomb* bomb = new GravityBomb(this->getContext());
        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        bomb->setOrientation(this->getMuzzleOrientation());
        bomb->setPosition(this->getMuzzlePosition());
        bomb->setVelocity(this->getMuzzleDirection() * (this->speed_+this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getVelocity().length()));

        bomb->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
		bomb->setDamage(this->getDamage());
		bomb->setShieldDamage(this->getShieldDamage());
		bomb->setHealthDamage(this->getHealthDamage());
	}


}


