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

// BspVertex.h -- used in the tessellation of the Bezier patches

#ifndef _BSPVERTEX_H
#define _BSPVERTEX_H

class BspVertex
{
public:
  BspVertex();
  BspVertex(float p[3], float texcoord[2][2], float n[3]);

  float mPosition[3];
  float mTexcoord[2][2];
  float mNormal[3];

  BspVertex operator+(BspVertex a);
  BspVertex operator*(float a);

  void normalise(void);

};

#endif 
