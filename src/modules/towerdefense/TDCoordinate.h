#ifndef _TDCoordinate_H__
#define _TDCoordinate_H__

#include "core/CoreIncludes.h"
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
            TDCoordinate();
            TDCoordinate(int x, int y);
            virtual ~TDCoordinate() {};
            virtual void Set(int x, int y);
            virtual int GetX();
            virtual int GetY();
            virtual Vector3 get3dcoordinate(); 

        private:
            int _x;
            int _y;
    };

}

#endif /* _TDCoordinate_H__ */
