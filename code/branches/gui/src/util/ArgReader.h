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
 @file  Argreader.h
 @brief reads arguments
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _ArgReader_H__
#define _ArgReader_H__

#include "UtilPrereqs.h"

#include <string>
#include <vector>
#include "Convert.h"


class _UtilExport ArgReader
{
public:
    ArgReader() { }
    std::string parse(int argc, char **argv);
    const std::string& getArgument(const std::string& option);
    bool allChecked();

private:
    struct CmdLineArg
    {
        std::string name_;
        std::string value_;
        bool bChecked_;
    };

    std::vector<CmdLineArg> arguments_;
};

#endif /* _ArgReader_H__ */
