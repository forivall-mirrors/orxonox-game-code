/*
 * DodgePlayer.h
 *
 *  Created on: Oct 15, 2014
 *      Author: sriedel
 */

#ifndef DODGEPLAYER_H_
#define DODGEPLAYER_H_

#include "worldentities/pawns/SpaceShip.h"
#include "graphics/Camera.h"

namespace orxonox
{

	class DodgePlayer
	{
		public:
			DodgePlayer();
			virtual ~DodgePlayer();
			virtual void tick(float dt);
	};

} /* namespace orxonox */

#endif /* DODGEPLAYER_H_ */
