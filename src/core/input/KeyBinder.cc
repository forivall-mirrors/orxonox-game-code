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
 @brief Implementation of the different input handlers.
 */

#include "KeyBinder.h"

#include <fstream>
#include <string>
#include <boost/filesystem.hpp>

#include "util/Convert.h"
#include "util/Debug.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/ConfigFileManager.h"
#include "core/Core.h"
#include "InputCommands.h"
#include "InputManager.h"

namespace orxonox
{
    /**
    @brief
        Constructor that does as little as necessary.
    */
    KeyBinder::KeyBinder()
        : numberOfJoySticks_(0)
        , deriveTime_(0.0f)
    {
        mouseRelative_[0] = 0;
        mouseRelative_[1] = 0;
        mousePosition_[0] = 0;
        mousePosition_[1] = 0;

        RegisterRootObject(KeyBinder);

        // intialise all buttons and half axes to avoid creating everything with 'new'
        // keys
        for (unsigned int i = 0; i < KeyCode::numberOfKeys; i++)
        {
            std::string keyname = KeyCode::ByString[i];
            if (!keyname.empty())
                keys_[i].name_ = std::string("Key") + keyname;
            else
                keys_[i].name_ = "";
            keys_[i].paramCommandBuffer_ = &paramCommandBuffer_;
            keys_[i].groupName_ = "Keys";
        }
        // mouse buttons plus 4 mouse wheel buttons only 'generated' by KeyBinder
        const char* const mouseWheelNames[] = { "Wheel1Down", "Wheel1Up", "Wheel2Down", "Wheel2Up" };
        for (unsigned int i = 0; i < numberOfMouseButtons_; i++)
        {
            std::string nameSuffix;
            if (i < MouseButtonCode::numberOfButtons)
                nameSuffix = MouseButtonCode::ByString[i];
            else
                nameSuffix = mouseWheelNames[i - MouseButtonCode::numberOfButtons];
            mouseButtons_[i].name_ = std::string("Mouse") + nameSuffix;
            mouseButtons_[i].paramCommandBuffer_ = &paramCommandBuffer_;
            mouseButtons_[i].groupName_ = "MouseButtons";
        }
        // mouse axes
        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
        {
            mouseAxes_[i].name_ = std::string("Mouse") + MouseAxisCode::ByString[i / 2];
            if (i & 1)
                mouseAxes_[i].name_ += "Pos";
            else
                mouseAxes_[i].name_ += "Neg";
            mouseAxes_[i].paramCommandBuffer_ = &paramCommandBuffer_;
            mouseAxes_[i].groupName_ = "MouseAxes";
        }

        // Get a new ConfigFileType from the ConfigFileManager
        this->configFile_ = ConfigFileManager::getInstance().getNewConfigFileType();

        // initialise joy sticks separatly to allow for reloading
        numberOfJoySticks_ = InputManager::getInstance().numberOfJoySticks();
        initialiseJoyStickBindings();

        // collect all Buttons and HalfAxes
        compilePointerLists();

        // set them here to use allHalfAxes_
        setConfigValues();
    }

    /**
    @brief
        Destructor
    */
    KeyBinder::~KeyBinder()
    {
        // almost no destructors required because most of the arrays are static.
        clearBindings(); // does some destruction work
    }

    /**
    @brief
        Loader for the key bindings, managed by config values.
    */
    void KeyBinder::setConfigValues()
    {
        SetConfigValue(analogThreshold_, 0.05f)
            .description("Threshold for analog axes until which the state is 0.");
        SetConfigValue(bFilterAnalogNoise_, false)
            .description("Specifies whether to filter small analog values like joy stick fluctuations.");
        SetConfigValue(mouseSensitivity_, 1.0f)
            .description("Mouse sensitivity.");
        SetConfigValue(bDeriveMouseInput_, false)
            .description("Whether or not to derive moues movement for the absolute value.");
        SetConfigValue(derivePeriod_, 0.05f)
            .description("Accuracy of the mouse input deriver. The higher the more precise, but laggier.");
        SetConfigValue(mouseSensitivityDerived_, 1.0f)
            .description("Mouse sensitivity if mouse input is derived.");
        SetConfigValue(mouseWheelStepSize_, 120)
            .description("Equals one step of the mousewheel.");
        SetConfigValue(buttonThreshold_, 0.80f)
            .description("Threshold for analog axes until which the button is not pressed.")
            .callback(this, &KeyBinder::buttonThresholdChanged);
    }

    void KeyBinder::buttonThresholdChanged()
    {
        for (unsigned int i = 0; i < allHalfAxes_.size(); i++)
            if (!allHalfAxes_[i]->bButtonThresholdUser_)
                allHalfAxes_[i]->buttonThreshold_ = this->buttonThreshold_;
    }

    void KeyBinder::JoyStickDeviceNumberChanged(unsigned int value)
    {
        unsigned int oldValue = numberOfJoySticks_;
        numberOfJoySticks_ = value;

        // initialise joy stick bindings
        initialiseJoyStickBindings();

        // collect all Buttons and HalfAxes again
        compilePointerLists();

        // load the bindings if required
        if (configFile_ != ConfigFileType::NoType)
        {
            for (unsigned int iDev = oldValue; iDev < numberOfJoySticks_; ++iDev)
            {
                for (unsigned int i = 0; i < JoyStickButtonCode::numberOfButtons; ++i)
                    joyStickButtons_[iDev][i].readConfigValue(this->configFile_);
                for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; ++i)
                    joyStickAxes_[iDev][i].readConfigValue(this->configFile_);
            }
        }

        // Set the button threshold for potential new axes
        buttonThresholdChanged();
    }

    void KeyBinder::initialiseJoyStickBindings()
    {
        this->joyStickAxes_.resize(numberOfJoySticks_);
        this->joyStickButtons_.resize(numberOfJoySticks_);

        // reinitialise all joy stick binings (doesn't overwrite the old ones)
        for (unsigned int iDev = 0; iDev < numberOfJoySticks_; iDev++)
        {
            std::string deviceNumber = convertToString(iDev);
            // joy stick buttons
            for (unsigned int i = 0; i < JoyStickButtonCode::numberOfButtons; i++)
            {
                joyStickButtons_[iDev][i].name_ = std::string("JoyStick") + deviceNumber + JoyStickButtonCode::ByString[i];
                joyStickButtons_[iDev][i].paramCommandBuffer_ = &paramCommandBuffer_;
                joyStickButtons_[iDev][i].groupName_ = std::string("JoyStick") + deviceNumber + "Buttons";
            }
            // joy stick axes
            for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
            {
                joyStickAxes_[iDev][i].name_ = std::string("JoyStick") + deviceNumber + JoyStickAxisCode::ByString[i >> 1];
                if (i & 1)
                    joyStickAxes_[iDev][i].name_ += "Pos";
                else
                    joyStickAxes_[iDev][i].name_ += "Neg";
                joyStickAxes_[iDev][i].paramCommandBuffer_ = &paramCommandBuffer_;
                joyStickAxes_[iDev][i].groupName_ = std::string("JoyStick") + deviceNumber + "Axes";
            }
        }
    }

    void KeyBinder::compilePointerLists()
    {
        allButtons_.clear();
        allHalfAxes_.clear();

        // Note: Don't include the dummy keys which don't actually exist in OIS but have a number
        for (unsigned int i = 0; i < KeyCode::numberOfKeys; i++)
            if (!keys_[i].name_.empty())
                allButtons_[keys_[i].name_] = keys_ + i;
        for (unsigned int i = 0; i < numberOfMouseButtons_; i++)
            allButtons_[mouseButtons_[i].name_] = mouseButtons_ + i;
        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
        {
            allButtons_[mouseAxes_[i].name_] = mouseAxes_ + i;
            allHalfAxes_.push_back(mouseAxes_ + i);
        }
        for (unsigned int iDev = 0; iDev < numberOfJoySticks_; iDev++)
        {
            for (unsigned int i = 0; i < JoyStickButtonCode::numberOfButtons; i++)
                allButtons_[joyStickButtons_[iDev][i].name_] = &(joyStickButtons_[iDev][i]);
            for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
            {
                allButtons_[joyStickAxes_[iDev][i].name_] = &(joyStickAxes_[iDev][i]);
                allHalfAxes_.push_back(&(joyStickAxes_[iDev][i]));
            }
        }
    }

    /**
    @brief
        Loads the key and button bindings.
    @return
        True if loading succeeded.
    */
    void KeyBinder::loadBindings(const std::string& filename)
    {
        COUT(3) << "KeyBinder: Loading key bindings..." << std::endl;

        if (filename.empty())
            return;

        ConfigFileManager::getInstance().setFilename(this->configFile_, filename);

        // Parse bindings and create the ConfigValueContainers if necessary
        clearBindings();
        for (std::map<std::string, Button*>::const_iterator it = allButtons_.begin(); it != allButtons_.end(); ++it)
            it->second->readConfigValue(this->configFile_);

        COUT(3) << "KeyBinder: Loading key bindings done." << std::endl;
    }

    bool KeyBinder::setBinding(const std::string& binding, const std::string& name, bool bTemporary)
    {
        std::map<std::string, Button*>::iterator it = allButtons_.find(name);
        if (it != allButtons_.end())
        {
            if (bTemporary)
                it->second->configContainer_->tset(binding);
            else
                it->second->configContainer_->set(binding);
            it->second->configContainer_->getValue(&(it->second->bindingString_), it->second);
            return true;
        }
        else
        {
            COUT(2) << "Could not find key/button/axis with name '" << name << "'." << std::endl;
            return false;
        }
    }

    /**
    @brief
        Overwrites all bindings with ""
    */
    void KeyBinder::clearBindings()
    {
        for (std::map<std::string, Button*>::const_iterator it = allButtons_.begin(); it != allButtons_.end(); ++it)
            it->second->clear();

        for (unsigned int i = 0; i < paramCommandBuffer_.size(); i++)
            delete paramCommandBuffer_[i];
        paramCommandBuffer_.clear();
    }

    void KeyBinder::resetJoyStickAxes()
    {
        for (unsigned int iDev = 0; iDev < numberOfJoySticks_; ++iDev)
        {
            for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
            {
                joyStickAxes_[iDev][i].absVal_ = 0.0f;
                joyStickAxes_[iDev][i].relVal_ = 0.0f;
            }
        }
    }

    void KeyBinder::tickMouse(float dt)
    {
        if (bDeriveMouseInput_)
        {
            // only update when derivation dt has passed
            if (deriveTime_ > derivePeriod_)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (mouseRelative_[i] < 0)
                    {
                        mouseAxes_[2*i + 0].absVal_
                            = -mouseRelative_[i] / deriveTime_ * 0.0005 * mouseSensitivityDerived_;
                        mouseAxes_[2*i + 1].absVal_ = 0.0f;
                    }
                    else if (mouseRelative_[i] > 0)
                    {
                        mouseAxes_[2*i + 0].absVal_ = 0.0f;
                        mouseAxes_[2*i + 1].absVal_
                            =  mouseRelative_[i] / deriveTime_ * 0.0005 * mouseSensitivityDerived_;
                    }
                    else
                    {
                        mouseAxes_[2*i + 0].absVal_ = 0.0f;
                        mouseAxes_[2*i + 1].absVal_ = 0.0f;
                    }
                    mouseRelative_[i] = 0;
                    mouseAxes_[2*i + 0].hasChanged_ = true;
                    mouseAxes_[2*i + 1].hasChanged_ = true;
                }
                deriveTime_ = 0.0f;
            }
            else
                deriveTime_ += dt;
        }

        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
        {
            // Why dividing relative value by dt? The reason lies in the simple fact, that when you
            // press a button that has relative movement, that value has to be multiplied by dt to be
            // frame rate independent. This can easily (and only) be done in tickInput(float).
            // Hence we need to divide by dt here for the mouse to compensate, because the relative
            // move movements have nothing to do with dt.
            if (dt != 0.0f)
            {
                // just ignore if dt == 0.0 because we have multiplied by 0.0 anyway..
                mouseAxes_[i].relVal_ /= dt;
            }

            tickHalfAxis(mouseAxes_[i]);
        }
    }

    void KeyBinder::tickJoyStick(float dt, unsigned int joyStick)
    {
        for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
        {
            tickHalfAxis(joyStickAxes_[joyStick][i]);
        }
    }

    void KeyBinder::tickHalfAxis(HalfAxis& halfAxis)
    {
        // button mode
        // TODO: optimize out all the half axes that don't act as a button at the moment
        if (halfAxis.hasChanged_)
        {
            if (!halfAxis.pressed_ && halfAxis.absVal_ > halfAxis.buttonThreshold_)
            {
                // key pressed event
                halfAxis.pressed_ = true;
                if (halfAxis.nCommands_[KeybindMode::OnPress])
                    halfAxis.execute(KeybindMode::OnPress);
            }
            else if (halfAxis.pressed_ && halfAxis.absVal_ < halfAxis.buttonThreshold_)
            {
                // key released event
                halfAxis.pressed_ = false;
                if (halfAxis.nCommands_[KeybindMode::OnRelease])
                    halfAxis.execute(KeybindMode::OnRelease);
            }
            halfAxis.hasChanged_ = false;
        }

        if (halfAxis.pressed_)
        {
            // key held event
            if (halfAxis.nCommands_[KeybindMode::OnHold])
                halfAxis.execute(KeybindMode::OnHold);
        }

        // these are the actually useful axis bindings for analog input
        if (!bFilterAnalogNoise_ || halfAxis.relVal_ > analogThreshold_ || halfAxis.absVal_ > analogThreshold_)
        {
            halfAxis.execute();
        }
    }

    /**
    @brief
        Event handler for the mouseMoved Event.
    @param e
        Mouse state information
    */
    void KeyBinder::mouseMoved(IntVector2 abs_, IntVector2 rel_, IntVector2 clippingSize)
    {
        // y axis of mouse input is inverted
        int rel[] = { rel_.x, -rel_.y };

        if (bDeriveMouseInput_)
        {
            mouseRelative_[0] += rel[0];
            mouseRelative_[1] += rel[1];
        }
        else
        {
            for (int i = 0; i < 2; i++)
            {
                if (rel[i]) // performance opt. for the case that rel[i] == 0
                {
                    // write absolute values
                    mouseAxes_[2*i + 0].hasChanged_ = true;
                    mouseAxes_[2*i + 1].hasChanged_ = true;
                    mousePosition_[i] += rel[i];

                    // clip absolute position
                    if (mousePosition_[i] > mouseClippingSize_)
                        mousePosition_[i] =  mouseClippingSize_;
                    if (mousePosition_[i] < -mouseClippingSize_)
                        mousePosition_[i] = -mouseClippingSize_;

                    if (mousePosition_[i] < 0)
                    {
                        mouseAxes_[2*i + 0].absVal_ =  -mousePosition_[i]/(float)mouseClippingSize_ * mouseSensitivity_;
                        mouseAxes_[2*i + 1].absVal_ =  0.0f;
                    }
                    else
                    {
                        mouseAxes_[2*i + 0].absVal_ =  0.0f;
                        mouseAxes_[2*i + 1].absVal_ =   mousePosition_[i]/(float)mouseClippingSize_ * mouseSensitivity_;
                    }
                }
            }
        }

        // relative
        for (int i = 0; i < 2; i++)
        {
            if (rel[i] < 0)
                mouseAxes_[0 + 2*i].relVal_ = -((float)rel[i])/(float)mouseClippingSize_ * mouseSensitivity_;
            else
                mouseAxes_[1 + 2*i].relVal_ =  ((float)rel[i])/(float)mouseClippingSize_ * mouseSensitivity_;
        }
    }

    /**
    @brief Event handler for the mouseScrolled Event.
    @param e Mouse state information
    */
    void KeyBinder::mouseScrolled(int abs, int rel)
    {
        if (rel < 0)
            for (int i = 0; i < -rel/mouseWheelStepSize_; i++)
                mouseButtons_[8].execute(KeybindMode::OnPress, ((float)abs)/mouseWheelStepSize_);
        else
            for (int i = 0; i < rel/mouseWheelStepSize_; i++)
                mouseButtons_[9].execute(KeybindMode::OnPress, ((float)abs)/mouseWheelStepSize_);
    }

    void KeyBinder::joyStickAxisMoved(unsigned int joyStickID, unsigned int axis, float value)
    {
        int i = axis * 2;
        if (value < 0)
        {
            joyStickAxes_[joyStickID][i].absVal_ = -value;
            joyStickAxes_[joyStickID][i].relVal_ = -value;
            joyStickAxes_[joyStickID][i].hasChanged_ = true;
            if (joyStickAxes_[joyStickID][i + 1].absVal_ > 0.0f)
            {
                joyStickAxes_[joyStickID][i + 1].absVal_ = -0.0f;
                joyStickAxes_[joyStickID][i + 1].relVal_ = -0.0f;
                joyStickAxes_[joyStickID][i + 1].hasChanged_ = true;
            }
        }
        else
        {
            joyStickAxes_[joyStickID][i + 1].absVal_ = value;
            joyStickAxes_[joyStickID][i + 1].relVal_ = value;
            joyStickAxes_[joyStickID][i + 1].hasChanged_ = true;
            if (joyStickAxes_[joyStickID][i].absVal_ > 0.0f)
            {
                joyStickAxes_[joyStickID][i].absVal_ = -0.0f;
                joyStickAxes_[joyStickID][i].relVal_ = -0.0f;
                joyStickAxes_[joyStickID][i].hasChanged_ = true;
            }
        }
    }
}
