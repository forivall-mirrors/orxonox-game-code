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
 *      Benjamin Knecht <beni_at_orxonox.net>
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  ArgReader.cc
 @brief Argument Reader
 */

#include "ArgReader.h"
#include "SubString.h"

std::string ArgReader::parse(int argc, char **argv)
{
    std::string errorString;
    CmdLineArg arg;
    int i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-' && argv[i][1] == '-') // name
        {
            if (argv[i][2] == '\0')
            {
                errorString += "Expected word after \"--\".\n";
            }
            arg.bChecked_ = false;
            arg.name_ = argv[i] + 2;
            arg.value_ = " ";
            arguments_.push_back(arg);
        }
        else // value
        {
            if (arguments_.size() == 0)
            {
                errorString += "Expected \"--\" in command line arguments.\n";
                arg.bChecked_ = false;
                arg.name_ = "";
                arg.value_ = " ";
                arguments_.push_back(arg);
            }

            if (arguments_.back().value_ != " ")
                arguments_.back().value_ += " " + std::string(argv[i]);
            else
                arguments_.back().value_ = argv[i];
        }
        ++i;
    }
    return errorString;
}

const std::string& ArgReader::getArgument(const std::string& option)
{
    unsigned int iArg = 0;
    while (iArg < arguments_.size())
    {
        if (arguments_[iArg].name_ == option)
        {
            arguments_[iArg].bChecked_ = true;
            return arguments_[iArg].value_;
        }
        ++iArg;
    }
    return blankString;
}

bool ArgReader::allChecked()
{
    bool argumentsChecked = true;
    for (unsigned int i = 1; i < arguments_.size(); ++i)
        argumentsChecked &= arguments_[i].bChecked_;

    return argumentsChecked;
}
