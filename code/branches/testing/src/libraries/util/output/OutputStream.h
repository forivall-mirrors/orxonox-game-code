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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @ingroup Output
    @brief Declaration of the OutputStream class which is used to send output to orxonox::OutputManager.
*/

#ifndef _OutputStream_H__
#define _OutputStream_H__

#include "util/UtilPrereqs.h"

#include <sstream>

#include "OutputDefinitions.h"

namespace orxonox
{
    /**
        @brief This class is used to buffer output and send it to OutputManager whenever std::endl is passed to it.

        OutputStream inherits from std::ostringstream and acts like std::cout.
        This means you can use the << operator to write output to the stream.
        This class is used by the orxout() function.

        @attention
        You must end an output message with std::endl, otherwise the message
        won't be flushed. '\\n' only adds a new line to the message.

        The following code samples are all equivalent:
        @code
        OutputStream stream;
        stream.setOutputAttributes(user_info, context::example());
        stream << "Hello World" << endl;
        @endcode

        @code
        OutputStream stream(user_info, context::example());
        stream << "Hello World" << endl;
        @endcode

        @code
        orxout(user_info, context::example) << "Hello World" << endl;
        @endcode
    */
    class OutputStream : public std::ostringstream
    {
        typedef std::ostream& (*EndlType)(std::ostream&);

        public:
            _UtilExport OutputStream();
            _UtilExport OutputStream(OutputLevel level, const OutputContextContainer& context);
            _UtilExport ~OutputStream();

            void _UtilExport setOutputAttributes(OutputLevel level, const OutputContextContainer& context);

            /// @brief Generic << operator which adds output to the stream.
            template <class T>
            inline OutputStream& operator<<(const T& val) { return this->output(val); }
            /// @brief Sends a manipulator to the output stream.
            inline OutputStream& operator<<(std::ios_base& (*manipulator)(std::ios_base&)) { return this->output(manipulator); }
            /// @brief Sends a manipulator to the output stream.
            inline OutputStream& operator<<(std::ios&      (*manipulator)(std::ios&))      { return this->output(manipulator); }
            /// @brief Sends a manipulator to the output stream and flushes the message if the manipulator is std::endl.
            inline OutputStream& operator<<(std::ostream&  (*manipulator)(std::ostream&))
            {
                if (this->bAcceptsOutput_)
                {
                    if (manipulator == static_cast<EndlType>(std::endl))
                        this->sendMessage(); // send the message to OutputManager
                    else
                        return this->output(manipulator); // apply the manipulator
                }
                return *this;
            }

            inline const OutputLevel getOutputLevel() const { return this->level_; }
            inline const OutputContextContainer* getOutputContext() const { return this->context_; }

        private:
            /// @brief Generic function to add values to the output stream, using the inherited << operator from std::ostringstream.
            template <class T>
            inline OutputStream& output(const T& val)
            {
                if (this->bAcceptsOutput_)
                    static_cast<std::ostringstream&>(*this) << val;
                return *this;
            }

            void _UtilExport sendMessage();

            OutputLevel level_;                     ///< The output level of the current message
            const OutputContextContainer* context_; ///< The output context of the current message
            bool bAcceptsOutput_;                   ///< After defining level and context of the following message, this flag is set according to the masks defined in OutputManager. If it is false, the OutputStream will throw away every output sent using the << operator.
    };
}

#endif /* _OutputStream_H__ */
