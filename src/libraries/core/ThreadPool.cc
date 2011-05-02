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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "ThreadPool.h"

#include "util/OrxAssert.h"
#include "Thread.h"

namespace orxonox
{

    ThreadPool::ThreadPool()
    {
    }

    ThreadPool::~ThreadPool()
    {
        OrxVerify(this->setNrOfThreads(0) == 0, "" );
    }

    void ThreadPool::addThreads( unsigned int nr )
    {
        for( unsigned int i=0; i<nr; i++ )
            this->threadPool_.push_back(new Thread());
    }
    unsigned int ThreadPool::removeThreads( unsigned int nr )
    {
        unsigned int i=0;
        std::vector<Thread*>::iterator it;
        for( it = this->threadPool_.begin(); it != threadPool_.end() && i<nr; )
        {
            if( ! (*it)->isWorking() )
            {
                Thread* temp = *it;
                it=this->threadPool_.erase( it );
                delete temp;
                ++i;
            }
            else
              ++it;
        }
        return i;
    }
    unsigned int ThreadPool::setNrOfThreads( unsigned int nr )
    {
        unsigned int currentsize = this->threadPool_.size();
        if ( nr < currentsize )
            return currentsize - removeThreads( currentsize - nr );
        else if ( nr == currentsize )
            return currentsize;
        else
        {
            addThreads( nr - currentsize );
            return nr;
        }
    }

    bool ThreadPool::passFunction( const ExecutorPtr& executor, bool addThread )
    {
        std::vector<Thread*>::iterator it;
        for ( it=this->threadPool_.begin(); it!=this->threadPool_.end(); ++it )
        {
            if ( ! (*it)->isWorking() )
            {
                // If that fails, then there is some code error
                OrxVerify( (*it)->evaluateExecutor( executor ), "" );
                return true;
            }
        }
        if ( addThread )
        {
            addThreads( 1 );
            OrxVerify( this->threadPool_.back()->evaluateExecutor( executor ), "" ); // access the last element
            return true;
        }
        else
            return false;
    }

    void ThreadPool::synchronise()
    {
        std::vector<Thread*>::iterator it;
        for ( it=this->threadPool_.begin(); it!=this->threadPool_.end(); ++it )
        {
            (*it)->waitUntilFinished();
        }
    }

}
