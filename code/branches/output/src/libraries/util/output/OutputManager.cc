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

#include "OutputManager.h"

#include "util/Debug.h"
#include "OutputListener.h"

namespace orxonox
{
namespace test
{
    class ConsoleOutput : public OutputListener
    {
        public:
            ConsoleOutput()
            {
                this->setLevelMax(level::user_info);
            }

            virtual void output(OutputLevel level, OutputContext context, const std::vector<std::string>& lines)
            {
                std::string prefix = OutputManager::getInstance().getLevelName(level) + ": ";
                if (context != context::undefined)
                {
                    std::string context_name = OutputManager::getInstance().getContextName(context);
                    if (context_name == "")
                        context_name = OutputManager::getInstance().getComposedContextName(context);
                    prefix += "[" + context_name + "] ";
                }
                std::string blanks(prefix.length(), ' ');

                for (size_t i = 0; i < lines.size(); ++i)
                    COUT(0) << (i == 0 ? prefix : blanks) << lines[i] << endl;
            }
    };

    OutputManager::OutputManager()
    {
        this->combinedLevelMask_ = 0;
        this->combinedContextMask_ = 0;
    }

    OutputManager::~OutputManager()
    {
    }

    /*static*/ OutputManager& OutputManager::getInstance()
    {
        static OutputManager& instance = OutputManager::getInstanceInternal();
        static ConsoleOutput consoleOutputInstance;
        return instance;
    }

    /*static*/ OutputManager& OutputManager::getInstanceInternal()
    {
        static OutputManager instance;
        return instance;
    }

    void OutputManager::pushMessage(OutputLevel level, OutputContext context, const std::string& message)
    {
        std::vector<std::string> lines;
        for (size_t start = 0, end = 0; end != std::string::npos; start = end + 1)
        {
            end = message.find_first_of('\n', start);
            lines.push_back(message.substr(start, end));
        }

        for (size_t i = 0; i < this->listeners_.size(); ++i)
            if (this->listeners_[i]->acceptsOutput(level, context))
                this->listeners_[i]->output(level, context, lines);
    }

    void OutputManager::registerListener(OutputListener* listener)
    {
        this->listeners_.push_back(listener);
        this->updateMasks();
    }

    void OutputManager::unregisterListener(OutputListener* listener)
    {
        for (std::vector<OutputListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if (*it == listener)
            {
                this->listeners_.erase(it);
                break;
            }
        }
        this->updateMasks();
    }

    void OutputManager::updateMasks()
    {
        this->updateCombinedLevelMask();
        this->updateCombinedContextMask();
    }

    void OutputManager::updateCombinedLevelMask()
    {
        this->combinedLevelMask_ = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            this->combinedLevelMask_ |= this->listeners_[i]->getLevelMask();
    }

    void OutputManager::updateCombinedContextMask()
    {
        this->combinedContextMask_ = 0;
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            this->combinedContextMask_ |= this->listeners_[i]->getContextMask();
    }

    OutputContext OutputManager::registerContext(const std::string& name)
    {
        boost::bimap<OutputContext, std::string>::right_map::iterator it = this->contexts_.right.find(name);
        if (it == this->contexts_.right.end())
        {
            OutputContext context = 0x1 << this->contexts_.size();
            this->contexts_.insert(boost::bimap<OutputContext, std::string>::value_type(context, name));
            return context;
        }
        else
        {
            return it->second;
        }
    }

    OutputContext registerContext(const std::string& name)
    {
        COUT(0) << "### register context " << name << std::endl;
        return OutputManager::getInstance().registerContext(name);
    }

    const std::string& OutputManager::getLevelName(OutputLevel level) const
    {
        switch (level)
        {
            case level::none:               { static std::string name = "None"; return name; }
            case level::debug_output:       { static std::string name = "Debug"; return name; }
            case level::user_error:         { static std::string name = "Error"; return name; }
            case level::user_warning:       { static std::string name = "Warning"; return name; }
            case level::user_status:        { static std::string name = "Status"; return name; }
            case level::user_info:          { static std::string name = "Info"; return name; }
            case level::internal_error:     { static std::string name = "Error (internal)"; return name; }
            case level::internal_warning:   { static std::string name = "Warning (internal)"; return name; }
            case level::internal_status:    { static std::string name = "Status (internal)"; return name; }
            case level::internal_info:      { static std::string name = "Info (internal)"; return name; }
            case level::verbose:            { static std::string name = "Verbose"; return name; }
            case level::verbose_more:       { static std::string name = "Verbose (more)"; return name; }
            case level::verbose_ultra:      { static std::string name = "Verbose (ultra)"; return name; }
            default:                        { static std::string name = ""; return name; }
        }
    }

    const std::string& OutputManager::getContextName(OutputContext context) const
    {
        if (context != context::undefined)
        {
            boost::bimap<OutputContext, std::string>::left_map::const_iterator it = this->contexts_.left.find(context);
            if (it != this->contexts_.left.end())
                return it->second;
        }
        return BLANKSTRING;
    }

    std::string OutputManager::getComposedContextName(OutputContext context) const
    {
        std::string name;
        size_t counter = 0;
        for (OutputContext context_test = 0x1; context_test != 0x0; context_test = context_test << 1)
        {
            if (context & context_test)
            {
                boost::bimap<OutputContext, std::string>::left_map::const_iterator it = this->contexts_.left.find(context_test);
                if (it != this->contexts_.left.end())
                {
                    if (counter)
                        name += ", ";

                    name += it->second;
                    ++counter;
                }
            }
        }
        return name;
    }
}
}
