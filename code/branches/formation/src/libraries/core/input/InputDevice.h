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
    Implementation of InputDevice and InputDeviceTemplated
*/

#ifndef _InputDevice_H__
#define _InputDevice_H__

#include "InputPrereqs.h"

#include <vector>
#include <ois/OISInputManager.h>
#include <ois/OISException.h>

#include "util/Clock.h"
#include "util/Output.h"
#include "util/Exception.h"
#include "InputState.h"

namespace orxonox
{
    /**
    @brief
        Abstract base class for all input devices (mouse, keyboard and joy sticks).

        It provides common virtual functions to be used by the InputManager.
    */
    class InputDevice
    {
    public:
        //! Only resets the members
        InputDevice(unsigned int id) : bCalibrating_(false), deviceID_(id) { }
        virtual ~InputDevice() { }
        //! Returns the device class (derived) name as string
        virtual std::string getClassName() const = 0;
        //! Updates the device which should in turn distribute events
        virtual void update(const Clock& time) = 0;
        //! Clear all button related buffers
        virtual void clearBuffers() = 0;

        //! Start calibrating (only useful for joy sticks)
        void startCalibration()
        {
            bCalibrating_ = true;
            this->calibrationStarted();
        }

        //! Stop calibrating and evaluate the data (only useful for joy sticks)
        void stopCalibration()
        {
            this->calibrationStopped();
            bCalibrating_ = false;
        }

        //! Returns a reference to the internal input state vector. Use with care!
        std::vector<InputState*>& getStateListRef() { return this->inputStates_; }
        //! Returns the ID of the device (the same as in InputDeviceEnumerator for mouse and keyboard)
        unsigned int getDeviceID() const { return this->deviceID_; }
        //! Tells whether the device is in calibration mode
        bool isCalibrating() const { return bCalibrating_; }

    protected:
        //! To be ovrridden by the subclass
        virtual void calibrationStarted() { }
        //! To be ovrridden by the subclass
        virtual void calibrationStopped() { }

        //! List of all input states that receive events from this device
        std::vector<InputState*> inputStates_;

    private:
        InputDevice(const InputDevice& rhs); //!< Don't use!

        bool bCalibrating_;                  //!< Whether the device is in calibration mode
        const unsigned int deviceID_;        //!< ID of the device (the same as in InputDeviceEnumerator for mouse and keyboard)
    };

    /**
    @brief
        Heavily templated base class for all three input devices.

        The purpose of this class is not to provide an interface but rather
        to reduce code redundancy. This concerns device creation and destruction
        as well as common code for button events (press, release, hold).

        In order to derive from this class you have to supply it with a struct
        as template parameter that contains the necessary type traits.
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
        //! Creates the OIS device
        InputDeviceTemplated(unsigned int id, OIS::InputManager* oisInputManager)
            : InputDevice(id)
            , oisInputManager_(oisInputManager)
        {
            oisDevice_ = static_cast<OISDeviceClass*>(oisInputManager_->createInputObject(OISDeviceValue, true));
            // Note: after the static_cast here, the cast this pointer becomes
            //       invalid right until the subclass has been constructed!
            oisDevice_->setEventCallback(static_cast<DeviceClass*>(this));
            orxout(verbose, context::input) << "Instantiated a " << this->getClassName() << endl;
        }

        //! Destroys the OIS device
        virtual ~InputDeviceTemplated()
        {
            try
            {
                oisInputManager_->destroyInputObject(oisDevice_);
            }
            catch (const OIS::Exception& ex)
            {
                orxout(internal_error, context::input) << this->getClassName() << " destruction failed: " << ex.eText << '\n'
                                                       << "Potential resource leak!" << endl;
            }
        }

        //! Captures OIS events (which then get distributed to the derived class) and creates the button held events
        void update(const Clock& time)
        {
            oisDevice_->capture();

            // Call all the states with the held button event
            for (unsigned int iB = 0; iB < pressedButtons_.size(); ++iB)
                for (unsigned int iS = 0; iS < inputStates_.size(); ++iS)
                    inputStates_[iS]->buttonEvent<ButtonEvent::THold, typename Traits::ButtonTypeParam>(
                        this->getDeviceID(), static_cast<DeviceClass*>(this)->getButtonEventArg(pressedButtons_[iB]));

            // Call states with device update events
            for (unsigned int i = 0; i < inputStates_.size(); ++i)
                inputStates_[i]->update(time.getDeltaTime(), this->getDeviceID());

            static_cast<DeviceClass*>(this)->updateImpl(time);
        }

        //! Clears the list of pressed buttons and calls the derived class's method
        void clearBuffers()
        {
            pressedButtons_.clear();
            static_cast<DeviceClass*>(this)->clearBuffersImpl();
        }

        // Returns a pointer to the OIS device
        OISDeviceClass* getOISDevice()   { return this->oisDevice_; }
        // Returns the name of the derived class as string
        std::string getClassName() const { return DeviceClass::getClassNameImpl(); }

    protected:
        //! Common code for all button pressed events (updates pressed buttons list and calls the input states)
        ORX_FORCEINLINE void buttonPressed(ButtonTypeParam button)
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
                inputStates_[i]->buttonEvent<ButtonEvent::TPress, typename Traits::ButtonTypeParam>(this->getDeviceID(), static_cast<DeviceClass*>(this)->getButtonEventArg(button));
        }

        //! Common code for all button released events (updates pressed buttons list and calls the input states)
        ORX_FORCEINLINE void buttonReleased(ButtonTypeParam button)
        {
            // remove the button from the pressedButtons_ list
            bool found = false;
            for (unsigned int iButton = 0; iButton < pressedButtons_.size(); iButton++)
            {
                if (pressedButtons_[iButton] == button)
                {
                    pressedButtons_.erase(pressedButtons_.begin() + iButton);
                    found = true;
                    break;
                }
            }
            if (!found)
                return; // We ignore release strokes when the press was not captured

            // Call states
            for (unsigned int i = 0; i < inputStates_.size(); ++i)
                inputStates_[i]->buttonEvent<ButtonEvent::TRelease, typename Traits::ButtonTypeParam>(this->getDeviceID(), static_cast<DeviceClass*>(this)->getButtonEventArg(button));
        }

        //! Managed pointer to the OIS device
        OISDeviceClass* oisDevice_;

    private:
        //!< Fallback dummy function for static polymorphism
        void clearBuffersImpl() { }
        //!< Fallback dummy function for static polymorphism
        void updateImpl(const Clock& time) { }
        //!< Fallback dummy function for static polymorphism
        ButtonType& getButtonEventArg(ButtonType& button) { return button; }

        std::vector<ButtonType> pressedButtons_; //!< List of all buttons that are currently pressed down
        OIS::InputManager* oisInputManager_;     //!< Pointer to the OIS InputManager that can create and destroy devices
    };
}

#endif /* _InputDevice_H__ */
