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
    Declaration of facilities to handle exceptions.
*/

#ifndef _Exception_H__
#define _Exception_H__

#include "UtilPrereqs.h"

#include <exception>
#include <sstream>
#include <string>
#include "Debug.h"

namespace orxonox
{
    /**
    @brief
        Base class for all exceptions (derived from std::exception).
    @details
        This class provides support for information about the file, the line
        and the function the error occured.
    */
    class _UtilExport Exception : public std::exception
    {
    public:
        /**
        @brief
            Creates the exception but doesn't yet compose the full descrption (because of the virtual functions)
        @param description
            Exception description as string. This message is supposed to help developers!
        */
        Exception(const std::string& description, unsigned int lineNumber,
                  const char* filename, const char* functionName);
        //! Simplified constructor with just a description. If you need more, use the other one.
        Exception(const std::string& description);

        //! Needed for compatibility with std::exception
        virtual ~Exception() throw() { }
        const char* what() const throw();

        //! Returns a full description with type, line, file and function
        virtual const std::string& getFullDescription() const;
        //! Returns the string name of the exception type
        virtual std::string        getTypeName()        const = 0;
        //! Returns the short developer written exception
        virtual const std::string& getDescription()     const { return this->description_; }
        //! Returns the line number on which the exception occurred.
        virtual const unsigned int getLineNumber()      const { return this->lineNumber_; }
        //! Returns the function in which the exception occurred.
        virtual const std::string& getFunctionName()    const { return this->functionName_; }
        //! Returns the filename in which the exception occurred.
        virtual const std::string& getFilename()        const { return this->filename_; }

        /**
        @brief
            Retrieves information from an exception caught with "..."
            Works for std::exception and CEGUI::Exception
        @remarks
            Never ever call this function without an exception in the stack!
        */
        static std::string handleMessage();

    protected:
        std::string description_;             //!< User typed text about why the exception occurred
        unsigned int lineNumber_;             //!< Line on which the exception occurred
        std::string functionName_;            //!< Function (including namespace and class) where the exception occurred
        std::string filename_;                //!< File where the exception occurred
        // mutable because "what()" is a const method
        mutable std::string fullDescription_; //!< Full description with line, file and function
    };

//! Creates a new type of exception that inherits from tracker::Exception
#define CREATE_ORXONOX_EXCEPTION(ExceptionName)                                     \
    class ExceptionName##Exception : public Exception                               \
    {                                                                               \
    public:                                                                         \
        ExceptionName##Exception(const std::string& description,                    \
                unsigned int lineNumber, const char* filename,                      \
                const char* functionName)                                           \
            : Exception(description, lineNumber, filename, functionName)            \
        { }                                                                         \
                                                                                    \
        ExceptionName##Exception(const std::string& description)                    \
            : Exception(description)                                                \
        { }                                                                         \
                                                                                    \
        ~ExceptionName##Exception() throw() { }                                     \
                                                                                    \
        std::string getTypeName() const { return #ExceptionName; }                  \
    }

    // Creates all possible exception types.
    // If you want to add a new type, simply copy and adjust a new line here.
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    CREATE_ORXONOX_EXCEPTION(General);
    CREATE_ORXONOX_EXCEPTION(FileNotFound);
    CREATE_ORXONOX_EXCEPTION(Argument);
    CREATE_ORXONOX_EXCEPTION(PhysicsViolation);
    CREATE_ORXONOX_EXCEPTION(ParseError);
    CREATE_ORXONOX_EXCEPTION(PluginsNotFound);
    CREATE_ORXONOX_EXCEPTION(InitialisationFailed);
    CREATE_ORXONOX_EXCEPTION(InitialisationAborted);
    CREATE_ORXONOX_EXCEPTION(NotImplemented);
    CREATE_ORXONOX_EXCEPTION(GameState);
    CREATE_ORXONOX_EXCEPTION(NoGraphics);
    CREATE_ORXONOX_EXCEPTION(AbortLoading);
#endif

    /**
    @brief
        Helper function that forwards an exception and displays the message.
    @details
        This is necessary because only when using 'throw' the objects storage is managed.
    */
    template <class T>
    inline const T& exceptionThrowerHelper(const T& exception)
    {
        // let the catcher decide whether to display the message below level 4
        COUT(4) << exception.getFullDescription() << std::endl;
        return exception;
    }

/**
@brief
    Throws an exception and logs a message beforehand.
@param type
    Type of the exception as literal (General, Initialisation, etc.)
@param description
    Exception description as string
*/
#define ThrowException(type, description) \
    throw orxonox::exceptionThrowerHelper(type##Exception(static_cast<std::ostringstream&>(std::ostringstream().flush() << description).str(), __LINE__, __FILE__, __FUNCTIONNAME__))

} /* namespace orxonox */

#endif /* _Exception_H__ */
