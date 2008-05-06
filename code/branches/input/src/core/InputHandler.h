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
#include "OrxonoxClass.h"
#include "CommandExecutor.h"
#include "InputInterfaces.h"

namespace orxonox
{
  namespace KeybindSetting
  {
    enum KeybindSetting
    {
      None,
      OnPress,
      OnRelease,
      Continuous,
    };
  }

  struct _CoreExport KeyBinding
  {
    std::string commandStr;
    CommandEvaluation evaluation;
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
    void clearBindings();

    void setConfigValues();

    std::string testtest;

  private: // functions

    bool executeBinding(KeyBinding &binding);

    bool keyPressed (const KeyEvent& evt);
    bool keyReleased(const KeyEvent& evt);
    bool keyHeld    (const KeyEvent& evt);

    bool mouseButtonPressed (const MouseState& state, MouseButton::Enum id);
    bool mouseButtonReleased(const MouseState& state, MouseButton::Enum id);
    bool mouseButtonHeld    (const MouseState& state, MouseButton::Enum id);
    bool mouseMoved         (const MouseState& state);
    bool mouseWheelTurned   (const MouseState& state);

		bool joyStickButtonPressed (const JoyStickState& state, int button);
		bool joyStickButtonReleased(const JoyStickState& state, int button);
    bool joyStickButtonHeld    (const JoyStickState& state, int button);
		bool joyStickAxisMoved     (const JoyStickState& state, int axis)  ;
		bool joyStickSliderMoved   (const JoyStickState& state, int index) ;
		bool joyStickPovMoved      (const JoyStickState& state, int index) ;
		bool joyStickVector3Moved  (const JoyStickState& state, int index) ;

  private: // variables

    //! denotes the number of different keys there are in OIS.
    static const int numberOfKeys_s = 0xEE;
    //! Array of input events for every pressed key
    KeyBinding bindingsKeyPress_  [numberOfKeys_s];
    //! Array of input events for every released key
    KeyBinding bindingsKeyRelease_[numberOfKeys_s];
    //! Array of input events for every held key
    KeyBinding bindingsKeyHold_   [numberOfKeys_s];
    //! Names of the keys as strings
    std::string keyNames_[numberOfKeys_s];

    //! denotes the number of different mouse buttons there are in OIS.
    static const int numberOfMouseButtons_s = 8;
    //! Array of input events for every pressed mouse button
    KeyBinding bindingsMouseButtonPress_  [numberOfMouseButtons_s];
    //! Array of input events for every released mouse button
    KeyBinding bindingsMouseButtonRelease_[numberOfMouseButtons_s];
    //! Array of input events for every held mouse button
    KeyBinding bindingsMouseButtonHold_   [numberOfMouseButtons_s];
    //! Names of the mouse buttons as strings
    std::string mouseButtonNames_[numberOfMouseButtons_s];

    //! denotes the number of different joy stick buttons there are in OIS.
    static const int numberOfJoyStickButtons_s = 32;
    //! Array of input events for every pressed joy stick button
    KeyBinding bindingsJoyStickButtonPress_  [numberOfJoyStickButtons_s];
    //! Array of input events for every released joy stick button
    KeyBinding bindingsJoyStickButtonRelease_[numberOfJoyStickButtons_s];
    //! Array of input events for every held joy stick button
    KeyBinding bindingsJoyStickButtonHold_   [numberOfJoyStickButtons_s];
    //! Names of the joy stick buttons as strings
    std::string joyStickButtonNames_[numberOfJoyStickButtons_s];

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
