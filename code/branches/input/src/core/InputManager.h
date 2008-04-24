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
 @brief Implementation of a little Input handler that distributes everything
        coming from OIS.
 */

#ifndef _InputManager_H__
#define _InputManager_H__

#include "CorePrereqs.h"

#include <map>
#include <list>
#include <OIS/OIS.h>

#include "Tickable.h"
#include "InputEvent.h"

namespace orxonox
{
  /**
    @brief Captures and distributes mouse and keyboard input.
    It resolves the key bindings to InputEvents which can be heard by
    implementing the InputEventListener interface.
  */
  class _CoreExport InputManager
        : public Tickable,
          public OIS::MouseListener, public OIS::KeyListener, public OIS::JoyStickListener
  {
  public:
    /**
      @brief Designates the way input is handled currently.
      IM_GUI:    All the OIS input events are passed to CEGUI
      IM_BUFFER: Only keyboard input is captured and passed to the InputBuffer
      IM_NORMAL: Normal game mode. Key bindings and mouse are active.
    */
    enum InputState
    {
      IS_UNINIT,
      IS_NONE,
      IS_NORMAL,
      IS_GUI,
      IS_CONSOLE,
      IS_CUSTOM
    };

  public:
    void tick(float dt);

    static bool initialise(size_t windowHnd, int windowWidth, int windowHeight);
    static void destroy();

    static void setWindowExtents(int width, int height);
    static void setInputState(const InputState state);
    static InputState getInputState();

    static bool addKeyListener(OIS::KeyListener* listener, const std::string& name);
    //static bool removeKeyListener(OIS::KeyListener* listener);
    static bool removeKeyListener(const std::string& name);
    static bool enableKeyListener(const std::string& name);
    static bool disableKeyListener(const std::string& name);
    static OIS::KeyListener* getKeyListener(const std::string& name);
    static bool isKeyListenerActive(const std::string& name);

    static bool addMouseListener(OIS::MouseListener* listener, const std::string& name);
    //static bool removeMouseListener(OIS::MouseListener* listener);
    static bool removeMouseListener(const std::string& name);
    static bool enableMouseListener(const std::string& name);
    static bool disableMouseListener(const std::string& name);
    static OIS::MouseListener* getMouseListener(const std::string& name);
    static bool isMouseListenerActive(const std::string& name);

    static bool addJoyStickListener(OIS::JoyStickListener* listener, const std::string& name);
    //static bool removeJoyStickListener(OIS::JoyStickListener* listener);
    static bool removeJoyStickListener(const std::string& name);
    static bool enableJoyStickListener(const std::string& name);
    static bool disableJoyStickListener(const std::string& name);
    static OIS::JoyStickListener* getJoyStickListener(const std::string& name);
    static bool isJoyStickListenerActive(const std::string& name);

    // Temporary solutions. Will be removed soon!
    static OIS::Mouse*    getMouse()    { return _getSingleton().mouse_   ; }
    static OIS::Keyboard* getKeyboard() { return _getSingleton().keyboard_; }

  private:
    // don't mess with a Singleton
    InputManager ();
    InputManager (const InputManager&);
    ~InputManager();

    bool _initialise(size_t windowHnd, int windowWidth, int windowHeight);
    void _destroy();
    void _setDefaultState();
    void _loadBindings();

    // input events
		bool mousePressed  (const OIS::MouseEvent    &arg, OIS::MouseButtonID id);
		bool mouseReleased (const OIS::MouseEvent    &arg, OIS::MouseButtonID id);
    bool mouseMoved    (const OIS::MouseEvent    &arg);
		bool keyPressed    (const OIS::KeyEvent      &arg);
		bool keyReleased   (const OIS::KeyEvent      &arg);
		bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
		bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
		bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);
		bool sliderMoved   (const OIS::JoyStickEvent &arg, int id);
		bool povMoved      (const OIS::JoyStickEvent &arg, int id);

    static InputManager& _getSingleton();
    static InputManager* _getSingletonPtr() { return &_getSingleton(); }

  private:
    OIS::InputManager* inputSystem_;    //!< OIS input manager
    OIS::Keyboard*     keyboard_;       //!< OIS mouse
    OIS::Mouse*        mouse_;          //!< OIS keyboard

    InputState state_;
    InputState stateRequest_;

    std::list<OIS::KeyCode> keysDown_;

    bool bDefaultKeyInput;
    bool bDefaultMouseInput;
    bool bDefaultJoyStickInput;

    std::map<std::string, OIS::KeyListener*>      keyListeners_;
    std::list<OIS::KeyListener*>                  activeKeyListeners_;
    std::map<std::string, OIS::MouseListener*>    mouseListeners_;
    std::list<OIS::MouseListener*>                activeMouseListeners_;
    std::map<std::string, OIS::JoyStickListener*> joystickListeners_;
    std::list<OIS::JoyStickListener*>             activeJoyStickListeners_;


    /** denotes the maximum number of different keys there are in OIS.
        256 should be ok since the highest number in the enum is 237. */
    static const int numberOfKeys_s = 256;
    //! Array of input events for every pressed key
    std::string bindingsKeyPress_  [numberOfKeys_s];
    //! Array of input events for every released key
    std::string bindingsKeyRelease_[numberOfKeys_s];
    //! Array of input events for every held key
    std::string bindingsKeyHold_   [numberOfKeys_s];

    /** denotes the maximum number of different buttons there are in OIS.
        16 should be ok since the highest number in the enum is 7. */
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
    //! Array of input events for every pressed joystick button
    std::string bindingsJoyStickButtonPress_  [numberOfJoyStickButtons_s];
    //! Array of input events for every released joystick button
    std::string bindingsJoyStickButtonRelease_[numberOfJoyStickButtons_s];
    //! Array of input events for every held joystick button
    std::string bindingsJoyStickButtonHold_   [numberOfJoyStickButtons_s];

  };
}

#endif /* _InputManager_H__ */
