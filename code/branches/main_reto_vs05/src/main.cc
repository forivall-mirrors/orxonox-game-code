/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

//#define WITH_HIERARCHY

#include "OgrePlatform.h"
#include "OgreException.h"

#include "orxonox.h"

#ifdef WITH_HIERARCHY
#include "class_hierarchy/BaseObject.h"
#include "class_hierarchy/Test.h"
#include "class_hierarchy/test1.h"
#include "class_hierarchy/test2.h"
#include "class_hierarchy/test3.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
#define WIN32_LEAN_AND_MEAN 
#include "windows.h" 
  INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else 
  int main(int argc, char **argv) 
#endif 
  {
#ifndef WITH_HIERARCHY
    try {
      // create an orxonox application and run it
      orxonox::Orxonox myApp;

      myApp.go();
    }
    catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
      MessageBoxA(NULL, e.getFullDescription().c_str(),
            "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "Exception:\n";
      std::cerr << e.getFullDescription().c_str() << "\n";
#endif
      return 1;
    }

#else
#define testandcout(code) \
    std::cout << #code << " " << code << "\n"

    using namespace orxonox;

    std::cout << "Test 8\n";

    std::cout << "1\n";
    Test1* test8_01 = new Test1;
    Test1* asdf = new Test1;
    Test3* test8_03 = new Test3;
    test8_03->usefullClassesIsATest(test8_01);

    std::cout << "2\n";
    Test2* test8_02 = new Test2;
    test8_03->usefullClassesIsATest(test8_02);

    std::cout << "3\n";
    test8_01->setUsefullClass1(Class(Test1));
    test8_01->setUsefullClass1(test8_02->getIdentifier());
    test8_01->setUsefullClass2(Class(Test2));
    test8_01->setUsefullClassOfTypeTest3(Class(Test3));
    test8_01->setUsefullClassOfTypeTest3(test8_03->getIdentifier());


    testandcout(test8_01->isA(Class(Test1)));
    testandcout(test8_01->isA(Class(Test2)));
    testandcout(test8_01->isA(Class(Test3)));

    Test2* test8_04 = new Test2;
    testandcout(test8_02->isA(Class(Test1)));
    testandcout(test8_02->isA(Class(Test2)));
    testandcout(test8_02->isA(Class(Test3)));

    Test3* test8_05 = new Test3;
    testandcout(test8_03->isA(Class(Test1)));
    testandcout(test8_03->isA(Class(Test2)));
    testandcout(test8_03->isA(Class(Test3)));

    delete test8_01;
    delete test8_02;
    delete test8_03;

#endif
    return 0;
  }

#ifdef __cplusplus
}
#endif
