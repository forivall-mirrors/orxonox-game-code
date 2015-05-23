/*
 * GravityBombMunition.h
 *
 *  Created on: Apr 16, 2015
 *      Author: meggiman
 */

#ifndef GRAVITYBOMBMUNITION_H_
#define GRAVITYBOMBMUNITION_H_

#include "weapons/WeaponsPrereqs.h"
#include "weaponsystem/Munition.h"

namespace orxonox
{
	/**
	 * @class	GravityBombMunition
	 *
	 * @brief	This class is used to set the behaviour of various 
	 * 			munition specific attributes of the GravityBomb like max count of munition per magazine.
	 *
	 * @author	Manuel
	 * @date	23.05.2015
	 */
	class _WeaponsExport GravityBombMunition : public Munition
    {
        public:
			GravityBombMunition(Context* context);
            virtual ~GravityBombMunition() {}
    };

}


#endif /* GRAVITYBOMBMUNITION_H_ */
