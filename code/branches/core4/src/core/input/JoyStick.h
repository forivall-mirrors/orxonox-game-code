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
*/

#ifndef _JoyStick_H__
#define _JoyStick_H__

#include "core/CorePrereqs.h"

#include <string>
#include <vector>
#include "InputInterfaces.h"

namespace orxonox
{
    class _CoreExport JoyStick : public OrxonoxClass, public OIS::JoyStickListener
    {
    private:
        struct JoyStickCalibration
        {
        };

    public:
        JoyStick(const std::vector<InputState*>& states, unsigned int id);
        ~JoyStick();

        void setConfigValues();

        OIS::JoyStick* getOISJoyStick() { return this->oisJoyStick_; }
        const std::string& getIDString() const { return this->idString_; }

        void startCalibration();
        void stopCalibration();

        void capture();
        void clearBuffer();

    private:
        void calibrationFileCallback();
        void evaluateCalibration();
        void fireAxis(int axis, int value);

        bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
        bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
        bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);
        bool sliderMoved   (const OIS::JoyStickEvent &arg, int id);
        bool povMoved      (const OIS::JoyStickEvent &arg, int id);
        // don't remove that! Or else add OIS as dependency library to orxonox (it actually is..)
        bool vector3Moved  (const OIS::JoyStickEvent &arg, int id) { return true; }

        static const unsigned int sliderAxes_s = 8;

        OIS::JoyStick* oisJoyStick_;
        const unsigned int id_;
        std::string idString_;

        // calibration
        bool bCalibrating_;
        int zeroValues_[24];
        float positiveCoeffs_[24];
        float negativeCoeffs_[24];

        std::vector<int> configMinValues_;
        std::vector<int> configMaxValues_;
        std::vector<int> configZeroValues_;

        int povStates_[4];
        int sliderStates_[4][2];
        std::vector<JoyStickButtonCode::ByEnum> pressedButtons_;

        // InputState handling
        const std::vector<InputState*>& inputStates_;

        // ConfigValues
        std::string calibrationFilename_;  //!< Joy stick calibration ini filename
    };
}

#endif /* _JoyStick_H__ */
