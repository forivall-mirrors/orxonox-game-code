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
    Different definitions of input processing.
*/

#ifndef _Button_H__
#define _Button_H__

#include "InputPrereqs.h"

#include <string>
#include <vector>
#include "InputCommands.h"

namespace orxonox
{
    class _CoreExport Button
    {
    public:
        Button();
        virtual ~Button();
        virtual void clear();
        virtual bool addParamCommand(ParamCommand* command) { return false; }
        void parse(const std::string& binding);
        void readBinding(ConfigFile* configFile, ConfigFile* fallbackFile);
        void setBinding(ConfigFile* configFile, ConfigFile* fallbackFile, const std::string& binding, bool bTemporary);
        bool execute(KeybindMode::Value mode, float abs = 1.0f, float rel = 1.0f);

        //! The configured string value
        std::string bindingString_;
        //! Name of the trigger as strings
        std::string name_;
        //! Name of the group of input triggers
        std::string groupName_;
        //! Basic commands for OnPress, OnHold and OnRelease
        BaseCommand** commands_[3];
        //! Number of basic commands
        unsigned int nCommands_[3];
        //! Says how much it takes for an analog axis to trigger a button
        //! Note: This variable is here to have only one parse() function.
        float buttonThreshold_;
        bool bButtonThresholdUser_;
        //! Pointer to the list of parametrised commands
        std::vector<BufferedParamCommand*>* paramCommandBuffer_;

    private:
        void parseError(const std::string& message, bool serious);
    };

    inline bool Button::execute(KeybindMode::Value mode, float abs, float rel)
    {
        // execute all the parsed commands in the string
        for (unsigned int iCommand = 0; iCommand < nCommands_[mode]; iCommand++)
            commands_[mode][iCommand]->execute(abs, rel);
        return true;
    }

}

#endif /* _Button_H__ */
