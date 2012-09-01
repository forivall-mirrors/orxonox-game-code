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
    @brief Declaration of the BaseWriter class, the base of all output writers.
*/

#ifndef _BaseWriter_H__
#define _BaseWriter_H__

#include "util/UtilPrereqs.h"
#include "SubcontextOutputListener.h"

namespace orxonox
{
    /**
        @brief BaseWriter is an output listener and makes the accepted output levels and contexts configurable.

        All output writers like ConsoleWriter and LogWriter are inherited from
        this class. BaseWriter itself inherits from SubcontextOutputListener.
        It adds helper functions to configure the accepted levels and contexts.

        The levels are not fully configurable, only the "max" form is allowed
        (which means that it's only possible to define a maximum level, not
        the full mask).

        Contexts are defined by a vector of strings, each context is defined
        by it's name. Sub-contexts have the form \a "main-name::sub-name", i.e.
        their name is concatenated with :: in between.

        Each instance of BaseWriter needs a name to generate distinguishable
        config values.

        Received output messages are split into lines and sent line by line to
        the virtual printLine() function. Each line has a prepended prefix
        which describes the level and context of the output.
    */
    class _UtilExport BaseWriter : public SubcontextOutputListener
    {
        public:
            BaseWriter(const std::string& name, bool bRegister = true);
            virtual ~BaseWriter();

            void setLevelMax(OutputLevel max);
            void setAdditionalContextsLevelMax(OutputLevel max);

            /// @brief Returns the name of this instance.
            const std::string& getName() const
                { return this->name_; }

            /// Config value, used to define the maximum output level (independent of contexts)
            int configurableMaxLevel_;
            /// @brief Returns the name of the config value which defines the maximum output level (independent of contexts).
            inline std::string getConfigurableMaxLevelName() const
                { return this->name_ + "Level"; }

            /// Config value, used to define the maximum output level of additional context
            int configurableAdditionalContextsMaxLevel_;
            /// @brief Returns the name of the config value which defines the maximum output level of additional context.
            inline std::string getConfigurableAdditionalContextsMaxLevelName() const
                { return this->name_ + "AdditionalContextsLevel"; }

            /// Config vector, used to define the additional contexts (and sub-contexts)
            std::vector<std::string> configurableAdditionalContexts_;
            /// @brief Returns the name of the config vector which defines the additional contexts (and sub-contexts)
            inline std::string getConfigurableAdditionalContextsName() const
                { return this->name_ + "AdditionalContexts"; }

            void changedConfigurableLevel();
            void changedConfigurableAdditionalContextsLevel();
            void changedConfigurableAdditionalContexts();

            /// Returns the (static) name of the section wherein the config-values are defined.
            static inline std::string getConfigurableSectionName()
                { return "Output"; }

        protected:
            virtual void output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines);

        private:
            virtual void printLine(const std::string& line, OutputLevel level) = 0; ///< Pure virtual function, gets called for each line of output together with a prefix which describes level and context of the output.

            void setLevelRange(OutputLevel min, OutputLevel max);                   ///< Inherited function, overwritten as private because it is not supported by the config-value
            void setLevelMask(OutputLevel mask);                                    ///< Inherited function, overwritten as private because it is not supported by the config-value

            void setAdditionalContextsLevelRange(OutputLevel min, OutputLevel max); ///< Inherited function, overwritten as private because it is not supported by the config-value
            void setAdditionalContextsLevelMask(OutputLevel mask);                  ///< Inherited function, overwritten as private because it is not supported by the config-value

            std::string name_; ///< The name of this instance, used to generate unique config-values
    };
}

#endif /* _BaseWriter_H__ */
