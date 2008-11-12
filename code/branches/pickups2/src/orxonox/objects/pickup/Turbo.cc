#include "core/Executor.h"

namespace orxonox
{

	bool pickedUp(Pawn* player)
	{
		if(player-> isA(playerBaseClass))
			{
			Spaceship* ship = dynamic_cast <SpaceShip*>(player);
			setSpeedBoost(this->boost, this->duration, Spaceship* ship)
			return true;
			}
		return false;

	}
	
	
	void unsetSpeedBoost(int boost, Spaceship* ship)
	{
	ship->setMaxSpeed( ship->getMaxSpeed() - boost);
	ship->setTransAcc( ship->getTransAcc()/ACCBOOST);
	}

	void setSpeedBoost(float boost, float duration, Spaceship* ship)
	{
	const float ACCBOOST =1.5
	ship->setMaxSpeed( ship->getMaxSpeed() + boost);
	ship->setTransAcc( ship->getTransAcc()*ACCBOOST);

	turbotimer_.setTimer(duration, false, this, createExecutor(createFunctor(&Turbo::unsetSpeedBoost)))
	
	}
	
	
}
	
	
	
	
	
