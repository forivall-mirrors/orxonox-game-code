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

#include "KeyBinder.h"

#include <algorithm>
#include <sstream>
#include "util/Convert.h"
#include "util/Output.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/config/ConfigFile.h"
#include "core/PathConfig.h"
#include "InputCommands.h"
#include "JoyStick.h"

namespace orxonox
{
    /**
    @brief
        Constructor that does as little as necessary.
    */
    KeyBinder::KeyBinder(const std::string& filename)
        : deriveTime_(0.0f)
        , filename_(filename)
        , configFile_(NULL)
        , fallbackConfigFile_(NULL)
    {
        mouseRelative_[0] = 0;
        mouseRelative_[1] = 0;
        mousePosition_[0] = 0.0;
        mousePosition_[1] = 0.0;

        RegisterObject(KeyBinder);

        // initialise all buttons and half axes to avoid creating everything with 'new'
        // keys
        for (unsigned int i = 0; i < KeyCode::numberOfKeys; i++)
        {
            const std::string& keyname = KeyCode::ByString[i];
            if (!keyname.empty())
                keys_[i].name_ = std::string("Key") + keyname;
            else
                keys_[i].name_.clear();
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
            mouseButtons_[i].name_ = nameSuffix;
            mouseButtons_[i].paramCommandBuffer_ = &paramCommandBuffer_;
            mouseButtons_[i].groupName_ = "MouseButtons";
        }
        // mouse axes
        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
        {
            mouseAxes_[i].name_ = MouseAxisCode::ByString[i / 2];
            if (i & 1)
                mouseAxes_[i].name_ += "Pos";
            else
                mouseAxes_[i].name_ += "Neg";
            mouseAxes_[i].paramCommandBuffer_ = &paramCommandBuffer_;
            mouseAxes_[i].groupName_ = "MouseAxes";
        }

        // initialise joy sticks separatly to allow for reloading
        this->JoyStickQuantityChanged(this->getJoyStickList());

        // set them here to use allHalfAxes_
        setConfigValues();

        // Load the bindings if filename was given
        if (!this->filename_.empty())
            this->loadBindings();
    }

    /**
    @brief
        Destructor
    */
    KeyBinder::~KeyBinder()
    {
        // almost no destructors required because most of the arrays are static.
        clearBindings(); // does some destruction work
        if (this->configFile_)
            delete this->configFile_;
        if (this->fallbackConfigFile_)
            delete this->fallbackConfigFile_;
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
        SetConfigValue(mouseSensitivity_, 3.0f)
            .description("Mouse sensitivity.");
        this->totalMouseSensitivity_ = this->mouseSensitivity_ / this->mouseClippingSize_;
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

    void KeyBinder::JoyStickQuantityChanged(const std::vector<JoyStick*>& joyStickList)
    {
        unsigned int oldValue = joySticks_.size();
        joySticks_ = joyStickList;

        // initialise joy stick bindings
        initialiseJoyStickBindings();

        // collect all Buttons and HalfAxes again
        compilePointerLists();

        // load the bindings if required
        if (configFile_ != NULL)
        {
            for (unsigned int iDev = oldValue; iDev < joySticks_.size(); ++iDev)
            {
                for (unsigned int i = 0; i < JoyStickButtonCode::numberOfButtons; ++i)
                    (*joyStickButtons_[iDev])[i].readBinding(this->configFile_, this->fallbackConfigFile_);
                for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; ++i)
                    (*joyStickAxes_[iDev])[i].readBinding(this->configFile_, this->fallbackConfigFile_);
            }
        }

        // Set the button threshold for potential new axes
        buttonThresholdChanged();
    }

    void KeyBinder::initialiseJoyStickBindings()
    {
        while (joyStickAxes_.size() < joySticks_.size())
            joyStickAxes_.push_back(shared_ptr<JoyStickAxisVector>(new JoyStickAxisVector()));
        while (joyStickButtons_.size() < joySticks_.size())
            joyStickButtons_.push_back(shared_ptr<JoyStickButtonVector>(new JoyStickButtonVector()));
        // For the case the new size is smaller
        this->joyStickAxes_.resize(joySticks_.size());
        this->joyStickButtons_.resize(joySticks_.size());

        // reinitialise all joy stick bindings (doesn't overwrite the old ones)
        for (unsigned int iDev = 0; iDev < joySticks_.size(); iDev++)
        {
            const std::string& deviceName = joySticks_[iDev]->getDeviceName();
            // joy stick buttons
            for (unsigned int i = 0; i < JoyStickButtonCode::numberOfButtons; i++)
            {
                (*joyStickButtons_[iDev])[i].name_ = JoyStickButtonCode::ByString[i];
                (*joyStickButtons_[iDev])[i].paramCommandBuffer_ = &paramCommandBuffer_;
                (*joyStickButtons_[iDev])[i].groupName_ = "JoyStickButtons_" + deviceName;
            }
            // joy stick axes
            for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
            {
                (*joyStickAxes_[iDev])[i].name_ = JoyStickAxisCode::ByString[i / 2];
                if (i & 1)
                    (*joyStickAxes_[iDev])[i].name_ += "Pos";
                else
                    (*joyStickAxes_[iDev])[i].name_ += "Neg";
                (*joyStickAxes_[iDev])[i].paramCommandBuffer_ = &paramCommandBuffer_;
                (*joyStickAxes_[iDev])[i].groupName_ = "JoyStickAxes_" + deviceName;
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
                allButtons_[keys_[i].groupName_ + '.' + keys_[i].name_] = keys_ + i;
        for (unsigned int i = 0; i < numberOfMouseButtons_; i++)
            allButtons_[mouseButtons_[i].groupName_ + '.' + mouseButtons_[i].name_] = mouseButtons_ + i;
        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
        {
            allButtons_[mouseAxes_[i].groupName_ + '.' + mouseAxes_[i].name_] = mouseAxes_ + i;
            allHalfAxes_.push_back(mouseAxes_ + i);
        }
        for (unsigned int iDev = 0; iDev < joySticks_.size(); iDev++)
        {
            for (unsigned int i = 0; i < JoyStickButtonCode::numberOfButtons; i++)
                allButtons_[(*joyStickButtons_[iDev])[i].groupName_ + '.' + (*joyStickButtons_[iDev])[i].name_] = &((*joyStickButtons_[iDev])[i]);
            for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
            {
                allButtons_[(*joyStickAxes_[iDev])[i].groupName_ + '.' + (*joyStickAxes_[iDev])[i].name_] = &((*joyStickAxes_[iDev])[i]);
                allHalfAxes_.push_back(&((*joyStickAxes_[iDev])[i]));
            }
        }
    }

    /**
    @brief
        Loads the key and button bindings.
    */
    void KeyBinder::loadBindings()
    {
        orxout(internal_info, context::input) << "KeyBinder: Loading key bindings..." << endl;

        this->configFile_ = new ConfigFile(this->filename_, !PathConfig::buildDirectoryRun());
        this->configFile_->load();

        if (PathConfig::buildDirectoryRun())
        {
            // Dev users should have combined key bindings files
            std::string defaultFilepath(PathConfig::getDataPathString() + ConfigFile::DEFAULT_CONFIG_FOLDER + '/' + this->filename_);
            std::ifstream file(defaultFilepath.c_str());
            if (file.is_open())
            {
                file.close();
                // Open the default file for later use (use absolute path!)
                this->fallbackConfigFile_ = new ConfigFile(defaultFilepath, false);
                this->fallbackConfigFile_->load();
            }
        }

        // Parse bindings and create the ConfigValueContainers if necessary
        for (std::map<std::string, Button*>::const_iterator it = allButtons_.begin(); it != allButtons_.end(); ++it)
        {
            it->second->readBinding(this->configFile_, this->fallbackConfigFile_);
            addButtonToCommand(it->second->bindingString_, it->second);
        }

        orxout(internal_info, context::input) << "KeyBinder: Loading key bindings done." << endl;
    }

    bool KeyBinder::setBinding(const std::string& binding, const std::string& name, bool bTemporary)
    {
        std::map<std::string, Button*>::iterator it = allButtons_.find(name);
        if (it != allButtons_.end())
        {
            addButtonToCommand(binding, it->second);
            std::string str = binding;
            if (PathConfig::buildDirectoryRun() && binding.empty())
                str = "NoBinding";
            it->second->setBinding(this->configFile_, this->fallbackConfigFile_, binding, bTemporary);
            return true;
        }
        else
        {
            orxout(internal_warning, context::input) << "Could not find key/button/axis with name '" << name << "'." << endl;
            return false;
        }
    }

     void KeyBinder::addButtonToCommand(const std::string& command, Button* button)
     {
        std::ostringstream stream;
        stream << button->groupName_  << '.' << button->name_;

        std::vector<std::string>& oldKeynames = this->allCommands_[button->bindingString_];
        std::vector<std::string>::iterator it = std::find(oldKeynames.begin(), oldKeynames.end(), stream.str());
        if (it != oldKeynames.end())
            oldKeynames.erase(it);

        if (!command.empty())
        {
            std::vector<std::string>& keynames = this->allCommands_[command];
            if (std::find(keynames.begin(), keynames.end(), stream.str()) == keynames.end())
                this->allCommands_[command].push_back(stream.str());
        }
     }

    /**
    @brief
        Return the first key name for a specific command
    */
    const std::string& KeyBinder::getBinding(const std::string& commandName)
    {
        if (this->allCommands_.find(commandName) != this->allCommands_.end())
        {
            std::vector<std::string>& keynames = this->allCommands_[commandName];
            return keynames.front();
        }

        return BLANKSTRING;
    }

    /**
    @brief
        Return the key name for a specific command at a given index.
    @param commandName
        The command name the key name is returned for.
    @param index
        The index at which the key name is returned for.
    */
    const std::string& KeyBinder::getBinding(const std::string& commandName, unsigned int index)
    {
        if (this->allCommands_.find(commandName) != this->allCommands_.end())
        {
            std::vector<std::string>& keynames = this->allCommands_[commandName];
            if (index < keynames.size())
                return keynames[index];

            return BLANKSTRING;
        }

        return BLANKSTRING;
    }

    /**
    @brief
        Get the number of different key bindings of a specific command.
    @param commandName
        The command.
    */
    unsigned int KeyBinder::getNumberOfBindings(const std::string& commandName)
    {
        if (this->allCommands_.find(commandName) != this->allCommands_.end())
        {
            std::vector<std::string>& keynames = this->allCommands_[commandName];
            return keynames.size();
        }

        return 0;
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

    /**
        @brief Changes the keybind mode of a given console command.
    */
    void KeyBinder::changeMode(ConsoleCommand* command, KeybindMode::Value new_mode)
    {
        // iterate over all buttons
        for (std::map<std::string, Button*>::iterator it = this->allButtons_.begin(); it != this->allButtons_.end(); ++it)
        {
            Button* button = it->second;

            // iterate over all modes
            for (int mode_index = 0; mode_index < 3; ++mode_index)
            {
                if (mode_index == new_mode) // skip commands that are already in the desired mode
                    continue;

                // iterate over all commands of the given mode at the given button
                for (size_t command_index = 0; command_index < button->nCommands_[mode_index]; ++command_index)
                {
                    if (button->commands_[mode_index][command_index]->hasFixedKeybindMode())
                        continue;

                    CommandEvaluation* evaluation = button->commands_[mode_index][command_index]->getEvaluation();

                    // compare the command
                    if (evaluation && evaluation->getConsoleCommand() == command)
                    {
                        // increase array of new mode
                        BaseCommand** array_new_mode = new BaseCommand*[button->nCommands_[new_mode] + 1];
                        // copy array content
                        for (size_t c = 0; c < button->nCommands_[new_mode]; ++c)
                            array_new_mode[c] = button->commands_[new_mode][c];
                        // insert changed command at the end
                        array_new_mode[button->nCommands_[new_mode]] = button->commands_[mode_index][command_index];
                        // delete old array
                        delete[] button->commands_[new_mode];
                        // assign new array
                        button->commands_[new_mode] = array_new_mode;
                        // increase counter
                        button->nCommands_[new_mode]++;

                        // erase command from old array
                        for (size_t c = command_index; c < button->nCommands_[mode_index] - 1; ++c)
                            button->commands_[mode_index][c] = button->commands_[mode_index][c + 1];
                        // decrease counter
                        button->nCommands_[mode_index]--;
                        // old array would not get deleted if nCommands_ is now 0
                        // otherwise: nobody cares about an array that is one element too large - nCommands_ defines the size
                        if (button->nCommands_[mode_index] == 0)
                        {
                            delete[] button->commands_[mode_index];
                            button->commands_[mode_index] = 0;
                        }

                        // decrement the index since we shifted the array and continue searching for more occurrences of the command
                        command_index--;
                    }
                }
            }
        }
    }

    void KeyBinder::resetJoyStickAxes()
    {
        for (unsigned int iDev = 0; iDev < joySticks_.size(); ++iDev)
            for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
                (*joyStickAxes_[iDev])[i].reset();
    }

    /**
        @brief Sets the position of the mouse back to 0/0.
    */
    void KeyBinder::resetMouseAxes()
    {
        this->mousePosition_[0] = 0.0f;
        this->mousePosition_[1] = 0.0f;

        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
            mouseAxes_[i].reset();
    }

    void KeyBinder::mouseUpdated(float dt)
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
                            = -mouseRelative_[i] / deriveTime_ * 0.0005f * mouseSensitivityDerived_;
                        mouseAxes_[2*i + 1].absVal_ = 0.0f;
                    }
                    else if (mouseRelative_[i] > 0)
                    {
                        mouseAxes_[2*i + 0].absVal_ = 0.0f;
                        mouseAxes_[2*i + 1].absVal_
                            =  mouseRelative_[i] / deriveTime_ * 0.0005f * mouseSensitivityDerived_;
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
            // frame rate independent. This can easily (and only) be done in updateInput(float).
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

    void KeyBinder::joyStickUpdated(unsigned int joyStick, float dt)
    {
        for (unsigned int i = 0; i < JoyStickAxisCode::numberOfAxes * 2; i++)
        {
            tickHalfAxis((*joyStickAxes_[joyStick])[i]);
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
        halfAxis.execute();
    }

    /**
    @brief
        Event handler for the mouseMoved Event.
    @param abs_
        The absolute position of the mouse
    @param rel_
        The relative movement of the mouse
    @param clippingSize
        Mouse screen area in pixels (usually 1024x1024)
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
                    mousePosition_[i] += rel[i] * totalMouseSensitivity_;

                    // clip absolute position
                    if (mousePosition_[i] > 1.0)
                        mousePosition_[i] =  1.0;
                    if (mousePosition_[i] < -1.0)
                        mousePosition_[i] = -1.0;

                    if (mousePosition_[i] < 0.0)
                    {
                        mouseAxes_[2*i + 0].absVal_ = -mousePosition_[i];
                        mouseAxes_[2*i + 1].absVal_ = 0.0f;
                    }
                    else
                    {
                        mouseAxes_[2*i + 0].absVal_ = 0.0f;
                        mouseAxes_[2*i + 1].absVal_ =  mousePosition_[i];
                    }
                }
            }
        }

        // relative
        for (int i = 0; i < 2; i++)
        {
            if (rel[i] < 0)
                mouseAxes_[0 + 2*i].relVal_ = -rel[i] * totalMouseSensitivity_;
            else
                mouseAxes_[1 + 2*i].relVal_ =  rel[i] * totalMouseSensitivity_;
        }
    }

    /**
    @brief Event handler for the mouseScrolled Event.
    @param abs The absolute position of the scroll wheel
    @param rel The relative movement of the scroll wheel
    */
    void KeyBinder::mouseScrolled(int abs, int rel)
    {
        if (rel < 0)
            for (int i = 0; i < -rel/mouseWheelStepSize_; i++)
                mouseButtons_[8].execute(KeybindMode::OnPress, static_cast<float>(abs)/mouseWheelStepSize_);
        else
            for (int i = 0; i < rel/mouseWheelStepSize_; i++)
                mouseButtons_[9].execute(KeybindMode::OnPress, static_cast<float>(abs)/mouseWheelStepSize_);
    }

    void KeyBinder::axisMoved(unsigned int device, unsigned int axisID, float value)
    {
        // Filter analog noise
        if (this->bFilterAnalogNoise_ && std::abs(value) < this->analogThreshold_)
            value = 0.0;
        int i = axisID * 2;
        JoyStickAxisVector& axis = *joyStickAxes_[device];
        if (value < 0)
        {
            axis[i].absVal_ = -value;
            axis[i].relVal_ = -value;
            axis[i].hasChanged_ = true;
            if (axis[i + 1].absVal_ > 0.0f)
            {
                axis[i + 1].absVal_ = -0.0f;
                axis[i + 1].relVal_ = -0.0f;
                axis[i + 1].hasChanged_ = true;
            }
        }
        else
        {
            axis[i + 1].absVal_ = value;
            axis[i + 1].relVal_ = value;
            axis[i + 1].hasChanged_ = true;
            if (axis[i].absVal_ > 0.0f)
            {
                axis[i].absVal_ = -0.0f;
                axis[i].relVal_ = -0.0f;
                axis[i].hasChanged_ = true;
            }
        }
    }
}
