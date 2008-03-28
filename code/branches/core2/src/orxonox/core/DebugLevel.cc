/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DebugLevel.cc
    @brief Implementation of the DebugLevel class.
*/

#include "DebugLevel.h"
#include "CoreIncludes.h"

namespace orxonox
{
    /**
        @brief Constructor: Registers the object and sets the debug level.
        @param A reference to a global variable, used to avoid an infinite recursion in getSoftDebugLevel()
    */
    DebugLevel::DebugLevel(bool& bReturnSoftDebugLevel)
    {
        RegisterRootObject(DebugLevel);
        this->setConfigValues();
        bReturnSoftDebugLevel = true;
    }

    /**
        @brief Function to collect the SetConfigValue-macro calls.
    */
    void DebugLevel::setConfigValues()
    {
        SetConfigValue(softDebugLevelConsole_, 3).description("The maximal level of debug output shown in the console");
        SetConfigValue(softDebugLevelLogfile_, 3).description("The maximal level of debug output shown in the logfile");
        SetConfigValue(softDebugLevelShell_, 1).description("The maximal level of debug output shown in the ingame shell");

        // softDebugLevel_ is the maximum of the 3 variables
        this->softDebugLevel_ = this->softDebugLevelConsole_;
        if (this->softDebugLevelLogfile_ > this->softDebugLevel_)
            this->softDebugLevel_ = this->softDebugLevelLogfile_;
        if (this->softDebugLevelShell_ > this->softDebugLevel_)
            this->softDebugLevel_ = this->softDebugLevelShell_;
    }

    /**
        @brief Static function that holds the singleton.
    */
    int DebugLevel::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        static bool bCreatingSoftDebugLevelObject = true;   // Static variable - used to enhance the performance
        static bool bReturnSoftDebugLevel = false;          // Static variable - used to avoid an infinite recursion
        static DebugLevel* theOnlyDebugLevelObject = 0;     // Static variable - will contain a pointer to the only instance of the DebugLevel class

        // If bReturnSoftDebugLevel is true, the instance of DebugLevel was created (it's set to true at the end of the constructor, call by reference)
        if (bReturnSoftDebugLevel)
        {
            if (device == OutputHandler::LD_All)
                return theOnlyDebugLevelObject->softDebugLevel_;
            else if (device == OutputHandler::LD_Console)
                return theOnlyDebugLevelObject->softDebugLevelConsole_;
            else if (device == OutputHandler::LD_Logfile)
                return theOnlyDebugLevelObject->softDebugLevelLogfile_;
            else if (device == OutputHandler::LD_Shell)
                return theOnlyDebugLevelObject->softDebugLevelShell_;
        }

        // If bCreatingSoftDebugLevelObject is true, we're just about to create an instance of the DebugLevel class
        if (bCreatingSoftDebugLevelObject)
        {
            bCreatingSoftDebugLevelObject = false;
            theOnlyDebugLevelObject = new DebugLevel(bReturnSoftDebugLevel);
            return getSoftDebugLevel(device);
        }

        // Return a constant value while we're creating the object
        return 5;
    }
}

/**
    @brief Returns the soft debug level, stored in the only existing instance of the DebugLevel class, configured in the config-file.
    @return The soft debug level
*/
int getSoftDebugLevel()
{
    return orxonox::DebugLevel::getSoftDebugLevel();
}
