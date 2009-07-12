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

#ifndef _InputDevice_H__
#define _InputDevice_H__

#include "InputPrereqs.h"

#include <vector>
#include <boost/foreach.hpp>
#include <ois/OISInputManager.h>

#include "util/Debug.h"
#include "core/Clock.h"
// TODO: Try to remove this
#include "InputManager.h"
#include "InputState.h"

namespace orxonox
{
    /**
    @brief
    */
    class InputDevice
    {
        friend class InputManager;

    public:
        InputDevice(unsigned int id) : bCalibrating_(false), id_(id) { }
        virtual ~InputDevice() { }
        virtual std::string getClassName() = 0;
        virtual void update(const Clock& time) = 0;
        virtual void clearBuffers() = 0;

        void startCalibration()
        {
            bCalibrating_ = true;
            this->calibrationStarted();
        }

        void stopCalibration()
        {
            this->calibrationStopped();
            bCalibrating_ = false;
        }

        unsigned int getDeviceID() { return this->id_; }

    protected:
        virtual void calibrationStarted() { }
        virtual void calibrationStopped() { }
        bool isCalibrating() { return bCalibrating_; }

        // InputState handling
        std::vector<InputState*> inputStates_;

    private:
        InputDevice(const InputDevice& rhs);

        std::vector<InputState*>& getStateListRef()
        {
            return this->inputStates_;
        }

        bool bCalibrating_;
        const unsigned int id_;
    };

    /**
    @brief
    */
    template <class Traits>
    class InputDeviceTemplated : public InputDevice
    {
        typedef typename Traits::DeviceClass DeviceClass;
        typedef typename Traits::OISDeviceClass OISDeviceClass;
        typedef typename Traits::ButtonType ButtonType;
        typedef typename Traits::ButtonTypeParam ButtonTypeParam;
        static const OIS::Type OISDeviceValue = Traits::OISDeviceValue;

    public:
        InputDeviceTemplated(unsigned int id)
            : InputDevice(id)
        {
            OIS::InputManager* system = InputManager::getInstance().getInputSystem();
            oisDevice_ = static_cast<OISDeviceClass*>(system->createInputObject(OISDeviceValue, true));
            oisDevice_->setEventCallback(static_cast<DeviceClass*>(this));
            COUT(4) << "Instantiated a " << this->getClassName() << std::endl;
        }

        virtual ~InputDeviceTemplated()
        {
            try
            {
                InputManager::getInstance().getInputSystem()->destroyInputObject(oisDevice_);
            }
            catch (...)
            {
                COUT(1) << this->getClassName() << " destruction failed! Potential resource leak!" << std::endl;
            }
        }

        OISDeviceClass* getOISDevice()
        {
            return this->oisDevice_;
        }

        std::string getClassName()
        {
            return InputDeviceNames::values[OISDeviceValue];
        }

        void update(const Clock& time)
        {
            oisDevice_->capture();

            if (!this->isCalibrating())
            {
                // Call all the states with the held button event
                for (unsigned int iB = 0; iB < pressedButtons_.size(); ++iB)
                    for (unsigned int iS = 0; iS < inputStates_.size(); ++iS)
                        inputStates_[iS]->buttonEvent<ButtonEvent::THold, Traits>(
                            this->getDeviceID(), static_cast<DeviceClass*>(this)->getButtonEventArg(pressedButtons_[iB]));

                // Call states with device update events
                for (unsigned int i = 0; i < inputStates_.size(); ++i)
                    inputStates_[i]->update(time.getDeltaTime(), this->getDeviceID());
            }

            static_cast<DeviceClass*>(this)->updateImpl(time);
        }

        void clearBuffers()
        {
            pressedButtons_.clear();
            static_cast<DeviceClass*>(this)->clearBuffersImpl();
        }

    protected:
        void buttonPressed(ButtonTypeParam button)
        {
            // check whether the button already is in the list (can happen when focus was lost)
            unsigned int iButton = 0;
            while (iButton < pressedButtons_.size() && pressedButtons_[iButton] != button)
                iButton++;
            if (iButton == pressedButtons_.size())
                pressedButtons_.push_back(button);
            else
                return; // Button already pressed

            // Call states
            for (unsigned int i = 0; i < inputStates_.size(); ++i)
                inputStates_[i]->buttonEvent<ButtonEvent::TPress, Traits>(this->getDeviceID(), static_cast<DeviceClass*>(this)->getButtonEventArg(button));
        }

        void buttonReleased(ButtonTypeParam button)
        {
            // remove the button from the pressedButtons_ list
            for (unsigned int iButton = 0; iButton < pressedButtons_.size(); iButton++)
            {
                if (pressedButtons_[iButton] == button)
                {
                    pressedButtons_.erase(pressedButtons_.begin() + iButton);
                    break;
                }
            }

            // Call states
            for (unsigned int i = 0; i < inputStates_.size(); ++i)
                inputStates_[i]->buttonEvent<ButtonEvent::TRelease, Traits>(this->getDeviceID(), static_cast<DeviceClass*>(this)->getButtonEventArg(button));
        }

        OISDeviceClass* oisDevice_;

    private:
        void clearBuffersImpl() { } //!< Fallback dummy function for static polymorphism
        void updateImpl(const Clock& time) { } //!< Fallback dummy function for static polymorphism
        //!< Fallback dummy function for static polymorphism
        ButtonType& getButtonEventArg(ButtonType& button) { return button; }

        std::vector<ButtonType> pressedButtons_;
    };
}

#endif /* _InputDevice_H__ */
