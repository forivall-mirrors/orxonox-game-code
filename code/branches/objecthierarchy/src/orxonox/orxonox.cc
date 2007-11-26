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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  orxonox.cc
 @brief Orxonox Main File
 */

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

#include <string>
#include <iostream>

#include "../xml/xmlParser.h"
#include "../loader/LevelLoader.h"

#include "core/IdentifierIncludes.h"
#include "objects/BaseObject.h"
#include "objects/Test.h"
#include "objects/test1.h"
#include "objects/test2.h"
#include "objects/test3.h"


// some tests to see if enet works without includsion
//#include <enet/enet.h>
//#include <enet/protocol.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
  char path[1024];
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  assert(mainBundle);

  CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
  assert(mainBundleURL);

  CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
  assert(cfStringRef);

  CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

  CFRelease(mainBundleURL);
  CFRelease(cfStringRef);

  return std::string(path);
}
#endif

namespace orxonox
{
  class OrxExitListener : public Ogre::FrameListener
  {
    public:
      OrxExitListener(OIS::Keyboard *keyboard)
    : mKeyboard(keyboard)
      {
      }

      bool frameStarted(const Ogre::FrameEvent& evt)
      {
        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
      }

    private:
      OIS::Keyboard *mKeyboard;
  };

  class OrxApplication
  {
    public:
      void go()
      {
/*
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        createScene();
        setupScene();
        setupInputSystem();
        setupCEGUI();
        createFrameListener();
        startRenderLoop();
*/

        #define testandcout(code) \
          std::cout << #code << " " << code << "\n"

/*
        std::cout << "Test 1\n";
        BaseObject* test1;
        test1 = new BaseObject();
        test1 = new BaseObject();
        test1 = new BaseObject();

        std::cout << "Test 2\n";
        A1* test2;
        test2 = new A1();
        test2 = new A1();
        test2 = new A1();

        std::cout << "Test 3\n";
        BaseObject* test3;
        test3 = new BaseObject();
        test3 = new BaseObject();
        test3 = new BaseObject();

        std::cout << "Test 4\n";
        A3* test4;
        test4 = new A3();
        test4 = new A3();
        test4 = new A3();
*/
/*
        std::cout << "Test 5\n";
        A1* test5_01 = new A1();
        A2* test5_02 = new A2();
        A3* test5_03 = new A3();
        A1B1* test5_04 = new A1B1();
        A1B2* test5_05 = new A1B2();
        A2B1* test5_06 = new A2B1();
        A2B2* test5_07 = new A2B2();
        A3B1* test5_08 = new A3B1();
        A3B2* test5_09 = new A3B2();
        A1B1C1* test5_10 = new A1B1C1();
        A1B1C2* test5_11 = new A1B1C2();
        A1B2C1* test5_12 = new A1B2C1();
        A2B1C1* test5_13 = new A2B1C1();
        A2B2C1* test5_14 = new A2B2C1();
        A3B1C1* test5_15 = new A3B1C1();
        A3B1C2* test5_16 = new A3B1C2();
        A3B2C1* test5_17 = new A3B2C1();
        A3B2C2* test5_18 = new A3B2C2();

        OrxonoxClass* test5;
        for (int i = 0; i <= 18; i++)
        {
          if (i == 0) test5 = new BaseObject;
          if (i == 1) test5 = test5_01;
          if (i == 2) test5 = test5_02;
          if (i == 3) test5 = test5_03;
          if (i == 4) test5 = test5_04;
          if (i == 5) test5 = test5_05;
          if (i == 6) test5 = test5_06;
          if (i == 7) test5 = test5_07;
          if (i == 8) test5 = test5_08;
          if (i == 9) test5 = test5_09;
          if (i == 10) test5 = test5_10;
          if (i == 11) test5 = test5_11;
          if (i == 12) test5 = test5_12;
          if (i == 13) test5 = test5_13;
          if (i == 14) test5 = test5_14;
          if (i == 15) test5 = test5_15;
          if (i == 16) test5 = test5_16;
          if (i == 17) test5 = test5_17;
          if (i == 18) test5 = test5_18;

          std::cout << "\n";
          std::cout << test5->getIdentifier()->getName() << ": parents:     " << test5->getIdentifier()->getParents().toString() << "\n";
          std::cout << test5->getIdentifier()->getName() << ": children:    " << test5->getIdentifier()->getChildren().toString() << "\n";
        }
*/
/*
        std::cout << "\n";
        std::cout << "isA(XYZ)-Test:\n";
        std::cout << "test5_01 = A1, Erwartet: 1 0 0 1 0\n";
        testandcout(test5_01->isA(Class(A1)));
        testandcout(test5_01->isA(Class(A2)));
        testandcout(test5_01->isA(Class(A1B1)));
        testandcout(test5_01->isA(Class(BaseObject)));
        testandcout(test5_01->isA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_02 = A2, Erwartet: 0 1 0 1 0\n";
        testandcout(test5_02->isA(Class(A1)));
        testandcout(test5_02->isA(Class(A2)));
        testandcout(test5_02->isA(Class(A1B1)));
        testandcout(test5_02->isA(Class(BaseObject)));
        testandcout(test5_02->isA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_01 = A3, Erwartet: 0 0 0 1 1\n";
        testandcout(test5_03->isA(Class(A1)));
        testandcout(test5_03->isA(Class(A2)));
        testandcout(test5_03->isA(Class(A1B1)));
        testandcout(test5_03->isA(Class(BaseObject)));
        testandcout(test5_03->isA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "isDirectA(XYZ)-Test:\n";
        std::cout << "test5_01 = A1, Erwartet: 1 0 0 0 0\n";
        testandcout(test5_01->isDirectlyA(Class(A1)));
        testandcout(test5_01->isDirectlyA(Class(A2)));
        testandcout(test5_01->isDirectlyA(Class(A1B1)));
        testandcout(test5_01->isDirectlyA(Class(BaseObject)));
        testandcout(test5_01->isDirectlyA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_02 = A2, Erwartet: 0 1 0 0 0\n";
        testandcout(test5_02->isDirectlyA(Class(A1)));
        testandcout(test5_02->isDirectlyA(Class(A2)));
        testandcout(test5_02->isDirectlyA(Class(A1B1)));
        testandcout(test5_02->isDirectlyA(Class(BaseObject)));
        testandcout(test5_02->isDirectlyA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "test5_03 = A3, Erwartet: 0 0 0 0 0\n";
        testandcout(test5_03->isDirectlyA(Class(A1)));
        testandcout(test5_03->isDirectlyA(Class(A2)));
        testandcout(test5_03->isDirectlyA(Class(A1B1)));
        testandcout(test5_03->isDirectlyA(Class(BaseObject)));
        testandcout(test5_03->isDirectlyA(Class(Interface1)));

        std::cout << "\n";
        std::cout << "isChildOf(XYZ)-Test:\n";
        std::cout << "test5_04 = A1B1, Erwartet: 1 0 1 0 0 0 0\n";
        testandcout(test5_04->isChildOf(Class(A1)));
        testandcout(test5_04->isChildOf(Class(A2)));
        testandcout(test5_04->isChildOf(Class(BaseObject)));
        testandcout(test5_04->isChildOf(Class(Interface1)));
        testandcout(test5_04->isChildOf(Class(Interface2)));
        testandcout(test5_04->isChildOf(Class(A1B1C2)));
        testandcout(test5_04->isChildOf(Class(A1B1)));

        std::cout << "\n";
        std::cout << "test5_06 = A2B1, Erwartet: 0 1 1 0 0 0 0\n";
        testandcout(test5_06->isChildOf(Class(A1)));
        testandcout(test5_06->isChildOf(Class(A2)));
        testandcout(test5_06->isChildOf(Class(BaseObject)));
        testandcout(test5_06->isChildOf(Class(Interface1)));
        testandcout(test5_06->isChildOf(Class(Interface2)));
        testandcout(test5_06->isChildOf(Class(A1B1C2)));
        testandcout(test5_06->isChildOf(Class(A1B1)));

        std::cout << "\n";
        std::cout << "test5_07 = A2B2, Erwartet: 0 1 1 1 0 0\n";
        testandcout(test5_07->isChildOf(Class(A1)));
        testandcout(test5_07->isChildOf(Class(A2)));
        testandcout(test5_07->isChildOf(Class(BaseObject)));
        testandcout(test5_07->isChildOf(Class(Interface1)));
        testandcout(test5_07->isChildOf(Class(Interface2)));
        testandcout(test5_07->isChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "test5_08 = A3B1, Erwartet: 0 0 1 1 0 0\n";
        testandcout(test5_08->isChildOf(Class(A1)));
        testandcout(test5_08->isChildOf(Class(A2)));
        testandcout(test5_08->isChildOf(Class(BaseObject)));
        testandcout(test5_08->isChildOf(Class(Interface1)));
        testandcout(test5_08->isChildOf(Class(Interface2)));
        testandcout(test5_08->isChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "test5_09 = A3B2, Erwartet: 0 0 1 1 1 0\n";
        testandcout(test5_09->isChildOf(Class(A1)));
        testandcout(test5_09->isChildOf(Class(A2)));
        testandcout(test5_09->isChildOf(Class(BaseObject)));
        testandcout(test5_09->isChildOf(Class(Interface1)));
        testandcout(test5_09->isChildOf(Class(Interface2)));
        testandcout(test5_09->isChildOf(Class(A1B1C2)));

        std::cout << "\n";
        std::cout << "isParentOf(XYZ)-Test:\n";
        std::cout << "test1 = BaseObject, Erwartet: 0 0 1 1 1 1 1\n";
        testandcout(test1->isParentOf(Class(BaseObject)));
        testandcout(test1->isParentOf(Class(Interface1)));
        testandcout(test1->isParentOf(Class(A1)));
        testandcout(test1->isParentOf(Class(A2)));
        testandcout(test1->isParentOf(Class(A1B1)));
        testandcout(test1->isParentOf(Class(A2B2)));
        testandcout(test1->isParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "test5_01 = A1, Erwartet: 0 0 0 0 1 0 0\n";
        testandcout(test5_01->isParentOf(Class(BaseObject)));
        testandcout(test5_01->isParentOf(Class(Interface1)));
        testandcout(test5_01->isParentOf(Class(A1)));
        testandcout(test5_01->isParentOf(Class(A2)));
        testandcout(test5_01->isParentOf(Class(A1B1)));
        testandcout(test5_01->isParentOf(Class(A2B2)));
        testandcout(test5_01->isParentOf(Class(A3B1C2)));

        std::cout << "\n";
        std::cout << "Interface1, Erwartet: 0 0 0 0 0 1 1\n";
        testandcout(Class(Interface1)->isParentOf(Class(BaseObject)));
        testandcout(Class(Interface1)->isParentOf(Class(Interface1)));
        testandcout(Class(Interface1)->isParentOf(Class(A1)));
        testandcout(Class(Interface1)->isParentOf(Class(A2)));
        testandcout(Class(Interface1)->isParentOf(Class(A1B1)));
        testandcout(Class(Interface1)->isParentOf(Class(A2B2)));
        testandcout(Class(Interface1)->isParentOf(Class(A3B1C2)));
*/
/*
        std::cout << "Test 6\n";
        std::cout << "1:\n";
        Identifier* test6_01 = Class(A1B1);
        std::cout << "2:\n";
        Identifier* test6_02 = Class(A1B1);
        std::cout << "3:\n";
        Identifier* test6_03 = Class(A1);
        std::cout << "4:\n";
        Identifier* test6_04 = Class(A1B1C1);
        std::cout << "5:\n";
        Identifier* test6_05 = Class(A1B1);
        std::cout << "6:\n";
        Identifier* test6_06 = Class(A1B1C1);

        std::cout << "\n";
        std::cout << "BaseObject: parents:     " << Class(BaseObject)->getParents().toString() << "\n";
        std::cout << "BaseObject: children:    " << Class(BaseObject)->getChildren().toString() << "\n";

        std::cout << "\n";
        std::cout << "A1: parents:     " << Class(A1)->getParents().toString() << "\n";
        std::cout << "A1: children:    " << Class(A1)->getChildren().toString() << "\n";

        std::cout << "\n";
        std::cout << "A1B1: parents:     " << Class(A1B1)->getParents().toString() << "\n";
        std::cout << "A1B1: children:    " << Class(A1B1)->getChildren().toString() << "\n";

        std::cout << "\n";
        std::cout << "A1B1C1: parents:     " << Class(A1B1C1)->getParents().toString() << "\n";
        std::cout << "A1B1C1: children:    " << Class(A1B1C1)->getChildren().toString() << "\n";

        std::cout << "\n";
        std::cout << "A3B1C1 child of A3:  " << Class(A3B1C1)->isChildOf(Class(A3)) << "\n";
        std::cout << "\n";
        std::cout << "A2 parent of A2B1C1: " << Class(A2)->isParentOf(Class(A2B1C1)) << "\n";
*/
/*
        std::cout << "Test 7\n";
        std::cout << "1\n";
        SubclassIdentifier<A1B1> test7_01;
        test7_01 = Class(A1B1C1);

        SubclassIdentifier<A1B1> test7_02;
        test7_02 = Class(A1B1);

        std::cout << test7_01->getName() << "\n";
        std::cout << test7_02->getName() << "\n";
*/
/*
        std::cout << "2\n";

        SubclassIdentifier<A1B1> test7_03;
        test7_03 = Class(A1);

        SubclassIdentifier<A1B1> test7_04;
        test7_04 = Class(A1B2);

        SubclassIdentifier<A1B1> test7_05;
        test7_05 = Class(A2);
*/
/*
        std::cout << "Test 8\n";

        std::cout << "1\n";
        Test1* test8_01 = new Test1;
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


        std::cout << "Test 9\n";
        std::cout << "1\n";
        Identifier* test9_01 = Class(A3);
        BaseObject* test9_02 = test9_01->fabricate();
        std::cout << test9_02->getIdentifier()->getName() << "\n";

        std::cout << "\n2\n";
        BaseObject* test9_03 = Class(A1B2)->fabricate();
        std::cout << test9_03->getIdentifier()->getName() << "\n";

        std::cout << "\n3\n";
        SubclassIdentifier<A1> test9_04;
        test9_04 = Class(A1B1C1);
        A1* test9_05 = test9_04.fabricate();
        std::cout << test9_05->getIdentifier()->getName() << "\n";

        std::cout << "\n4\n";
        BaseObject* test9_06 = ID("A2B2")->fabricate();
        std::cout << test9_06->getIdentifier()->getName() << "\n";

        std::cout << "\n5\n";
        delete test9_02;
        delete test9_03;
        delete test9_05;
        delete test9_06;
*/

        std::cout << "Test 10\n";
        Identifier* test10_01 = Class(A1B2);
        Identifier* test10_02 = Class(A2);
        Identifier* test10_03 = Class(A3B1C1);

        BaseObject* test10_04 = test10_01->fabricate();
        BaseObject* test10_05 = test10_02->fabricate();
        BaseObject* test10_06 = test10_03->fabricate();

        BaseObject* test10_07;
        for (int i = 0; i < 10; i++)
            test10_07 = ID("A1B1C1")->fabricate();

        std::cout << "1\n";
        int count;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "2\n";
        BaseObject* test10_08;
        BaseObject* test10_09;
        BaseObject* test10_10;
        for (int i = 0; i < 10; i++)
        {
            test10_08 = ID("A2B1C1")->fabricate();
            test10_08->name_ = "A2B1C1#";
            test10_08->name_ += ('0' + i);

            if (i == 0)
                test10_09 = test10_08;

            if (i == 5)
                test10_10 = test10_08;
        }

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "3\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "4\n";
        delete test10_08;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "5\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "6\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "7\n";
        delete test10_09;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::end(); it != 0; --it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::end(); it != 0; --it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::end(); it; --it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::end(); it; --it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::end(); it; --it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "8\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "9\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "10\n";
        delete test10_10;

        count = 0; for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl BaseObjects: " << count << "\n";
        count = 0; for (Iterator<A1> it = ObjectList<A1>::start(); it != 0; ++it) { count++; }
        std::cout << "Anzahl A1: " << count << "\n";
        count = 0; for (Iterator<A1B1> it = ObjectList<A1B1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1: " << count << "\n";
        count = 0; for (Iterator<A1B1C1> it = ObjectList<A1B1C1>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A1B1C1: " << count << "\n";
        count = 0; for (Iterator<A2> it = ObjectList<A2>::start(); it; ++it) { count++; }
        std::cout << "Anzahl A2: " << count << "\n";

        std::cout << "11\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::start(); it; ++it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "12\n";
        for (Iterator<A2B1C1> it = ObjectList<A2B1C1>::end(); it; --it)
            std::cout << "Name: " << it->name_ << "\n";

        std::cout << "13\n";

      }

      ~OrxApplication()
      {
        mInputManager->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputManager);

//        delete mRenderer;
//        delete mSystem;

        delete mListener;
        delete mRoot;
      }

    private:
      Ogre::Root *mRoot;
      OIS::Keyboard *mKeyboard;
      OIS::Mouse *mMouse;
      OIS::InputManager *mInputManager;
      CEGUI::OgreCEGUIRenderer *mRenderer;
      CEGUI::System *mSystem;
      OrxExitListener *mListener;

      void createRoot()
      {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        mRoot = new Ogre::Root(macBundlePath() + "/Contents/Resources/plugins.cfg");
#else
        mRoot = new Ogre::Root();
#endif
      }

      void defineResources()
      {
        Ogre::String secName, typeName, archName;
        Ogre::ConfigFile cf;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
#else
        cf.load("resources.cfg");
#endif

        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
          secName = seci.peekNextKey();
          Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
          Ogre::ConfigFile::SettingsMultiMap::iterator i;
          for (i = settings->begin(); i != settings->end(); ++i)
          {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation( String(macBundlePath() + "/" + archName), typeName, secName);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
#endif
          }
        }
      }

      void setupRenderSystem()
      {
        if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
          throw Ogre::Exception(52, "User canceled the config dialog!", "OrxApplication::setupRenderSystem()");
      }

      void createRenderWindow()
      {
        mRoot->initialise(true, "Ogre Render Window");
      }

      void initializeResourceGroups()
      {
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
      }

      void createScene(void)
      {

        string levelFile = "sp_level_moonstation.oxw";
        loader::LevelLoader* loader = new loader::LevelLoader(levelFile);
      }

      void setupScene()
      {
        Ogre::SceneManager *mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
        Ogre::Camera *cam = mgr->createCamera("Camera");
        Ogre::Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
      }

      void setupInputSystem()
      {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;
        Ogre::RenderWindow *win = mRoot->getAutoCreatedWindow();

        win->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);

        try
        {
          mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
          mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
        }
        catch (const OIS::Exception &e)
        {
          throw new Ogre::Exception(42, e.eText, "OrxApplication::setupInputSystem");
        }
      }

      void setupCEGUI()
      {
        Ogre::SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
        Ogre::RenderWindow *win = mRoot->getAutoCreatedWindow();

        // CEGUI setup
//        mRenderer = new CEGUI::OgreCEGUIRenderer(win, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mgr);
//        mSystem = new CEGUI::System(mRenderer);

        // Other CEGUI setup here.
      }

      void createFrameListener()
      {
        mListener = new OrxExitListener(mKeyboard);
        mRoot->addFrameListener(mListener);
      }

      void startRenderLoop()
      {
        mRoot->startRendering();
      }
  };
}

using namespace Ogre;

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

             INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
             int main(int argc, char **argv)
#endif
{
  try
  {
    orxonox::OrxApplication orxonox;
    orxonox.go();
  }
  catch(Exception& e)
  {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
  }

  return 0;
}

