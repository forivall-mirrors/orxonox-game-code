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

        protected:
            virtual void output(OutputLevel level, OutputContext context, const std::string& message)
            {
                COUT(0) << (int)level << " / " << context << " : " << message << endl;
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
        for (size_t i = 0; i < this->listeners_.size(); ++i)
            if (this->listeners_[i]->acceptsOutput(level, context))
                this->listeners_[i]->output(level, context, message);
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
}
}
