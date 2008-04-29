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

  /**
    @brief Interface class used for key input listeners.
  */
  class _CoreExport KeyHandler : public OIS::KeyListener
  {
  public:
    virtual bool keyHeld(const OIS::KeyEvent &arg) = 0;
  };

  /**
    @brief Interface class used for mouse input listeners.
  */
  class _CoreExport MouseHandler : public OIS::MouseListener
  {
  public:
    virtual bool mouseHeld(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
  };

  /**
    @brief Interface class used for joy stick input listeners.
  */
  class _CoreExport JoyStickHandler : public OIS::JoyStickListener
  {
  public:
    virtual bool buttonHeld(const OIS::JoyStickEvent &arg, int button) = 0;
  };
 

  /**
    @brief Captures mouse, keyboard and joy stick input while in the actual game mode.
           Manages the key bindings.
  */
  class _CoreExport KeyBinder : public KeyHandler, public MouseHandler, public JoyStickHandler
  {
  public:
    KeyBinder ();
    ~KeyBinder();

    bool loadBindings();
    void clearBindings();

  private: // functions
		bool keyPressed   (const OIS::KeyEvent   &arg);
		bool keyReleased  (const OIS::KeyEvent   &arg);
		bool keyHeld      (const OIS::KeyEvent   &arg);

    bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseHeld    (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved   (const OIS::MouseEvent &arg);

		bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
		bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
		bool buttonHeld    (const OIS::JoyStickEvent &arg, int button);
		bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);
		bool sliderMoved   (const OIS::JoyStickEvent &arg, int id);
		bool povMoved      (const OIS::JoyStickEvent &arg, int id);

  private: // variables
    /** denotes the maximum number of different keys there are in OIS.
        256 should be ok since the highest number in the OIS enum is 237. */
    static const int numberOfKeys_s = 256;
    //! Array of input events for every pressed key
    std::string bindingsKeyPress_  [numberOfKeys_s];
    //! Array of input events for every released key
    std::string bindingsKeyRelease_[numberOfKeys_s];
    //!Array of input events for every held key
    std::string bindingsKeyHold_   [numberOfKeys_s];

    /** denotes the maximum number of different buttons there are in OIS.
        16 should be ok since the highest number in the OIS enum is 7. */
    static const int numberOfMouseButtons_s = 16;
    //! Array of input events for every pressed mouse button
    std::string bindingsMouseButtonPress_  [numberOfMouseButtons_s];
    //! Array of input events for every released mouse button
    std::string bindingsMouseButtonRelease_[numberOfMouseButtons_s];
    //! Array of input events for every held mouse button
    std::string bindingsMouseButtonHold_   [numberOfMouseButtons_s];

    /** denotes the maximum number of different buttons there are in OIS.
        32 should be ok. */
    static const int numberOfJoyStickButtons_s = 32;
    //! Array of input events for every pressed joy stick button
    std::string bindingsJoyStickButtonPress_  [numberOfJoyStickButtons_s];
    //! Array of input events for every released joy stick button
    std::string bindingsJoyStickButtonRelease_[numberOfJoyStickButtons_s];
    //! Array of input events for every held joy stick button
    std::string bindingsJoyStickButtonHold_   [numberOfJoyStickButtons_s];

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

  //  bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		//bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		//bool mouseHeld    (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
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
