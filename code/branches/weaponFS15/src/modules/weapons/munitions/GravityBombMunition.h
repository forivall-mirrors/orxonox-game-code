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

	class _WeaponsExport GravityBombMunition : public Munition
    {
        public:
			GravityBombMunition(Context* context);
            virtual ~GravityBombMunition() {}
    };

}


#endif /* GRAVITYBOMBMUNITION_H_ */
