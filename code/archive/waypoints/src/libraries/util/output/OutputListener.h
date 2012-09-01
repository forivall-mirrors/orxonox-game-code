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
    @brief Declaration of the OutputListener interface which receives output
    from orxonox::OutputManager.
*/

#ifndef _OutputListener_H__
#define _OutputListener_H__

#include "util/UtilPrereqs.h"

#include <vector>

#include "OutputDefinitions.h"

namespace orxonox
{
    /**
        @brief OutputListener is an interface which is used to receive output of a certain level and context from OutputManager.
    */
    //  See below the class declaration for a more detailed description.
    class _UtilExport OutputListener
    {
        public:
            OutputListener(bool bRegister = true);
            virtual ~OutputListener();

            void setLevelMax(OutputLevel max);
            void setLevelRange(OutputLevel min, OutputLevel max);
            void setLevelMask(OutputLevel mask);

            void setAdditionalContextsLevelMax(OutputLevel max);
            void setAdditionalContextsLevelRange(OutputLevel min, OutputLevel max);
            void setAdditionalContextsLevelMask(OutputLevel mask);

            void setAdditionalContextsMask(OutputContextMask mask);

            /// @brief Returns the level mask.
            inline OutputLevel getLevelMask() const
                { return this->levelMask_; }
            /// @brief Returns the additional contexts mask.
            inline OutputContextMask getAdditionalContextsMask() const
                { return this->additionalContextsMask_; }
            /// @brief Returns the additional contexts level mask.
            inline OutputLevel getAdditionalContextsLevelMask() const
                { return this->additionalContextsLevelMask_; }

            virtual bool acceptsOutput(OutputLevel level, const OutputContextContainer& context) const;

            /// @brief Called by OutputManager for each line of output, checks if this listener actually accepts this output before it calls the output() function.
            inline void unfilteredOutput(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
                { if (this->acceptsOutput(level, context)) this->output(level, context, lines); }

        protected:
            /// @brief Pure virtual function, needs to be implemented in order to receive output.
            virtual void output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines) = 0;

        private:
            OutputLevel       levelMask_;                   ///< Mask of accepted output levels, independent of contexts
            OutputContextMask additionalContextsMask_;      ///< Mask of accepted additional contexts
            OutputLevel       additionalContextsLevelMask_; ///< Mask of accepted output levels of the additional contexts
    };

    /**
        @class OutputListener

        An instance of OutputListener registers itself at OutputManager and
        declares the desired output levels and contexts. OutputManager will
        then send output to it by calling the output() function.

        OutputListener has 3 masks to define the desired output. These masks
        can be used in two different ways (or as a combination of both):
        \li 1. By defining the \a "level mask": The OutputListener will then
               receive all output of these levels independent of the context.
        \li 2. By defining the \a "additional contexts mask" and the
               \a "additional contexts level mask": This way the listener
               receives only output of a particular context and level.
        \li 3. By using all 3 masks which combines the output defined by the
               first two ways.

        This can be illustrated as follows:

        1. Only level mask:
        \li level-mask = error | warning;

        @verbatim
                |   Contexts:   |
                | A | B | C | D |
        --------|---|---|---|---|
        debug   | - | - | - | - |
        --------|---|---|---|---|
        error   | x | x | x | x |
        --------|---|---|---|---|       [x] Receives output
        warning | x | x | x | x |       [-] Does not receive output
        --------|---|---|---|---|
        status  | - | - | - | - |
        --------|---|---|---|---|
        verbose | - | - | - | - |
        --------|---|---|---|---|
        @endverbatim

        2. Only additional contexts:
        \li additional-contexts-mask = B | D;
        \li additional-contexts-level-mask = debug | verbose;

        @verbatim
                |   Contexts:   |
                | A | B | C | D |
        --------|---|---|---|---|
        debug   | - | x | - | x |
        --------|---|---|---|---|
        error   | - | - | - | - |
        --------|---|---|---|---|       [x] Receives output
        warning | - | - | - | - |       [-] Does not receive output
        --------|---|---|---|---|
        status  | - | - | - | - |
        --------|---|---|---|---|
        verbose | - | x | - | x |
        --------|---|---|---|---|
        @endverbatim

        3. Both level mask plus additional contexts:
        \li level-mask = error | warning;
        \li additional-contexts-mask = B | D;
        \li additional-contexts-level-mask = debug | verbose;

        @verbatim
                |   Contexts:   |
                | A | B | C | D |
        --------|---|---|---|---|
        debug   | - | x | - | x |
        --------|---|---|---|---|
        error   | x | x | x | x |
        --------|---|---|---|---|       [x] Receives output
        warning | x | x | x | x |       [-] Does not receive output
        --------|---|---|---|---|
        status  | - | - | - | - |
        --------|---|---|---|---|
        verbose | - | x | - | x |
        --------|---|---|---|---|
        @endverbatim
    */
}

#endif /* _OutputListener_H__ */
