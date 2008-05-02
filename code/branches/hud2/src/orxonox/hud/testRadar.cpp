#include <cmath>
//#include <iostream.h>

int main(){

    int dirX, dirY, dirZ;      //flying direction
    int ortX, ortY, ortZ;      //orthogonal direction
    int dX, dY, dZ;            //distance between main ship and the object
    int vecX, vecY, vecZ;      //vector product dir X ort
    double alpha;              //defines the radius in the radar
    double beta;               //defines the angle in the radar
    bool right;                //checks whether the object is on the right side (since cos is not bijective)
    
    dirX = 1;
    dirY = 0;
    dirZ = 0;
    
    ortX = 0;
    ortY = 0;
    ortZ = 1;
    
    dX = 2;
    dY = 0;
    dZ = 0;
    
    alpha = acos(((double)(dirX*dX+dirY*dY+dirZ*dZ))/(sqrt((double)(pow(dX,2)+pow(dY,2)+pow(dZ,2)))+sqrt((double)(pow(dirX,2)+pow(dirY,2)+pow(dirZ,2)))));
    beta = acos(((double)(ortX*dX+ortY*dY+ortZ*dZ))/(sqrt((double)(pow(dX,2)+pow(dY,2)+pow(dZ,2)))+sqrt((double)(pow(ortX,2)+pow(ortY,2)+pow(ortZ,2)))));
    vecX = dirY*ortZ - dirZ*ortY;
    vecY = dirZ*ortX - dirX*ortZ;
    vecZ = dirX*ortY - dirY*ortX;
    
    if((vecX*dX+vecY*dY+vecZ*dZ)>0){right=true;}
    else right=false;
    
//    cout<<alpha;
//    cout<<beta;
    
}

    
    

    


