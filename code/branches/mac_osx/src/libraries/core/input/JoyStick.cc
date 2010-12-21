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

#include "JoyStick.h"

#include <climits>
#include <ois/OISJoyStick.h>
#include <boost/foreach.hpp>

#include "util/StringUtils.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "util/Convert.h"
#include "InputState.h"

namespace orxonox
{
    //! Helper function that loads the config value vector of one coefficient
    void loadCalibration(std::vector<int>& list, const std::string& sectionName, const std::string& valueName, size_t size, int defaultValue);

    std::vector<std::string> JoyStick::deviceNames_s;

    JoyStick::JoyStick(unsigned int id, OIS::InputManager* oisInputManager)
        : super(id, oisInputManager)
    {
        RegisterRootObject(JoyStick);
        this->setConfigValues();
        // Initialise POV and Slider states
        this->clearBuffersImpl();

        // Generate unique name
        if (oisDevice_->vendor().empty())
            deviceName_ = "Unknown_";
        else
        {
            std::string name = oisDevice_->vendor();
            replaceCharacters(name, ' ', '_');
            deviceName_ = name + '_';
        }
        deviceName_ += multi_cast<std::string>(oisDevice_->getNumberOfComponents(OIS::OIS_Button))  + '_';
        deviceName_ += multi_cast<std::string>(oisDevice_->getNumberOfComponents(OIS::OIS_Axis))    + '_';
        deviceName_ += multi_cast<std::string>(oisDevice_->getNumberOfComponents(OIS::OIS_Slider))  + '_';
        deviceName_ += multi_cast<std::string>(oisDevice_->getNumberOfComponents(OIS::OIS_POV));
        //deviceName_ += multi_cast<std::string>(oisDevice_->getNumberOfComponents(OIS::OIS_Vector3));

        BOOST_FOREACH(std::string& idString, deviceNames_s)
        {
            if (deviceName_ == idString)
            {
                // Make the ID unique for this execution time.
                deviceName_ += '_' + multi_cast<std::string>(this->getDeviceName());
                break;
            }
        }

        COUT(4) << "Created OIS joy stick with ID " << deviceName_ << std::endl;

        // Load calibration
        size_t axes = sliderAxes_s + static_cast<size_t>(oisDevice_->getNumberOfComponents(OIS::OIS_Axis));
        loadCalibration(configMinValues_,  deviceName_, "MinValue",  axes,  -32768);
        loadCalibration(configMaxValues_,  deviceName_, "MaxValue",  axes,   32768);
        loadCalibration(configZeroValues_, deviceName_, "ZeroValue", axes, 0);
        this->evaluateCalibration();
    }

    //! Callback for the joy stick calibration config file.
    void JoyStick::calibrationFileCallback()
    {
        ConfigFileManager::getInstance().setFilename(ConfigFileType::JoyStickCalibration, calibrationFilename_);
    }

    void JoyStick::setConfigValues()
    {
        SetConfigValue(calibrationFilename_, "joystick_calibration.ini")
            .description("Ini filename for the the joy stick calibration data.")
            .callback(this, &JoyStick::calibrationFileCallback);
    }

    void loadCalibration(std::vector<int>& list, const std::string& sectionName, const std::string& valueName, size_t size, int defaultValue)
    {
        list.resize(size);
        unsigned int configValueVectorSize = ConfigFileManager::getInstance().getConfigFile(ConfigFileType::JoyStickCalibration)->getVectorSize(sectionName, valueName);
        if (configValueVectorSize > size)
            configValueVectorSize = size;

        for (unsigned int i = 0; i < configValueVectorSize; ++i)
        {
            list[i] = multi_cast<int>(ConfigFileManager::getInstance().getConfigFile(ConfigFileType::JoyStickCalibration)
                ->getOrCreateValue(sectionName, valueName, i, multi_cast<std::string>(defaultValue), false));
        }

        // fill the rest with default values
        for (unsigned int i = configValueVectorSize; i < size; ++i)
            list[i] = defaultValue;
    }

    //! Called by InputDevice when calibration mode has started
    void JoyStick::calibrationStarted()
    {
        // Set initial values
        BOOST_FOREACH(int& minVal, configMinValues_)
            minVal = INT_MAX;
        BOOST_FOREACH(int& minVal, configMaxValues_)
            minVal = INT_MIN;
        BOOST_FOREACH(int& zeroVal, configZeroValues_)
            zeroVal = 0;
    }

    //! Called by InputDevice when calibration mode has stopped
    void JoyStick::calibrationStopped()
    {
        // Get the middle positions now
        unsigned int iAxis = 0;
        for (unsigned int i = 0; i < sliderAxes_s/2; ++i)
        {
            configZeroValues_[iAxis++] = oisDevice_->getJoyStickState().mSliders[i].abX;
            configZeroValues_[iAxis++] = oisDevice_->getJoyStickState().mSliders[i].abY;
        }
        // Note: joyStickZeroValues_[iJoyStick] was already correctly resised in loadCalibration()
        assert(oisDevice_->getJoyStickState().mAxes.size() == configZeroValues_.size() - sliderAxes_s);
        for (unsigned int i = 0; i < configZeroValues_.size() - sliderAxes_s; ++i)
            configZeroValues_[iAxis++] = oisDevice_->getJoyStickState().mAxes[i].abs;

        for (unsigned int i = 0; i < configMinValues_.size(); ++i)
        {
            // Minimum values
            if (configMinValues_[i] == INT_MAX)
                configMinValues_[i] = -32768;
            ConfigFileManager::getInstance().getConfigFile(ConfigFileType::JoyStickCalibration)
                ->getOrCreateValue(deviceName_, "MinValue", i, multi_cast<std::string>(configMinValues_[i]), false);

            // Maximum values
            if (configMaxValues_[i] == INT_MIN)
                configMaxValues_[i] = 32767;
            ConfigFileManager::getInstance().getConfigFile(ConfigFileType::JoyStickCalibration)
                ->getOrCreateValue(deviceName_, "MaxValue", i, multi_cast<std::string>(configMaxValues_[i]), false);

            // Middle values
            ConfigFileManager::getInstance().getConfigFile(ConfigFileType::JoyStickCalibration)
                ->getOrCreateValue(deviceName_, "ZeroValue", i, multi_cast<std::string>(configZeroValues_[i]), false);
        }

        this->evaluateCalibration();
    }

    //! Evaluates the accumulated values during calibration
    void JoyStick::evaluateCalibration()
    {
        for (unsigned int i = 0; i < configMinValues_.size(); i++)
        {
            zeroValues_[i] = configZeroValues_[i];
            negativeCoeffs_[i] = - 1.0f / (configMinValues_[i] - configZeroValues_[i]);
            positiveCoeffs_[i] =   1.0f / (configMaxValues_[i] - configZeroValues_[i]);
        }
    }

    //! Resets the pov states
    void JoyStick::clearBuffersImpl()
    {
        for (int j = 0; j < 4; ++j)
            povStates_[j] = 0;
    }

    //! Generic method to forward axis events
    void JoyStick::fireAxis(int axis, int value)
    {
        if (this->isCalibrating())
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
                state->joyStickAxisMoved(this->getDeviceID(), axis, fValue);
        }
    }

    //! OIS joy stick axis event handler
    bool JoyStick::axisMoved(const OIS::JoyStickEvent &arg, int axis)
    {
        // keep in mind that the first 8 axes are reserved for the sliders
        this->fireAxis(axis + sliderAxes_s, arg.state.mAxes[axis].abs);

        return true;
    }

    //! A Slider always has an X and an Y direction!
    bool JoyStick::sliderMoved(const OIS::JoyStickEvent &arg, int id)
    {
        if (sliderStates_[id][0] != arg.state.mSliders[id].abX)
            fireAxis(id * 2, arg.state.mSliders[id].abX);
        else if (sliderStates_[id][1] != arg.state.mSliders[id].abY)
            fireAxis(id * 2 + 1, arg.state.mSliders[id].abY);

        return true;
    }

    //! A POV is the big button that can point in all directions (but only in one at once)
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
