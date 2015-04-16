/*
 * GravityBombMunition.cc
 *
 *  Created on: Apr 16, 2015
 *      Author: meggiman
 */
#include "GravityBombMunition.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
RegisterClass(GravityBombMunition);

	GravityBombMunition::GravityBombMunition(Context* context) : Munition(context)
	{
		RegisterObject(GravityBombMunition);
		this->maxMunitionPerMagazine_ = 1;
		this->maxMagazines_ = 30;
		this->magazines_ = 10;

		this->bUseSeparateMagazines_ = false;
		this->bStackMunition_ = true;

		this->bAllowMunitionRefilling_ = true;
		this->bAllowMultiMunitionRemovementUnderflow_ = false;
	}

}




