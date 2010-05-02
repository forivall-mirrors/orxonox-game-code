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

// Bezier.h -- class for the Bezier patches

#ifndef _BEZIER_H
#define _BEZIER_H

#include "Q3Map_BspVertex.h"

class Bezier 
{
public:
  Bezier();
  ~Bezier();
  
  void tessellate(int level);

  BspVertex mControls[9];
  BspVertex *mVertex;
  unsigned int *mIndex;
  unsigned int *mTrianglesPerRow;
  unsigned int *mRowIndex;
  unsigned int mNumIndex;
  unsigned int mNumVertex;
  int mBaseBufferindex;
  int mBaseVertexIndex;
};

#endif
