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
    @brief Declaration of the MemoryWriter singleton.
*/

#ifndef _MemoryWriter_H__
#define _MemoryWriter_H__

#include "util/UtilPrereqs.h"
#include "OutputListener.h"

namespace orxonox
{
    /**
        @brief MemoryWriter is a singleton which is derived from OutputListener and writes all output to a list.

        This list can be used to re-send old output to other instances of
        OutputListener, e.g. if they were newly created or to re-write the
        log-file.

        Since MemoryWriter receives output of all levels, this means also that
        all possible output needs to be generated as long as MemoryWriter stays
        active. Hence disable() should be called as soon as possible.
    */
    class _UtilExport MemoryWriter : public OutputListener
    {
        /// @brief A helper struct which is used to store output and its properties.
        struct Message
        {
            /// @brief Constructor, assigns all values.
            Message(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
                : level(level), context(&context), lines(lines) {}

            OutputLevel level;                      ///< The level of the output message
            const OutputContextContainer* context;  ///< The context of the output message
            std::vector<std::string> lines;         ///< The lines of text of the output message
        };

        public:
            static MemoryWriter& getInstance();

            void resendOutput(OutputListener* listener) const;
            void disable();

        protected:
            virtual void output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines);

        private:
            MemoryWriter();
            MemoryWriter(const MemoryWriter&);
            virtual ~MemoryWriter();

            std::vector<Message> messages_; ///< Stores all output messages from the creation of this instance until disable() is called.
    };
}

#endif /* _MemoryWriter_H__ */
