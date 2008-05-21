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
#include <vector>

#include "ois/OIS.h"
#include "util/Math.h"
#include "OrxonoxClass.h"
#include "CommandExecutor.h"
#include "InputInterfaces.h"

namespace orxonox
{
  class _CoreExport BaseCommand
  {
  public:
    virtual ~BaseCommand() { }
    virtual bool execute(float abs = 1.0f, float rel = 1.0f) = 0;
  };

  class _CoreExport BufferedParamCommand
  {
  public:
    BufferedParamCommand() : value_(0.0f), nValuesAdded_(0), paramIndex_(-1) { }
    bool execute();

    float value_;
    unsigned int nValuesAdded_;
    int paramIndex_;
    CommandEvaluation evaluation_;
  };

  class _CoreExport SimpleCommand : public BaseCommand
  {
  public:
    bool execute(float abs = 1.0f, float rel = 1.0f);

    CommandEvaluation evaluation_;
  };

  class _CoreExport ParamCommand : public BaseCommand
  {
  public:
    ParamCommand() : bRelative_(false), paramModifier_(1.0f), paramCommand_(0) { }
    bool execute(float abs = 1.0f, float rel = 1.0f);

    bool bRelative_;
    float paramModifier_;
    BufferedParamCommand* paramCommand_;
  };

  class _CoreExport Button
  {
  public:
    Button() { nCommands_[0]=0; nCommands_[1]=0; nCommands_[2]=0; clear(); }
    virtual ~Button() { clear(); }
    virtual void clear();
    virtual bool addParamCommand(ParamCommand* command) { return false; }
    void parse(std::vector<BufferedParamCommand*>& paramCommandBuffer);
    bool execute(KeybindMode::Enum mode, float abs = 1.0f, float rel = 1.0f);

    //! The configured string value
    std::string bindingString_;
    //! Name of the trigger as strings
    std::string name_;
    //! Basic commands for OnPress, OnHold and OnRelease
    BaseCommand** commands_[3];
    //! Number of basic commands
    unsigned int nCommands_[3];
    //! Says how much it takes for an analog axis to trigger a button
    //! Note: This variable is here to have only one parse() function.
    float buttonThreshold_;
  };


  class _CoreExport HalfAxis : public Button
  {
  public:
    HalfAxis() : relVal_(0.0f), absVal_(0.0f), paramCommands_(0), nParamCommands_(0),
                 wasDown_(false), hasChanged_(false) { }
    using Button::execute;
    bool execute();
    //bool execute(KeybindMode::Enum mode) { return Button::execute(mode); }
    bool addParamCommand(ParamCommand* command);
    void clear();

    // axis related
    float relVal_;
    float absVal_;
    ParamCommand** paramCommands_;
    unsigned int nParamCommands_;

    // button related
    bool wasDown_;
    bool hasChanged_;
  };


  /**
    @brief Handles mouse, keyboard and joy stick input while in the actual game mode.
           Manages the key bindings.
  */
  class _CoreExport KeyBinder : public KeyHandler, public MouseHandler, public JoyStickHandler, public OrxonoxClass
  {
  public:
    KeyBinder ();
    ~KeyBinder();

    void loadBindings();
    void clearBindings(bool bInit = false);

    void setConfigValues();

  private: // functions
    void readTrigger(Button& button);

    //static void clearBundle(KeyBindingBundle& bundle, bool bInit);
    //static void redimensionBinding(KeyBinding& binding);

    void tick(float dt);

    void keyPressed (const KeyEvent& evt);
    void keyReleased(const KeyEvent& evt);
    void keyHeld    (const KeyEvent& evt);

    void mouseButtonPressed (MouseButton::Enum id);
    void mouseButtonReleased(MouseButton::Enum id);
    void mouseButtonHeld    (MouseButton::Enum id);
    void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
    void mouseScrolled      (int abs, int rel);

    void joyStickButtonPressed (int joyStickID, int button);
    void joyStickButtonReleased(int joyStickID, int button);
    void joyStickButtonHeld    (int joyStickID, int button);
    void joyStickAxisMoved     (int joyStickID, int axis, int value);

  private: // variables
    //! denotes the number of different keys there are in OIS.
    static const unsigned int nKeys_s = 0xEE;
    //! Actual key bindings as bundle for Press, Hold and Release
    Button keys_ [nKeys_s];

    //! denotes the number of different mouse buttons there are in OIS.
    static const unsigned int nMouseButtons_s = 8 + 2*2; // 8 buttons and 2 scroll wheels
    //! Actual key bindings as bundle for Press, Hold and Release
    Button mouseButtons_ [nMouseButtons_s];

    //! denotes the number of different joy stick buttons there are in OIS.
    static const unsigned int nJoyStickButtons_s = 32 + 4 * 4; // 32 buttons and 4 POVs with 4 buttons
    //! Actual key bindings as bundle for Press, Hold and Release
    Button joyStickButtons_ [nJoyStickButtons_s];

    //! denotes the number of half axes (every axis twice) there can be.
    static const unsigned int nHalfAxes_s = 56;
    /**
    * Array with all the half axes for mouse and joy sticks.
    * Keep in mind that the positions are fixed and that the first entry is the
    * positive one and the second is negative.
    * Sequence is as follows:
    *  0 -  3: Mouse x and y
    *  4 -  7: empty
    *  8 - 23: joy stick (slider) axes 1 to 8
    * 24 - 55: joy stick axes 1 - 16
    */
    HalfAxis halfAxes_[nHalfAxes_s];

    /**
    * Commands that have additional parameters (axes) are executed at the end of
    * the tick() so that all values can be buffered for single execution.
    */
    std::vector<BufferedParamCommand*> paramCommandBuffer_;

    //! Keeps track of the absolute mouse value (incl. scroll wheel)
    int mousePosition_[3];
    //! Used to derive mouse input if requested
    int mouseRelative_[2];
    float deriveTime_;

    //##### ConfigValues #####
    //! Threshold for analog triggers until which the state is 0.
    float analogThreshold_;
    //! Threshold for analog triggers until which the button is not pressed.
    float buttonThreshold_;
    //! Derive mouse input for absolute values?
    bool bDeriveMouseInput_;
    //! Accuracy of the mouse input deriver. The higher the more precise, but laggier.
    float derivePeriod_;
    //! mouse sensitivity
    float mouseSensitivity_;
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
