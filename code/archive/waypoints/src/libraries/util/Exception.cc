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
    Implementation of the Exception class.
*/

#include "Exception.h"

#include <cstddef>
#include <CEGUIExceptions.h>
#include "Output.h"

namespace orxonox
{
    Exception::Exception(const std::string& description, unsigned int lineNumber,
                         const char* filename, const char* functionName)
        : description_(description)
        , lineNumber_(lineNumber)
        , functionName_(functionName)
        , filename_(filename)
    { }

    Exception::Exception(const std::string& description)
        : description_(description)
        , lineNumber_(0)
    { }

    const std::string& Exception::getFullDescription() const
    {
        if (fullDescription_.empty())
        {
            std::ostringstream fullDesc;

            fullDesc << this->getTypeName() << "Exception";

            if (!this->filename_.empty())
            {
                fullDesc << " in " << this->filename_;
                if (this->lineNumber_)
                    fullDesc << '(' << this->lineNumber_ << ')';
            }

            if (!this->functionName_.empty())
                fullDesc << " in function '" << this->functionName_ << '\'';

            fullDesc << ": ";
            if (!this->description_.empty())
                fullDesc << this->description_;
            else
                fullDesc << "No description available.";

            this->fullDescription_ = fullDesc.str();
        }

        return fullDescription_;
    }

    const char* Exception::what() const throw()
    {
        return getDescription().c_str();
    }

    /*static*/ std::string Exception::handleMessage()
    {
        try
        {
            // rethrow
            throw;
        }
        catch (const CEGUI::Exception& ex)
        {
            return GeneralException(ex.getMessage().c_str(), ex.getLine(),
                ex.getFileName().c_str(), ex.getName().c_str()).getDescription();
        }
        catch (const std::exception& ex)
        {
            return ex.what();
        }
        catch (const char* ex)
        {
            return ex;
        }
        catch (...)
        {
            orxout(user_error) << "BIG WARNING: Unknown exception type encountered."
                               << " Rethrowing" << endl;
            throw;
        }
    }
}
