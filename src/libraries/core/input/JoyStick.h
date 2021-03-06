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

#ifndef _Core_JoyStick_H__
#define _Core_JoyStick_H__

#include "InputPrereqs.h"

#include <string>
#include <vector>
#include <ois/OISJoyStick.h>
#include "InputDevice.h"
#include "core/config/Configurable.h"

namespace orxonox
{
    //! %Template parameter collection for the base class
    struct JoyStickTraits
    {
        typedef JoyStick DeviceClass;
        typedef OIS::JoyStick OISDeviceClass;
        typedef JoyStickButtonCode::ByEnum ButtonType;
        typedef JoyStickButtonCode::ByEnum ButtonTypeParam;
        static const OIS::Type OISDeviceValue = OIS::OISJoyStick;
    };

    /**
    @brief
        Wraps around an OIS::JoyStick and forwards the input events to
        a list of input states.

        The class also supports joy stick calibration and stores the values
        in an ini-file.
    */
    class _CoreExport JoyStick
        : public Configurable
        , public InputDeviceTemplated<JoyStickTraits>
        , public OIS::JoyStickListener
    {
        friend class InputDeviceTemplated<JoyStickTraits>;
        //! Super class alias
        typedef InputDeviceTemplated<JoyStickTraits> super;

    public:
        //! Assigns a generated ID string and loads the calibration (if present)
        JoyStick(unsigned int id, OIS::InputManager* oisInputManager);
        ~JoyStick() { }
        void setConfigValues();

        //! Returns the name generated from the number of knobs and the device name
        const std::string& getDeviceName() const { return this->deviceName_; }

    private:
        void calibrationStarted();
        void calibrationStopped();
        void evaluateCalibration();

        void clearBuffersImpl();
        void calibrationFileCallback();
        void fireAxis(int axis, int value);

        //! OIS event handler
        bool buttonPressed (const OIS::JoyStickEvent &arg, int button)
        {
            super::buttonPressed(static_cast<JoyStickButtonCode::ByEnum>(button));
            return true;
        }

        //! OIS event handler
        bool buttonReleased(const OIS::JoyStickEvent &arg, int button)
        {
            super::buttonReleased(static_cast<JoyStickButtonCode::ByEnum>(button));
            return true;
        }

        bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);
        bool sliderMoved   (const OIS::JoyStickEvent &arg, int id);
        bool povMoved      (const OIS::JoyStickEvent &arg, int id);
        //! OIS event handler (don't remove that because of OIS version issues!)
        bool vector3Moved  (const OIS::JoyStickEvent &arg, int id) { return true; }

        //! Returns the class name as string
        static std::string getClassNameImpl() { return "JoyStick"; }

        std::string deviceName_;              //!< Name generated by the number of knobs and the device name
        int povStates_[4];                    //!< Internal states for the POVs
        int sliderStates_[4][2];              //!< Internal states for the Sliders (each slider has X and Y!)

        // calibration
        int zeroValues_[24];                  //!< Axes values when the knob is in the middle
        float positiveCoeffs_[24];            //!< Maps the negative part of an axis to a 0.0 to 1.0 floating range
        float negativeCoeffs_[24];            //!< Maps the positive part of an axis to a 0.0 to 1.0 floating range

        std::vector<int> configZeroValues_;   //!< Config file stored axis values when the knob is in the middle
        std::vector<int> configMinValues_;    //!< Config file stored minimum axis values
        std::vector<int> configMaxValues_;    //!< Config file stored maximum axis values

        // ConfigValues
        std::string calibrationFilename_;     //!< Joy stick calibration ini filename

        //! Contains a list of all names to avoid duplicates
        static std::vector<std::string> deviceNames_s;

        //!< Maximum number of slider axes
        static const unsigned int sliderAxes_s = 8;
    };
}

#endif /* _Core_JoyStick_H__ */
