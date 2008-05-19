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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
 @file
 @brief Different definitions of input processing.
 */

#ifndef _InputHandler_H__
#define _InputHandler_H__

#include "CorePrereqs.h"

#include <string>

#include "ois/OIS.h"
#include "util/Math.h"
#include "OrxonoxClass.h"
#include "CommandExecutor.h"
#include "InputInterfaces.h"

namespace orxonox
{
  struct _CoreExport AxisCommand
  {
    std::string commandStr;
    CommandEvaluation evaluation;
    bool bRelative;
    float value;
    unsigned int nValuesAdded;
  };

  struct _CoreExport SimpleCommand
  {
    // for simple binding; direct
    std::string commandStr;
    CommandEvaluation evaluation;

    // axis binding; buffered
    AxisCommand* axisCommand;
    float axisModifier;
  };

  struct _CoreExport KeyBinding
  {
    void clear() { commands = 0; nCommands = 0; }
    SimpleCommand* commands;
    unsigned int nCommands;
  };

  struct _CoreExport KeyBindingBundle
  {
    KeyBinding OnPress;
    KeyBinding OnRelease;
    KeyBinding OnHold;
  };

  struct _CoreExport HalfAxis
  {
    float rel;
    float abs;
    float threshold;
    bool wasDown;
    bool hasChanged;
  };


  /**
    @brief Captures mouse, keyboard and joy stick input while in the actual game mode.
           Manages the key bindings.
  */
  class _CoreExport KeyBinder : public KeyHandler, public MouseHandler, public JoyStickHandler, public OrxonoxClass
  {
  public:
    KeyBinder ();
    ~KeyBinder();

    bool loadBindings();
    void clearBindings(bool bInit = false);

    void setConfigValues();

  private: // functions
    bool readBindings(std::string* names, std::string* bindingStrings, KeyBindingBundle* bindings, unsigned int size);
    bool executeBinding(KeyBinding& binding, float axisRel, float axisAbs);
    void clearBundle(KeyBindingBundle& bundle, bool bInit);

    void tick(float dt);

    bool keyPressed (const KeyEvent& evt);
    bool keyReleased(const KeyEvent& evt);
    bool keyHeld    (const KeyEvent& evt);

    bool mouseButtonPressed (MouseButton::Enum id);
    bool mouseButtonReleased(MouseButton::Enum id);
    bool mouseButtonHeld    (MouseButton::Enum id);
    bool mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
    bool mouseScrolled      (int abs, int rel);

    bool joyStickButtonPressed (int joyStickID, int button);
    bool joyStickButtonReleased(int joyStickID, int button);
    bool joyStickButtonHeld    (int joyStickID, int button);
    bool joyStickAxisMoved     (int joyStickID, int axis, int value)  ;

  private: // variables
    //! denotes the number of different keys there are in OIS.
    static const unsigned int nKeys_s = 0xEE;
    //! Actual key bindings as bundle for Press, Hold and Release
    KeyBindingBundle bindingsKeys_ [nKeys_s];
    //! Names of the keys as strings
    std::string namesKeys_         [nKeys_s];
    //! The configured string values
    std::string bindingStringsKeys_[nKeys_s];

    //! denotes the number of different mouse buttons there are in OIS.
    static const unsigned int nMouseButtons_s = 8;
    //! Actual key bindings as bundle for Press, Hold and Release
    KeyBindingBundle bindingsMouseButtons_ [nMouseButtons_s];
    //! Names of the mouse buttons as strings
    std::string namesMouseButtons_         [nMouseButtons_s];
    //! The configured string values
    std::string bindingStringsMouseButtons_[nMouseButtons_s];

    //! denotes the number of different joy stick buttons there are in OIS.
    static const unsigned int nJoyStickButtons_s = 32 + 4 * 4; // 32 buttons and 4 POVs with 4 buttons
    //! Actual key bindings as bundle for Press, Hold and Release
    KeyBindingBundle bindingsJoyStickButtons_ [nJoyStickButtons_s];
    //! Names of the joy stick buttons as strings
    std::string namesJoyStickButtons_         [nJoyStickButtons_s];
    //! The configured string values
    std::string bindingStringsJoyStickButtons_[nJoyStickButtons_s];

    //! denotes the number of half axes (every axis twice) there can be.
    static const unsigned int nHalfAxes_s = 56;
    /**
    * Array with all the half axes for mouse and joy sticks.
    * Bear in mind that the positions are fixed and that the first entry is the
    * positive one and the second is negative.
    * Sequence is as follows:
    *  0 -  3: Mouse x and y
    *  4 -  7: Mouse scroll wheels 1 and 2 (2 not yet supported)
    *  8 - 23: joy stick (slider) axes 1 to 8
    * 24 - 55: joy stick axes 1 - 16
    */
    HalfAxis halfAxes_[nHalfAxes_s];
    //! Actual key bindings as bundle for Press, Hold and Release
    KeyBindingBundle bindingsHalfAxes_ [nHalfAxes_s];
    //! Names of the half axes
    std::string namesHalfAxes_         [nHalfAxes_s];
    //! The configured string values
    std::string bindingStringsHalfAxes_[nHalfAxes_s];

    /**
    * Commands that have additional parameters (axes) are executed at the end of
    * the tick() so that all values can be buffered for single execution.
    */
    std::vector<AxisCommand*> axisCommands_;
  };


  /**
    @brief Captures mouse and keyboard input and distributes it to the
    GUI.
  */
  //class _CoreExport GUIInputHandler : public KeyHandler, public MouseHandler, public JoyStickHandler
  //{
  //public:
  //  GUIInputHandler ();
  //  ~GUIInputHandler();

  //private:
  //  // input events
    //bool keyPressed   (const OIS::KeyEvent   &arg);
    //bool keyReleased  (const OIS::KeyEvent   &arg);
    //bool keyHeld      (const OIS::KeyEvent   &arg);

  //  bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButton id);
    //bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButton id);
    //bool mouseHeld    (const OIS::MouseEvent &arg, OIS::MouseButton id);
  //  bool mouseMoved   (const OIS::MouseEvent &arg);

    //bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
    //bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
    //bool buttonHeld    (const OIS::JoyStickEvent &arg, int button);
    //bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);
    //bool sliderMoved   (const OIS::JoyStickEvent &arg, int id);
    //bool povMoved      (const OIS::JoyStickEvent &arg, int id);
  //};

}

#endif /* _InputHandler_H__ */
