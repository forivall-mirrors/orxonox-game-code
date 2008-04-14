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

#include <string>

#include "UtilPrereqs.h"

class _UtilExport ArgReader
{
  public:
    ArgReader(int argc, char **argv);
    void checkArgument(std::string option, std::string& string, bool must=false);
    void checkArgument(std::string option, int& integer, bool must=false);
    void checkArgument(std::string option, float& floating, bool must=false);
    bool errorHandling();
  private:
    int checkOption(std::string option);

  private:
    int counter_;
    char **arguments_;
    bool fail_;
    std::string errorStr_;
};

#endif /* _ArgReader_H__ */
