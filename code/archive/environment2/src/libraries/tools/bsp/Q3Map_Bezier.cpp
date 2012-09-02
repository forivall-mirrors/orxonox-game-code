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

// Bezier.cpp -- tessellates the Bezier patches

/* The tessellation code is taken from the document 
"Rendering Quake 3 Maps" at 
http://graphics.cs.brown.edu/games/quake/quake3.html
The author states that it is based on code from 
Paul Baker's Octagon project,
http://www.paulsprojects.net/opengl/octagon/octagon.html
which is released under the New BSD license.
Please see the licenses folder.
*/

#include "Q3Map_Bezier.h"
#include "Q3Map_misc.h"


Bezier::Bezier()
{	
  mVertex = 0;
  mIndex = 0;
  mTrianglesPerRow = 0;
  mRowIndex = 0;
}

Bezier::~Bezier()
{
  DELETE_ARRAY(mVertex);
  DELETE_ARRAY(mIndex);
  DELETE_ARRAY(mTrianglesPerRow);
  DELETE_ARRAY(mRowIndex);
}





void Bezier::tessellate(int L) 
{
  // The number of vertices along a side is 1 + num edges
  const int L1 = L + 1;

  mVertex = new BspVertex[L1*L1];
  mNumVertex = L1*L1;

  // Compute the vertices    
  for (int i = 0; i <= L; ++i)
  {
    float a = (float)i / L;
    float b = 1.0f - a;

    mVertex[i] =
      mControls[0] * (b * b) + 
      mControls[3] * (2 * b * a) +
      mControls[6] * (a * a);
  }

  for (int i = 1; i <= L; i++) 
  {
    float a = (float)i / L;
    float b = 1.0f - a;

    BspVertex temp[3];

    for (int j = 0; j < 3; j++)
    {
      int k = 3 * j;
      temp[j] =
        mControls[k + 0] * (b * b) + 
        mControls[k + 1] * (2 * b * a) +
        mControls[k + 2] * (a * a);
    }

    for(int j = 0; j <= L; ++j) 
    {
      float a = (float)j / L;
      float b = 1.0f - a;

      mVertex[i * L1 + j]=
        temp[0] * (b * b) + 
        temp[1] * (2 * b * a) +
        temp[2] * (a * a);
    }
  }

  // Compute the indices     
  mIndex = new unsigned int[L * (L + 1) * 2];
  mNumIndex = L * (L + 1) * 2;

  for (int row = 0; row < L; ++row) 
  {
    for(int col = 0; col <= L; ++col)	
    {
      mIndex[(row * (L + 1) + col) * 2 + 1] = row       * L1 + col;
      mIndex[(row * (L + 1) + col) * 2]     = (row + 1) * L1 + col;
    }
  }

  mTrianglesPerRow = new unsigned int[L];
  mRowIndex = new unsigned int[L];

  for (int row = 0; row < L; ++row) 
  {
    mTrianglesPerRow[row] = 2 * L1;
    //rowIndexes[row]      = &indexes[row * 2 * L1];
    mRowIndex[row]      = row * 2 * L1;
  }

  for (int i=0; i < L1*L1; i++)
    mVertex[i].normalise();
}
