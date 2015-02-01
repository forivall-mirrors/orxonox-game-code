#include "TDCoordinate.h"

#include "towerdefense/TowerDefensePrereqs.h"




namespace orxonox
{
    //RegisterClass(TDCoordinate);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    TDCoordinate::TDCoordinate()
    {
        //RegisterObject(TDCoordinate);
        x=0;
        y=0;

    }

    TDCoordinate::TDCoordinate(int x, int y)
    {
        this->x=x;
        this->y=y;
    }


    Vector3 TDCoordinate::get3dcoordinate()
    {
        float tileScale = 100;

        Vector3 *coord = new Vector3();
        coord->x= (x-8) * tileScale;
        coord->y= (y-8) * tileScale;
        coord->z=100;

        return *coord;
    }
}
