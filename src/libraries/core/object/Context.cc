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
    @brief Implementation of Context.
*/

#include "Context.h"
#include "core/class/Identifier.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    RegisterClass(Context);

    Context* Context::rootContext_s = 0;

    Context* getContextForInitializationOfOtherContexts()
    {
        static size_t count = 0;
        // the first time this is called, ++count returns 1 and the context is created
        // the second time this is called, ++count returns 2 and NULL is returned because we're in the constructor of the context itself
        // for each future call the context (now completely created) is returned
        if (++count == 2)
            return NULL;
        else
        {
            static Context context(NULL);
            return &context;
        }
    }

    // Initialize Listable(*) with a special context, only used to initialize other contexts. Later in the constructor we change the context
    Context::Context(Context* context) : Listable(getContextForInitializationOfOtherContexts()), parentContext_(context)
    {
        RegisterObject(Context);

        // the context is its own context
        this->setContext(this);
    }

    Context::~Context()
    {
        // unregister context from object lists before object lists are destroyed
        this->unregisterObject();
        for (size_t i = 0; i < this->objectLists_.size(); ++i)
            delete this->objectLists_[i];
    }

    /*static*/ void Context::setRootContext(Context* context)
    {
        Context::rootContext_s = context;
    }

    /*static*/ void Context::destroyRootContext()
    {
        delete Context::rootContext_s;
        Context::rootContext_s = NULL;
    }

    /*static*/ Context* Context::getRootContext()
    {
        OrxVerify(Context::rootContext_s != NULL, "Root Context is undefined");
        return Context::rootContext_s;
    }

    ObjectListBase* Context::getObjectList(const Identifier* identifier)
    {
        unsigned int classID = identifier->getClassID();
        if (classID >= this->objectLists_.size())
            this->objectLists_.resize(classID + 1);
        if (!this->objectLists_[classID])
            this->objectLists_[classID] = new ObjectListBase();
        return this->objectLists_[classID];
    }

    void Context::destroyObjectList(const Identifier* identifier)
    {
        ObjectListBase* objectList = this->getObjectList(identifier);
        delete objectList;
        this->objectLists_[identifier->getClassID()] = NULL;
    }
}
