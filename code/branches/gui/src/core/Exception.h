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
    Declaration of the Exception class.
*/

#ifndef _Exception_H__
#define _Exception_H__

#include "CorePrereqs.h"

#include <string>
#include <exception>
#include <cassert>
#include "core/Debug.h"

namespace orxonox
{
    class _CoreExport Exception : public std::exception
    {
    public:
        enum ExceptionType
        {
            General,
            FileNotFound,
            PluginsNotFound,
            InitialisationFailed,
            NotImplemented
        };

        Exception(const std::string& description, int lineNumber,
                  const char* fileName, const char* functionName);
        Exception(const std::string& description);

        /// Needed for  compatibility with std::exception (from Ogre::Exception)
        virtual ~Exception() throw() { }

        virtual std::string        getFullDescription() const;
        virtual ExceptionType      getType()            const = 0;
        virtual std::string        getTypeName()        const = 0;
        virtual const std::string& getDescription()     const { return this->description_; }
        virtual const int          getLineNumber()      const { return this->lineNumber_; }
        virtual const std::string& getFunctionName()    const { return this->functionName_; }

        /// Override std::exception::what (from Ogre::Exception)
        const char* what() const throw() { return getFullDescription().c_str(); }

    protected:
        std::string description_;
        int lineNumber_;
        std::string functionName_;
        std::string fileName_;
    };


    template <Exception::ExceptionType Type>
    class SpecificException : public Exception
    {
    public:
        SpecificException(const std::string& description, int lineNumber,
                  const char* fileName, const char* functionName)
                  : Exception(description, lineNumber, fileName, functionName)
        {
            // let the catcher decide whether to display the message or not
            //COUT(1) << this->getFullDescription() << std::endl;
        }

        SpecificException(const std::string& description)
            : Exception(description)
        {
            // let the catcher decide whether to display the message or not
            //COUT(1) << this->getFullDescription() << std::endl;
        }

        ~SpecificException() throw() { }

        ExceptionType getType() const { return Type; }
        std::string getTypeName() const
        {
            // note: break is not necessary due to the return. Keep in mind!
            switch (Type)
            {
            case Exception::General:
                return "General";
            case Exception::FileNotFound:
                return "FileNotFound";
            case Exception::PluginsNotFound:
                return "PluginsNotFound";
            case Exception::InitialisationFailed:
                return "InitialisationFailed";
            case Exception::NotImplemented:
                return "NotImplemented";
            default:
                return "";
            }
        }
    };

    typedef SpecificException<Exception::General> GeneralException;
    typedef SpecificException<Exception::FileNotFound> FileNotFoundException;
    typedef SpecificException<Exception::PluginsNotFound> PluginsNotFoundException;
    typedef SpecificException<Exception::InitialisationFailed> InitialisationFailedException;
    typedef SpecificException<Exception::NotImplemented> NotImplementedException;

#define ThrowException(type, description) \
    throw SpecificException<Exception::type>(description, __LINE__, __FILE__, __FUNCTIONNAME__)

    // define an assert macro that can display a message
#ifndef NDEBUG
#define OrxAssert(condition, errorMessage) \
    condition ? ((void)0) : (void)(orxonox::OutputHandler::getOutStream().setOutputLevel(ORX_ERROR) << errorMessage << std::endl); \
    assert(condition)
#else
#define OrxAssert(condition, errorMessage)  ((void)0)
#endif

}

#endif /* _Exception_H__ */
