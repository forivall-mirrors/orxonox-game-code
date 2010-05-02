/*
===========================================================================
Copyright (C) 2008 Daniel Örstadius

This file is part of bsp-renderer source code.

bsp-renderer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bsp-renderer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with bsp-renderer.  If not, see <http://www.gnu.org/licenses/>.

*/

// BspVertex.cpp -- used in the tessellation of the Bezier patches


#include <math.h>
#include "Q3Map_BspVertex.h"


BspVertex::BspVertex()
{
  for (int i=0; i<3; i++)
    mPosition[i] = 0;

  for (int i=0; i<3; i++)
    mNormal[i] = 0;

  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
      mTexcoord[i][j] = 0;
}

BspVertex::BspVertex(float p[3], float texcoord[2][2], float n[3])
{
  for (int i=0; i<3; i++)
    mPosition[i] = p[i];

  for (int i=0; i<3; i++)
    mNormal[i] = n[i];

  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
    {		
      this->mTexcoord[i][j] = texcoord[i][j];
    }
}

BspVertex BspVertex::operator+(BspVertex a)
{
  BspVertex res;

  for (int i=0; i<3; i++)
  {
    res.mPosition[i] = this->mPosition[i] + a.mPosition[i];
    res.mNormal[i] = this->mNormal[i] + a.mNormal[i];
  }

  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
    {			
      res.mTexcoord[i][j] = this->mTexcoord[i][j] + a.mTexcoord[i][j];
    }

    return res;
}

BspVertex BspVertex::operator*(float a)
{
  BspVertex res;

  for (int i=0; i<3; i++)
  {
    res.mPosition[i] = this->mPosition[i] * a;
    res.mNormal[i] = this->mNormal[i] * a;
  }

  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
      res.mTexcoord[i][j] = this->mTexcoord[i][j] * a;

  return res;
}

void BspVertex::normalise(void)
{
  //QVECTOR n((float)mNormal[0], (float)mNormal[1], (float)mNormal[2]);

  //D3DXVec3Normalize(&n, &n);
  //mNormal[0] = n.x;
  //mNormal[1] = n.y;
  //mNormal[2] = n.z;

	float flLen=sqrt( mNormal[0]*mNormal[0] + mNormal[1]*mNormal[1] + mNormal[2]*mNormal[2] ) ;
	mNormal[0]/=flLen ;
	mNormal[1]/=flLen ;
	mNormal[2]/=flLen ;
}
