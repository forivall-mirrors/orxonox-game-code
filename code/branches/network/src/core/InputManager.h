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
#include <vector>

#include "ois/OIS.h"
#include "util/Math.h"
#include "Tickable.h"
#include "InputInterfaces.h"

namespace orxonox
{
  /**
  * Helper class to realise a vector<int[4]>
  */
  class POVStates
  {
  public:
    int operator[](unsigned int index) { return povStates[index]; }
    int povStates[4];
  };

  /**
  * Helper class to realise a vector< {int[4], int[4]} >
  */
  class SliderStates
  {
  public:
    IntVector2 sliderStates[4];
  };

  /**
    @brief Captures and distributes mouse and keyboard input.
  */
  class _CoreExport InputManager
        : public TickableReal,
          public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
  {
  public: // enumerations
    /**
      @brief Designates the way input is handled and redirected.
    */
    enum InputState
    {
      IS_UNINIT,    //!< InputManager has not yet been initialised.
      IS_NONE,      //!< Input is discarded.
      IS_NORMAL,    //!< Normal play state. Key and button bindings are active.
      IS_GUI,       //!< All OIS input events are passed to CEGUI.
      IS_CONSOLE,   //!< Keyboard input is redirected to the InputBuffer.
      IS_DETECT,    //!< All the input additionally goes to the KeyDetector
      IS_NODETECT,  //!< remove KeyDetector
      IS_CUSTOM     //!< Any possible configuration.
    };

  public: // static functions
    static bool initialise(const size_t windowHnd, int windowWidth, int windowHeight,
          bool createKeyboard = true, bool createMouse = true, bool createJoySticks = false);
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

    //static bool isModifierDown(KeyboardModifier::Enum modifier);
    //static bool isKeyDown(KeyCode::Enum key);
    //static const MouseState getMouseState();
    //static const JoyStickState getJoyStickState(unsigned int ID);

    static void setWindowExtents(const int width, const int height);

    static void setInputState(const InputState state);
    static InputState getInputState();

    static void storeKeyStroke(const std::string& name);
    static void keyBind(const std::string& command);

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
    bool _initialise(const size_t, int, int, bool, bool, bool);
    bool _initialiseKeyboard();
    bool _initialiseMouse();
    bool _initialiseJoySticks();

    void _destroy();
    void _destroyKeyboard();
    void _destroyMouse();
    void _destroyJoySticks();

    void _updateTickables();

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
    //bool vector3Moved  (const OIS::JoyStickEvent &arg, int id);

    static InputManager& _getSingleton();
    static InputManager* _getSingletonPtr() { return &_getSingleton(); }

  private: // variables
    OIS::InputManager*                          inputSystem_;     //!< OIS input manager
    OIS::Keyboard*                              keyboard_;        //!< OIS mouse
    OIS::Mouse*                                 mouse_;           //!< OIS keyboard
    std::vector<OIS::JoyStick*>                 joySticks_;       //!< OIS joy sticks
    unsigned int                                joySticksSize_;

    KeyBinder*                                  keyBinder_;       //!< KeyBinder instance
    KeyDetector*                                keyDetector_;     //!< KeyDetector instance
    InputBuffer*                                buffer_;          //!< InputBuffer instance

    InputState state_;
    InputState stateRequest_;
    unsigned int keyboardModifiers_;
    InputState savedState_;

    //! Keeps track of the joy stick POV states
    std::vector<POVStates>                      povStates_;
    //! Keeps track of the possibly two slider axes
    std::vector<SliderStates>                   sliderStates_;

    std::map<std::string, KeyHandler*>          keyHandlers_;
    std::map<std::string, MouseHandler*>        mouseHandlers_;
    std::map<std::string, JoyStickHandler*>     joyStickHandlers_;

    std::vector<KeyHandler*>                    activeKeyHandlers_;
    std::vector<MouseHandler*>                  activeMouseHandlers_;
    std::vector<std::vector<JoyStickHandler*> > activeJoyStickHandlers_;
    std::vector<std::pair<InputTickable*, HandlerState> > activeHandlers_;

    std::vector<Key>                            keysDown_;
    std::vector<MouseButton::Enum>              mouseButtonsDown_;
    std::vector<std::vector<int> >              joyStickButtonsDown_;

    static std::string                          bindingCommmandString_s;
  };

}

#endif /* _InputManager_H__ */
