#ifndef _TDCoordinate_H__
#define _TDCoordinate_H__

#include "TDCoordinate.h"
//#include "towerdefense/TowerDefense.h"
#include "gametypes/Deathmatch.h"
#include "towerdefense/TowerDefensePrereqs.h"
#include "worldentities/pawns/SpaceShip.h"
namespace orxonox
{

//Class to save the Coordinates in a class instead of struct
//Convert 2d coordinates to 3d in order to set waypoints
    class _TowerDefenseExport TDCoordinate : public OrxonoxClass
    {
        public:
            int x;
            int y;

            TDCoordinate();

            Vector3 get3dcoordinate();

            virtual ~TDCoordinate() {};

            TDCoordinate(int x, int y);
    };

}

#endif /* _TDCoordinate_H__ */
