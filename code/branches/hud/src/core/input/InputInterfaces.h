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
 @brief Declarations of various interface classes for the input management.
*/

#ifndef _InputInterfaces_H__
#define _InputInterfaces_H__

#include "core/CorePrereqs.h"

#include "src/ois/OISKeyboard.h"
#include "src/ois/OISMouse.h"
#include "src/ois/OISJoyStick.h"
#include "util/Math.h"

namespace orxonox
{
  namespace KeyCode
  {
    // note: KeyCode comments were directly taken from OISKeyboard.h
    enum Enum
    {
      Unassigned    = OIS::KC_UNASSIGNED,
      Escape        = OIS::KC_ESCAPE,
      NumRow1       = OIS::KC_1,
      NumRow2       = OIS::KC_2,
      NumRow3       = OIS::KC_3,
      NumRow4       = OIS::KC_4,
      NumRow5       = OIS::KC_5,
      NumRow6       = OIS::KC_6,
      NumRow7       = OIS::KC_7,
      NumRow8       = OIS::KC_8,
      NumRow9       = OIS::KC_9,
      NumRow0       = OIS::KC_0,
      Minus         = OIS::KC_MINUS,           // - on main keyboard
      Equals        = OIS::KC_EQUALS,
      Back          = OIS::KC_BACK,            // backspace
      Tab           = OIS::KC_TAB,
      Q             = OIS::KC_Q,
      W             = OIS::KC_W,
      E             = OIS::KC_E,
      R             = OIS::KC_R,
      T             = OIS::KC_T,
      Y             = OIS::KC_Y,
      U             = OIS::KC_U,
      I             = OIS::KC_I,
      O             = OIS::KC_O,
      P             = OIS::KC_P,
      LeftBracket   = OIS::KC_LBRACKET,
      RightBracket  = OIS::KC_RBRACKET,
      Return        = OIS::KC_RETURN,          // Enter on main keyboard
      LeftControl   = OIS::KC_LCONTROL,
      A             = OIS::KC_A,
      S             = OIS::KC_S,
      D             = OIS::KC_D,
      F             = OIS::KC_F,
      G             = OIS::KC_G,
      H             = OIS::KC_H,
      J             = OIS::KC_J,
      K             = OIS::KC_K,
      L             = OIS::KC_L,
      Semicolon     = OIS::KC_SEMICOLON,
      Apostrophe    = OIS::KC_APOSTROPHE,
      Grave         = OIS::KC_GRAVE,           // accent
      LeftShift     = OIS::KC_LSHIFT,
      Backslash     = OIS::KC_BACKSLASH,
      Z             = OIS::KC_Z,
      X             = OIS::KC_X,
      C             = OIS::KC_C,
      V             = OIS::KC_V,
      B             = OIS::KC_B,
      N             = OIS::KC_N,
      M             = OIS::KC_M,
      Comma         = OIS::KC_COMMA,
      Period        = OIS::KC_PERIOD,          // . on main keyboard
      Slash         = OIS::KC_SLASH,           // / on main keyboard
      RightShift    = OIS::KC_RSHIFT,
      Multiply      = OIS::KC_MULTIPLY,        // * on numeric keypad
      LeftAlt       = OIS::KC_LMENU,           // left Alt
      Space         = OIS::KC_SPACE,
      CapsLock      = OIS::KC_CAPITAL,
      F1            = OIS::KC_F1,
      F2            = OIS::KC_F2,
      F3            = OIS::KC_F3,
      F4            = OIS::KC_F4,
      F5            = OIS::KC_F5,
      F6            = OIS::KC_F6,
      F7            = OIS::KC_F7,
      F8            = OIS::KC_F8,
      F9            = OIS::KC_F9,
      F10           = OIS::KC_F10,
      Numlock       = OIS::KC_NUMLOCK,
      Scrolllock    = OIS::KC_SCROLL,          // Scroll Lock
      Numpad7       = OIS::KC_NUMPAD7,
      Numpad8       = OIS::KC_NUMPAD8,
      Numpad9       = OIS::KC_NUMPAD9,
      NumpadSubtract= OIS::KC_SUBTRACT,        // - on numeric keypad
      Numpad4       = OIS::KC_NUMPAD4,
      Numpad5       = OIS::KC_NUMPAD5,
      Numpad6       = OIS::KC_NUMPAD6,
      NumpadAdd     = OIS::KC_ADD,             // + on numeric keypad
      Numpad1       = OIS::KC_NUMPAD1,
      Numpad2       = OIS::KC_NUMPAD2,
      Numpad3       = OIS::KC_NUMPAD3,
      Numpad0       = OIS::KC_NUMPAD0,
      NumpadPeriod  = OIS::KC_DECIMAL,         // . on numeric keypad
      LessThan      = OIS::KC_OEM_102,         // < > | on UK/Germany keyboards
      F11           = OIS::KC_F11,
      F12           = OIS::KC_F12,
      F13           = OIS::KC_F13,             //                     (NEC PC98)
      F14           = OIS::KC_F14,             //                     (NEC PC98)
      F15           = OIS::KC_F15,             //                     (NEC PC98)
      Kana          = OIS::KC_KANA,            // (Japanese keyboard)
      ABNT_C1       = OIS::KC_ABNT_C1,         // / ? on Portugese (Brazilian) keyboards
      Convert       = OIS::KC_CONVERT,         // (Japanese keyboard)
      NoConvert     = OIS::KC_NOCONVERT,       // (Japanese keyboard)
      Yen           = OIS::KC_YEN,             // (Japanese keyboard)
      ABNT_C2       = OIS::KC_ABNT_C2,         // Numpad . on Portugese (Brazilian) keyboards
      NumpadEquals  = OIS::KC_NUMPADEQUALS,    // = on numeric keypad (NEC PC98)
      PreviousTrack = OIS::KC_PREVTRACK,       // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
      AT            = OIS::KC_AT,              //                     (NEC PC98)
      Colon         = OIS::KC_COLON,           //                     (NEC PC98)
      Underline     = OIS::KC_UNDERLINE,       //                     (NEC PC98)
      Kanji         = OIS::KC_KANJI,           // (Japanese keyboard)
      Stop          = OIS::KC_STOP,            //                     (NEC PC98)
      AX            = OIS::KC_AX,              //                     (Japan AX)
      Unlabeled     = OIS::KC_UNLABELED,       //                        (J3100)
      NextTrack     = OIS::KC_NEXTTRACK,       // Next Track
      NumpadEnter   = OIS::KC_NUMPADENTER,     // Enter on numeric keypad
      RightControl  = OIS::KC_RCONTROL,
      Mute          = OIS::KC_MUTE,            // Mute
      Calculator    = OIS::KC_CALCULATOR,      // Calculator
      PlayPause     = OIS::KC_PLAYPAUSE,       // Play / Pause
      MediaStop     = OIS::KC_MEDIASTOP,       // Media Stop
      VolumeDown    = OIS::KC_VOLUMEDOWN,      // Volume -
      VolumeUp      = OIS::KC_VOLUMEUP,        // Volume +
      WebHome       = OIS::KC_WEBHOME,         // Web home
      NumpadComma   = OIS::KC_NUMPADCOMMA,     // , on numeric keypad (NEC PC98)
      Divide        = OIS::KC_DIVIDE,          // / on numeric keypad
      SYSRQ         = OIS::KC_SYSRQ,
      RightAlt      = OIS::KC_RMENU,           // right Alt
      Pause         = OIS::KC_PAUSE,           // Pause
      Home          = OIS::KC_HOME,            // Home on arrow keypad
      Up            = OIS::KC_UP,              // UpArrow on arrow keypad
      PageUp        = OIS::KC_PGUP,            // PgUp on arrow keypad
      Left          = OIS::KC_LEFT,            // LeftArrow on arrow keypad
      Right         = OIS::KC_RIGHT,           // RightArrow on arrow keypad
      End           = OIS::KC_END,             // End on arrow keypad
      Down          = OIS::KC_DOWN,            // DownArrow on arrow keypad
      PageDown      = OIS::KC_PGDOWN,          // PgDn on arrow keypad
      Insert        = OIS::KC_INSERT,          // Insert on arrow keypad
      Delete        = OIS::KC_DELETE,          // Delete on arrow keypad
      LeftWindows   = OIS::KC_LWIN,            // Left Windows key
      RightWindows  = OIS::KC_RWIN,            // Right Windows key
      Apps          = OIS::KC_APPS,            // AppMenu key
      Power         = OIS::KC_POWER,           // System Power
      Sleep         = OIS::KC_SLEEP,           // System Sleep
      Wake          = OIS::KC_WAKE,            // System Wake
      WebSearch     = OIS::KC_WEBSEARCH,       // Web Search
      WebFavorites  = OIS::KC_WEBFAVORITES,    // Web Favorites
      WebRefresh    = OIS::KC_WEBREFRESH,      // Web Refresh
      WebStop       = OIS::KC_WEBSTOP,         // Web Stop
      WebForward    = OIS::KC_WEBFORWARD,      // Web Forward
      WebBack       = OIS::KC_WEBBACK,         // Web Back
      MyComputer    = OIS::KC_MYCOMPUTER,      // My Computer
      Mail          = OIS::KC_MAIL,            // Mail
      MediaSelect   = OIS::KC_MEDIASELECT      // Media Select
    };
  }

  namespace MouseButton
  {
    enum Enum
    {
      Left    = OIS::MB_Left,
      Right   = OIS::MB_Right,
      Middle  = OIS::MB_Middle,
      Button3 = OIS::MB_Button3,
      Button4 = OIS::MB_Button4,
      Button5 = OIS::MB_Button5,
      Button6 = OIS::MB_Button6,
      Button7 = OIS::MB_Button7
    };
  }

  namespace KeyboardModifier
  {
    enum Enum
    {
      Shift = 0x0000001,
      Ctrl  = 0x0000010,
      Alt   = 0x0000100
    };
  }

  struct _CoreExport Key
  {
    Key(const OIS::KeyEvent& evt) : key((KeyCode::Enum)evt.key), text(evt.text) { }
    KeyCode::Enum key;
    unsigned int text;
  };

  class _CoreExport KeyEvent
  {
  public:
    KeyEvent(KeyCode::Enum key, unsigned int text) : key(key), text(text) { }
    KeyEvent(const OIS::KeyEvent& evt, unsigned int mod) : key((KeyCode::Enum)evt.key), text(evt.text), modifiers(mod) { }
    KeyEvent(const Key& key, unsigned int mod) : key(key.key), text(key.text), modifiers(mod) { }
    bool isModifierDown(KeyboardModifier::Enum modifier) const { return (KeyboardModifier::Enum)modifier&modifiers; }

    const KeyCode::Enum key;
    unsigned int text;
    unsigned int modifiers;
  };

  //typedef OIS::MouseState MouseState;

  /*class _CoreExport JoyStickState
  {
  public:
    JoyStickState(const OIS::JoyStickState& state, int ID) : OIS::JoyStickState(state), mJoyStickID(ID) { }
    JoyStickState() { clear(); }
    int mJoyStickID;
		JoyStickState() { clear(); }

		std::vector<bool> mButtons;
		int axes[16];
		std::vector<Vector3> mVectors;
  };*/

  /**
  * Helper struct to determine which handlers of an object (can implement
  * multiple handlers) are active.
  */
  struct HandlerState
  {
    HandlerState() : key(false), mouse(false), joyStick(false) { }
    bool key;
    bool mouse;
    bool joyStick;
  };

  class _CoreExport InputTickable
  {
  public:
    virtual ~InputTickable() { }
    virtual void tickInput(float dt, const HandlerState& state) = 0;
  };

  /**
    @brief Interface class used for key input listeners.
  */
  class _CoreExport KeyHandler : virtual public InputTickable
  {
  public:
    virtual ~KeyHandler() { }
    virtual void keyPressed (const KeyEvent& evt) = 0;
    virtual void keyReleased(const KeyEvent& evt) = 0;
    virtual void keyHeld    (const KeyEvent& evt) = 0;
    //virtual void tickKey    (float dt) { }
  };

  /**
    @brief Interface class used for mouse input listeners.
  */
  class _CoreExport MouseHandler : virtual public InputTickable
  {
  public:
    virtual ~MouseHandler() { }
    virtual void mouseButtonPressed (MouseButton::Enum id) = 0;
    virtual void mouseButtonReleased(MouseButton::Enum id) = 0;
    virtual void mouseButtonHeld    (MouseButton::Enum id) = 0;
    virtual void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize) = 0;
    virtual void mouseScrolled      (int abs, int rel)     = 0;
    //virtual void tickMouse          (float dt) { }
  };


  /**
    @brief Interface class used for joy stick input listeners.
  */
  class _CoreExport JoyStickHandler : virtual public InputTickable
  {
  public:
    virtual ~JoyStickHandler() { }
    virtual void joyStickButtonPressed (int joyStickID, int button) = 0;
    virtual void joyStickButtonReleased(int joyStickID, int button) = 0;
    virtual void joyStickButtonHeld    (int joyStickID, int button) = 0;
    virtual void joyStickAxisMoved     (int joyStickID, int axis, float value) = 0;
    //virtual bool joyStickVector3Moved  (int joyStickID, int index /*, fill list*/) {return true;}
    //virtual void tickJoyStick          (float dt) { }
  };

}

#endif /* _InputInterfaces_H__ */
