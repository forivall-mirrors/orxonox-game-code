//
//  Tower.cc
//  Orxonox
//
//  Created by Fabian Mentzer on 29.04.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

/* Not implemented fully */

//#include "Tower.h"
#include "TowerTurret.h"

#include "core/CoreIncludes.h"
//#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(TowerTurret);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TowerTurret::TowerTurret(Context* context) : StaticEntity(context)
    {
        RegisterObject(TowerTurret);

        this->setCollisionType(WorldEntity::Static);


    }

    void TowerTurret::setOrientation(const Quaternion& orientation)
    {
        static int ori;
        orxout() << "orientation " << ++ori << endl;
    }


}
