/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Test_H__
#define _Test_H__

#include "OrxonoxPrereqs.h"
#include "core/BaseObject.h"
#include "network/Synchronisable.h"

namespace orxonox
{
  class _OrxonoxExport Test: public BaseObject, public Synchronisable
  {
    public:
      Test(BaseObject* creator);
      virtual ~Test();

      void setConfigValues();
      void registerVariables();

      void setV1(unsigned int value){ v1 = value; }
      void setV2(unsigned int value){ v2 = value; }
      void setV3(unsigned int value){ v3 = value; }

      void checkV1();
      void checkV2();
      void checkV3();

    private:
      unsigned int v1;
      unsigned int v2;
      unsigned int v3;
  };
}

#endif /* _Test_H__ */
