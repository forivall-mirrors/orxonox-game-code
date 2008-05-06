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

#include "ois/OIS.h"
#include "Tickable.h"
#include "InputInterfaces.h"

namespace orxonox
{
  /**
    @brief Captures and distributes mouse and keyboard input.
  */
  class _CoreExport InputManager
        : public Tickable,
          public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
  {
  public: // enumerations
    /**
      @brief Designates the way input is handled and redirected.
    */
    enum InputState
    {
      IS_UNINIT,  //!< InputManager has not yet been initialised.
      IS_NONE,    //!< Input is discarded.
      IS_NORMAL,  //!< Normal play state. Key and button bindings are active.
      IS_GUI,     //!< All OIS input events are passed to CEGUI.
      IS_CONSOLE, //!< Keyboard input is redirected to the InputBuffer.
      IS_CUSTOM   //!< Any possible configuration.
    };

  public: // static functions
    static bool initialise(const size_t windowHnd, const int windowWidth, const int windowHeight,
          const bool createKeyboard = true, const bool createMouse = true, const bool createJoySticks = false);
    static bool initialiseKeyboard();
    static bool initialiseMouse();
    static bool initialiseJoySticks();
    static int  numberOfKeyboards();
    static int  numberOfMice();
    static int  numberOfJoySticks();

    static void destroy();
    static void destroyKeyboard();
    static void destroyMouse();
    static void destroyJoySticks();

    static bool isModifierDown(KeyboardModifier::Enum modifier);
    static bool isKeyDown(KeyCode::Enum key);
    static const MouseState getMouseState();
    static const JoyStickState getJoyStickState(unsigned int ID);

    static void setWindowExtents(const int width, const int height);

    static void setInputState(const InputState state);
    static InputState getInputState();

    static bool addKeyHandler                 (KeyHandler* handler, const std::string& name);
    static bool removeKeyHandler              (const std::string& name);
    static KeyHandler* getKeyHandler          (const std::string& name);
    static bool enableKeyHandler              (const std::string& name);
    static bool disableKeyHandler             (const std::string& name);
    static bool isKeyHandlerActive            (const std::string& name);

    static bool addMouseHandler               (MouseHandler* handler, const std::string& name);
    static bool removeMouseHandler            (const std::string& name);
    static MouseHandler* getMouseHandler      (const std::string& name);
    static bool enableMouseHandler            (const std::string& name);
    static bool disableMouseHandler           (const std::string& name);
    static bool isMouseHandlerActive          (const std::string& name);

    static bool addJoyStickHandler            (JoyStickHandler* handler, const std::string& name);
    static bool removeJoyStickHandler         (const std::string& name);
    static JoyStickHandler* getJoyStickHandler(const std::string& name);
    static bool enableJoyStickHandler         (const std::string& name, unsigned int id);
    static bool disableJoyStickHandler        (const std::string& name, unsigned int id);
    static bool isJoyStickHandlerActive       (const std::string& name, unsigned int id);

  private: // functions
    // don't mess with a Singleton
    InputManager ();
    InputManager (const InputManager&);
    ~InputManager();

    // Intenal methods
    bool _initialise(const size_t, const int, const int, const bool, const bool, const bool);
    bool _initialiseKeyboard();
    bool _initialiseMouse();
    bool _initialiseJoySticks();

    void _destroy();
    void _destroyKeyboard();
    void _destroyMouse();
    void _destroyJoySticks();

    void tick(float dt);

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
    bool vector3Moved  (const OIS::JoyStickEvent &arg, int id);

    static InputManager& _getSingleton();
    static InputManager* _getSingletonPtr() { return &_getSingleton(); }

  private: // variables
    OIS::InputManager*                          inputSystem_; //!< OIS input manager
    OIS::Keyboard*                              keyboard_;    //!< OIS mouse
    OIS::Mouse*                                 mouse_;       //!< OIS keyboard
    std::vector<OIS::JoyStick*>                 joySticks_;   //!< OIS joy sticks
    unsigned int                                joySticksSize_;

    InputState state_;
    InputState stateRequest_;
    unsigned int keyboardModifiers_;

    std::map<std::string, KeyHandler*>          keyHandlers_;
    std::map<std::string, MouseHandler*>        mouseHandlers_;
    std::map<std::string, JoyStickHandler*>     joyStickHandlers_;

    std::vector<KeyHandler*>                    activeKeyHandlers_;
    std::vector<MouseHandler*>                  activeMouseHandlers_;
    std::vector<std::vector<JoyStickHandler*> > activeJoyStickHandlers_;

    std::vector<Key>                            keysDown_;
    std::vector<MouseButton::Enum>            mouseButtonsDown_;
    std::vector<std::vector<int> >              joyStickButtonsDown_;

  };

}

#endif /* _InputManager_H__ */
