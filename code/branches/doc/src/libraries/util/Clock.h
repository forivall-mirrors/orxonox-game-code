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

#ifndef _Clock_H__
#define _Clock_H__

#include "UtilPrereqs.h"
#include "OgreForwardRefs.h"

namespace orxonox
{
    /** Simple real time clock based on Ogre::Timer
    @details
        The class can be used to both capture the current real time or to
        incrementally capture the time and then distribute that time information
        via Clock& references (for instance for the game tick). <br>
        Precision: <br>
        The maximum precision is given by the Ogre::Timer and that is somewhere
        in the microsecond range for both Windows and UNIX. 
    @remarks
        For proper functionality this class MUST be used in the same thread! <br>
        Further more it might be possible that the Ogre::Timer has a performance
        caveat on Windows because it will only capture the time on the same
        CPU core. Confining the main thread to one process could speed up the game.
        See command line argument 'limitToCPU'.
    */
    class _UtilExport Clock
    {
    public:
        //! Starts the time at 0
        Clock();
        ~Clock();

        //! Internally captures the time and stays at that particular time
        void capture();

        //! Returns the last captured absolute time in microseconds
        unsigned long long getMicroseconds() const
            { return tickTime_; }
        //! Returns the last captured absolute time in milliseconds
        unsigned long long getMilliseconds() const
            { return tickTime_ / 1000; }
        //! Returns the last captured absolute time in seconds
        unsigned long getSeconds() const
            { return static_cast<long> (tickTime_ / 1000000); }
        //! Returns the last captured absolute time in seconds as float
        float getSecondsPrecise() const
            { return static_cast<float>(tickTime_ / 1000000.0f); }

        //! Returns the timespan in seconds between the last two calls to capture()
        float getDeltaTime() const
            { return tickDtFloat_; }
        //! Returns the timespan in microseconds between the last two calls to capture()
        long getDeltaTimeMicroseconds() const
            { return tickDt_; }

        /** Returns the current real time in microseconds
        @note
            This is especially useful to measure execution times because of the
            high precision.
        */
        unsigned long long getRealMicroseconds() const;

    private:
        //! Undefined
        Clock(const Clock& instance);

        Ogre::Timer*       timer_;       //!< Ogre timer object
        unsigned long long tickTime_;    //!< Currently captured time
        long               tickDt_;      //!< Delta time in microseconds (cache value)
        float              tickDtFloat_; //!< Delta time in seconds (cache value)
    };
}

#endif /* _Clock_H__ */
