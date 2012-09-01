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
@brief
    Declarations of all key/button/axis code enumeration and string literals
    and an input device enumeration.
*/

#ifndef _InputPrereqs_H__
#define _InputPrereqs_H__

#include "core/CorePrereqs.h"

#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <ois/OISJoyStick.h>

namespace orxonox
{
    //-----------------------------------------------------------------------
    // Code enumerations
    //-----------------------------------------------------------------------

    namespace KeyCode
    {
        const unsigned int numberOfKeys = 0xEE; // 238

        //! Key codes as enumeration
        enum ByEnum
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
            NumLock       = OIS::KC_NUMLOCK,
            ScrollLock    = OIS::KC_SCROLL,          // Scroll Lock
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
            SystemRequest = OIS::KC_SYSRQ,
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

        //! Key codes as strings
        const char* const ByString[] =
        {
            "Unassigned",
            "Escape",
            "NumRow1", "NumRow2", "NumRow3", "NumRow4", "NumRow5",
            "NumRow6", "NumRow7", "NumRow8", "NumRow9", "NumRow0",
            "Minus", "Equals", "Back", "Tab",
            "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
            "LeftBracket", "RightBracket",
            "Return", "LeftControl",
            "A", "S", "D", "F", "G", "H", "J", "K", "L",
            "Semicolon", "Apostrophe", "Grave",
            "LeftShift", "Backslash",
            "Z", "X", "C", "V", "B", "N", "M",
            "Comma", "Period", "Slash",
            "RightShift",
            "Multiply",
            "LeftAlt",
            "Space",
            "CapsLock",
            "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
            "NumLock", "ScrollLock",
            "Numpad7", "Numpad8", "Numpad9",
            "NumpadSubtract",
            "Numpad4", "Numpad5", "Numpad6",
            "NumpadAdd",
            "Numpad1", "Numpad2", "Numpad3", "Numpad0",
            "NumpadPeriod",
            "","",
            "LessThan",
            "F11", "F12",
            "","","","","","","","","","","",
            "F13", "F14", "F15",
            "","","","","","","","","","",
            "Kana",
            "","",
            "ABNT_C1",
            "","","","","",
            "Convert",
            "",
            "NoConvert",
            "",
            "Yen",
            "ABNT_C2",
            "","","","","","","","","","","","","","",
            "NumpadEquals",
            "","",
            "PreviousTrack",
            "AT",
            "Colon", "Underline",
            "Kanji",
            "Stop",
            "AX",
            "Unlabeled",
            "NextTrack",
            "","",
            "NumpadEnter",
            "RightControl",
            "","",
            "Mute",
            "Calculator",
            "PlayPause",
            "",
            "MediaStop",
            "","","","","","","","","",
            "VolumeDown",
            "",
            "VolumeUp",
            "",
            "WebHome",
            "NumpadComma",
            "",
            "Divide",
            "",
            "SystemRequest",
            "RightAlt",
            "","","","","","","","","","","","",
            "Pause",
            "",
            "Home",
            "UP",
            "PageUp",
            "",
            "Left",
            "",
            "Right",
            "",
            "End", "Down", "PageDown", "Insert", "Delete",
            "","","","","","","",
            "LeftWindows", "RightWindows", "Apps",
            "Power", "Sleep",
            "","","",
            "Wake",
            "",
            "WebSearch", "WebFavorites", "WebRefresh", "WebStop", "WebForward", "WebBack",
            "MyComputer", "Mail", "MediaSelect"
        };
    }


    namespace MouseButtonCode
    {
        const unsigned int numberOfButtons = 8;

        //! Mouse button codes as enumeration
        enum ByEnum
        {
            Left    = OIS::MB_Left,
            Right   = OIS::MB_Right,
            Middle  = OIS::MB_Middle,
            Button3 = OIS::MB_Button3,
            Button4 = OIS::MB_Button4,
            Button5 = OIS::MB_Button5,
            Button6 = OIS::MB_Button6,
            Button7 = OIS::MB_Button7,
        };

        // Mouse button codes as strings
        const char* const ByString[] =
        {
            "Left",
            "Right",
            "Middle",
            "Button3",
            "Button4",
            "Button5",
            "Button6",
            "Button7",
        };
    }

    namespace MouseAxisCode
    {
        const unsigned int numberOfAxes = 2;

        // Mouse axis codes as enumeration
        enum ByEnum
        {
            X,
            Y
        };

        // Mouse axis codes as strings
        const char* const ByString[] =
        {
            "X",
            "Y"
        };
    }


    namespace JoyStickButtonCode
    {
        const unsigned int numberOfButtons = 64;

        // Joy stick button codes as enumeration
        enum ByEnum
        {
            Button0       =  0, Button1       =  1, Button2       =  2, Button3       =  3,
            Button4       =  4, Button5       =  5, Button6       =  6, Button7       =  7,
            Button8       =  8, Button9       =  9, Button10      = 10, Button11      = 11,
            Button12      = 12, Button13      = 13, Button14      = 14, Button15      = 15,
            Button16      = 16, Button17      = 17, Button18      = 18, Button19      = 19,
            Button20      = 20, Button21      = 21, Button22      = 22, Button23      = 23,
            Button24      = 24, Button25      = 25, Button26      = 26, Button27      = 27,
            Button28      = 28, Button29      = 29, Button30      = 30, Button31      = 31,

            POV0North     = 32, POV0South     = 33, POV0East      = 34, POV0West      = 35,
            POV0NorthEast = 36, POV0SouthEast = 37, POV0NorthWest = 38, POV0SouthWest = 39,

            POV1North     = 40, POV1South     = 41, POV1East      = 42, POV1West      = 43,
            POV1NorthEast = 44, POV1SouthEast = 45, POV1NorthWest = 46, POV1SouthWest = 47,

            POV2North     = 48, POV2South     = 49, POV2East      = 50, POV2West      = 51,
            POV2NorthEast = 52, POV2SouthEast = 53, POV2NorthWest = 54, POV2SouthWest = 55,

            POV3North     = 56, POV3South     = 57, POV3East      = 58, POV3West      = 59,
            POV3NorthEast = 60, POV3SouthEast = 61, POV3NorthWest = 62, POV3SouthWest = 63,
        };

        // Joy stick button codes as strings
        const char* const ByString[] =
        {
            "Button00",      "Button01",      "Button02",      "Button03",
            "Button04",      "Button05",      "Button06",      "Button07",
            "Button08",      "Button09",      "Button10",      "Button11",
            "Button12",      "Button13",      "Button14",      "Button15",
            "Button16",      "Button17",      "Button18",      "Button19",
            "Button20",      "Button21",      "Button22",      "Button23",
            "Button24",      "Button25",      "Button26",      "Button27",
            "Button28",      "Button29",      "Button30",      "Button31",
            "POV0North",     "POV0South",     "POV0East",      "POV0West",
            "POV0NorthEast", "POV0SouthEast", "POV0NorthWest", "POV0SouthWest",
            "POV1North",     "POV1South",     "POV1East",      "POV1West",
            "POV1NorthEast", "POV1SouthEast", "POV1NorthWest", "POV1SouthWest",
            "POV2North",     "POV2South",     "POV2East",      "POV2West",
            "POV2NorthEast", "POV2SouthEast", "POV2NorthWest", "POV2SouthWest",
            "POV3North",     "POV3South",     "POV3East",      "POV3West",
            "POV3NorthEast", "POV3SouthEast", "POV3NorthWest", "POV3SouthWest"
        };
    }

    namespace JoyStickAxisCode
    {
        const unsigned int numberOfAxes = 24;

        // Joy stick axis codes as enumeration
        enum ByEnum
        {
            Slider0 =  0, Slider1 =  1, Slider2 =  2, Slider3 =  3,
            Slider4 =  4, Slider5 =  5, Slider6 =  6, Slider7 =  7,
            Axis0   =  8, Axis1   =  9, Axis2   = 10, Axis3   = 11,
            Axis4   = 12, Axis5   = 13, Axis6   = 14, Axis7   = 15,
            Axis8   = 16, Axis9   = 17, Axis10  = 18, Axis11  = 19,
            Axis12  = 20, Axis13  = 21, Axis14  = 22, Axis15  = 23
        };

        // Joy stick axis codes as strings
        const char* const ByString[] =
        {
            "Slider0", "Slider1", "Slider2", "Slider3",
            "Slider4", "Slider5", "Slider6", "Slider7",
            "Axis00",  "Axis01",  "Axis02",  "Axis03",
            "Axis04",  "Axis05",  "Axis06",  "Axis07",
            "Axis08",  "Axis09",  "Axis10",  "Axis11",
            "Axis12",  "Axis13",  "Axis14",  "Axis15"
        };
    }


    //-----------------------------------------------------------------------
    // Miscellaneous
    //-----------------------------------------------------------------------

    namespace InputDeviceEnumerator
    {
        //! Used to access the devices in an array
        enum Value
        {
            Keyboard = 0,
            Mouse = 1,
            FirstJoyStick = 2
        };
    }
}

#endif /* _InputPrereqs_H__ */
