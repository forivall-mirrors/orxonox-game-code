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
    Implementation of the JoyStick wrapper class.
*/

#include "JoyStick.h"

#include <ois/OISJoyStick.h>
#include <ois/OISInputManager.h>
#include <boost/foreach.hpp>

#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "util/Convert.h"
#include "InputState.h"
#include "InputManager.h"

namespace orxonox
{
    /**
    @brief
        Helper function that loads the config value vector of one coefficient
    */
    void loadCalibration(std::vector<int>& list, const std::string& sectionName, const std::string& valueName, size_t size, int defaultValue);

    JoyStick::JoyStick(const std::vector<InputState*>& states, unsigned int id)
        : id_(id)
        , bCalibrating_(false)
        , inputStates_(states)
    {
        RegisterRootObject(JoyStick);
        this->setConfigValues();

        OIS::InputManager* system = InputManager::getInstance().getInputSystem();
        oisJoyStick_ = static_cast<OIS::JoyStick*>(system->createInputObject(OIS::OISJoyStick, true));
        oisJoyStick_->setEventCallback(this);

        idString_ = "JoyStick_";
        std::string name = oisJoyStick_->vendor();
        replaceCharacters(name, ' ', '_');
        idString_ += name + "_";
        idString_ += multi_cast<std::string>(oisJoyStick_->getNumberOfComponents(OIS::OIS_Button))  + "_";
        idString_ += multi_cast<std::string>(oisJoyStick_->getNumberOfComponents(OIS::OIS_Axis))    + "_";
        idString_ += multi_cast<std::string>(oisJoyStick_->getNumberOfComponents(OIS::OIS_Slider))  + "_";
        idString_ += multi_cast<std::string>(oisJoyStick_->getNumberOfComponents(OIS::OIS_POV));
        //idString_ += multi_cast<std::string>(oisJoyStick_->getNumberOfComponents(OIS::OIS_Vector3));

        if (InputManager::getInstance().checkJoyStickID(idString_) == false)
        {
            // Make the ID unique for this execution time.
            idString_ += "_" + multi_cast<std::string>(id_);
        }

        COUT(4) << "Created OIS joy stick with ID " << idString_ << std::endl;

        // Load calibration
        size_t axes = sliderAxes_s + static_cast<size_t>(oisJoyStick_->getNumberOfComponents(OIS::OIS_Axis));
        loadCalibration(configMinValues_,  idString_, "MinValue",  axes,  -32768);
        loadCalibration(configMaxValues_,  idString_, "MaxValue",  axes,   32768);
        loadCalibration(configZeroValues_, idString_, "ZeroValue", axes, 0);
        this->evaluateCalibration();
    }

    JoyStick::~JoyStick()
    {
        try
        {
            OIS::InputManager* system = InputManager::getInstance().getInputSystem();
            system->destroyInputObject(oisJoyStick_);
        }
        catch (...)
        {
            COUT(1) << "Joy stick destruction failed! Potential resource leak!" << std::endl;
        }
    }

    /**
    @brief
        Callback for the joy stick calibration config file. @see setConfigValues.
    */
    void JoyStick::calibrationFileCallback()
    {
        ConfigFileManager::getInstance().setFilename(ConfigFileType::JoyStickCalibration, calibrationFilename_);
    }

    /**
    @brief
        Sets the configurable values.
    */
    void JoyStick::setConfigValues()
    {
        SetConfigValue(calibrationFilename_, "joystick_calibration.ini")
            .description("Ini filename for the the joy stick calibration data.")
            .callback(this, &JoyStick::calibrationFileCallback);
    }

    void loadCalibration(std::vector<int>& list, const std::string& sectionName, const std::string& valueName, size_t size, int defaultValue)
    {
        list.resize(size);
        unsigned int configValueVectorSize = ConfigFileManager::getInstance().getVectorSize(ConfigFileType::JoyStickCalibration, sectionName, valueName);
        if (configValueVectorSize > size)
            configValueVectorSize = size;

        for (unsigned int i = 0; i < configValueVectorSize; ++i)
        {
            list[i] = multi_cast<int>(ConfigFileManager::getInstance().getValue(
                ConfigFileType::JoyStickCalibration, sectionName, valueName, i, multi_cast<std::string>(defaultValue), false));
        }

        // fill the rest with default values
        for (unsigned int i = configValueVectorSize; i < size; ++i)
        {
            list[i] = defaultValue;
        }
    }

    void JoyStick::startCalibration()
    {
        bCalibrating_ = true;

        // Set initial values
        BOOST_FOREACH(int& minVal, configMinValues_)
            minVal = INT_MAX;
        BOOST_FOREACH(int& minVal, configMaxValues_)
            minVal = INT_MIN;
        BOOST_FOREACH(int& zeroVal, configZeroValues_)
            zeroVal = 0;
    }

    void JoyStick::stopCalibration()
    {
        // Get the middle positions now
        unsigned int iAxis = 0;
        for (unsigned int i = 0; i < sliderAxes_s/2; ++i)
        {
            configZeroValues_[iAxis++] = oisJoyStick_->getJoyStickState().mSliders[i].abX;
            configZeroValues_[iAxis++] = oisJoyStick_->getJoyStickState().mSliders[i].abY;
        }
        // Note: joyStickMiddleValues_[iJoyStick] was already correctly resised in loadCalibration()
        assert(oisJoyStick_->getJoyStickState().mAxes.size() == configZeroValues_.size() - sliderAxes_s);
        for (unsigned int i = 0; i < configZeroValues_.size() - sliderAxes_s; ++i)
            configZeroValues_[iAxis++] = oisJoyStick_->getJoyStickState().mAxes[i].abs;

        for (unsigned int i = 0; i < configMinValues_.size(); ++i)
        {
            // Minimum values
            if (configMinValues_[i] == INT_MAX)
                configMinValues_[i] = -32768;
            ConfigFileManager::getInstance().setValue(ConfigFileType::JoyStickCalibration,
                idString_, "MinValue", i, multi_cast<std::string>(configMinValues_[i]), false);

            // Maximum values
            if (configMaxValues_[i] == INT_MIN)
                configMaxValues_[i] = 32767;
            ConfigFileManager::getInstance().setValue(ConfigFileType::JoyStickCalibration,
                idString_, "MaxValue", i, multi_cast<std::string>(configMaxValues_[i]), false);

            // Middle values
            ConfigFileManager::getInstance().setValue(ConfigFileType::JoyStickCalibration,
                idString_, "ZeroValue", i, multi_cast<std::string>(configZeroValues_[i]), false);
        }

        this->evaluateCalibration();

        bCalibrating_ = false;
    }

    void JoyStick::evaluateCalibration()
    {
        for (unsigned int i = 0; i < configMinValues_.size(); i++)
        {
            zeroValues_[i] = configZeroValues_[i];
            negativeCoeffs_[i] = - 1.0f / (configMinValues_[i] - configZeroValues_[i]);
            positiveCoeffs_[i] =   1.0f / (configMaxValues_[i] - configZeroValues_[i]);
        }
    }

    void JoyStick::clearBuffer()
    {
        pressedButtons_.clear();
        for (int j = 0; j < 4; ++j)
        {
            povStates_[j] = 0;
            sliderStates_[j][0] = 0;
            sliderStates_[j][1] = 0;
        }
    }


    // ###### Events ######

    void JoyStick::capture()
    {
        oisJoyStick_->capture();

        // call all the handlers for the held joy stick button events
        for (unsigned int iButton = 0; iButton < pressedButtons_.size(); iButton++)
        {
            BOOST_FOREACH(InputState* state, inputStates_)
                state->joyStickButtonHeld(id_, pressedButtons_[iButton]);
        }
    }

    bool JoyStick::buttonPressed(const OIS::JoyStickEvent &arg, int button)
    {
        // check whether the button already is in the list (can happen when focus was lost)
        unsigned int iButton = 0;
        while (iButton < pressedButtons_.size() && pressedButtons_[iButton] != button)
            iButton++;
        if (iButton == pressedButtons_.size())
            pressedButtons_.push_back(static_cast<JoyStickButtonCode::ByEnum>(button));

        BOOST_FOREACH(InputState* state, inputStates_)
            state->joyStickButtonPressed(id_, static_cast<JoyStickButtonCode::ByEnum>(button));

        return true;
    }

    bool JoyStick::buttonReleased(const OIS::JoyStickEvent &arg, int button)
    {
        // remove the button from the pressedButtons_ list
        for (unsigned int iButton = 0; iButton < pressedButtons_.size(); iButton++)
        {
            if (static_cast<int>(pressedButtons_[iButton]) == button)
            {
                pressedButtons_.erase(pressedButtons_.begin() + iButton);
                break;
            }
        }

        BOOST_FOREACH(InputState* state, inputStates_)
            state->joyStickButtonPressed(id_, static_cast<JoyStickButtonCode::ByEnum>(button));

        return true;
    }

    /**
    @brief
        Calls the states for a particular axis with our enumeration.
        Used by OIS sliders and OIS axes.
    */
    void JoyStick::fireAxis(int axis, int value)
    {
        if (bCalibrating_)
        {
            if (value < configMinValues_[axis])
                configMinValues_[axis] = value;
            if (value > configMaxValues_[axis])
                configMaxValues_[axis] = value;
        }
        else
        {
            float fValue = static_cast<float>(value - zeroValues_[axis]);
            if (fValue > 0.0f)
                fValue *= positiveCoeffs_[axis];
            else
                fValue *= negativeCoeffs_[axis];

            BOOST_FOREACH(InputState* state, inputStates_)
                state->joyStickAxisMoved(id_, axis, fValue);
        }
    }

    bool JoyStick::axisMoved(const OIS::JoyStickEvent &arg, int axis)
    {
        // keep in mind that the first 8 axes are reserved for the sliders
        this->fireAxis(axis + sliderAxes_s, arg.state.mAxes[axis].abs);

        return true;
    }

    bool JoyStick::sliderMoved(const OIS::JoyStickEvent &arg, int id)
    {
        if (sliderStates_[id][0] != arg.state.mSliders[id].abX)
            fireAxis(id * 2, arg.state.mSliders[id].abX);
        else if (sliderStates_[id][1] != arg.state.mSliders[id].abY)
            fireAxis(id * 2 + 1, arg.state.mSliders[id].abY);

        return true;
    }

    bool JoyStick::povMoved(const OIS::JoyStickEvent &arg, int id)
    {
        // translate the POV into 8 simple buttons

        int lastState = povStates_[id];
        if (lastState & OIS::Pov::North)
            buttonReleased(arg, 32 + id * 4 + 0);
        if (lastState & OIS::Pov::South)
            buttonReleased(arg, 32 + id * 4 + 1);
        if (lastState & OIS::Pov::East)
            buttonReleased(arg, 32 + id * 4 + 2);
        if (lastState & OIS::Pov::West)
            buttonReleased(arg, 32 + id * 4 + 3);

        povStates_[id] = arg.state.mPOV[id].direction;

        int currentState = povStates_[id];
        if (currentState & OIS::Pov::North)
            buttonPressed(arg, 32 + id * 4 + 0);
        if (currentState & OIS::Pov::South)
            buttonPressed(arg, 32 + id * 4 + 1);
        if (currentState & OIS::Pov::East)
            buttonPressed(arg, 32 + id * 4 + 2);
        if (currentState & OIS::Pov::West)
            buttonPressed(arg, 32 + id * 4 + 3);

        return true;
    }
}
