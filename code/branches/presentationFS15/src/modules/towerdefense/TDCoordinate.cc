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
        Set(0,0);

    }

    TDCoordinate::TDCoordinate(int x, int y)
    {        
        Set(x,y);
    }

    void TDCoordinate::Set(int x, int y)
    {        
        if (x < 0)
        {
            _x = 0;
        }
        else if (x > 15)
        {
            _x = 15;
        }
        else
        {
            _x = x;
        }

        if (y < 0)
        {
            _y = 0;
        }
        else if (y > 15)
        {
            _y = 15;
        }
        else
        {
            _y = y;
        }
    }

    int TDCoordinate::GetX()
    {        
        return _x;
    }

    int TDCoordinate::GetY()
    {        
        return _y;
    }


    Vector3 TDCoordinate::get3dcoordinate()
    {
        float tileScale = 100;

        Vector3 *coord = new Vector3();
        coord->x= (_x-8) * tileScale;
        coord->y= (_y-8) * tileScale;
        coord->z=100;

        return *coord;
    }
}
