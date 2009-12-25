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
#include "network/synchronisable/Synchronisable.h"
#include "tools/interfaces/Tickable.h"


typedef int TYPE;
typedef unsigned int UTYPE;


namespace orxonox
{
  class _OrxonoxExport Test: public BaseObject, public Synchronisable, public Tickable
  {
    public:
      Test(BaseObject* creator);
      virtual ~Test();

      void setConfigValues();
      void registerVariables();

      static void call(unsigned int clientID);
      void call2(unsigned int clientID, std::string s1, std::string s2, std::string s3, std::string s4);
      virtual void tick(float dt);


      //unsigned functions
      void setU1(UTYPE value){ u1 = value; }
      void setU2(UTYPE value){ u2 = value; }
      void setU3(UTYPE value){ u3 = value; }
      void setU4(UTYPE value){ u4 = value; }
      void checkU1();
      void checkU2();
      void checkU3();
      void checkU4();

      //signed functions
      void setS1(TYPE value){ s1 = value; }
      void setS2(TYPE value){ s2 = value; }
      void setS3(TYPE value){ s3 = value; }
      void setS4(TYPE value){ s4 = value; }
      void checkS1();
      void checkS2();
      void checkS3();
      void checkS4();

      void printPointer();

      static void printV1(){ instance_->checkU1(); }
      static void printV2(){ instance_->checkU2(); }
      static void printV3(){ instance_->checkU3(); }
      static void printV4(){ instance_->checkU4(); }

      void printBlaBla(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5);

    private:
      UTYPE u1;
      UTYPE u2;
      UTYPE u3;
      UTYPE u4;

      TYPE s1;
      TYPE s2;
      TYPE s3;
      TYPE s4;

      Test* pointer_;

      static Test* instance_;
  };
}

#endif /* _Test_H__ */
