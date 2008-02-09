/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include "Loader.h"
#include "Identifier.h"
#include "Iterator.h"
#include "BaseObject.h"
#include "util/tinyxml/tinyxml.h"

namespace orxonox
{
    std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > Loader::files_s;

    // ###############################
    // ###          open           ###
    // ###############################
    // Single file
    bool Loader::open(const std::string& file)
    {
        return Loader::open(file, ClassIdentifier<BaseObject>::getIdentifier());
    }

    bool Loader::open(const std::string& file, const Identifier* subclassInclude)
    {
        return Loader::open(file, subclassInclude, 0);
    }

    bool Loader::open(const std::string& file, const Identifier* subclassInclude, const Identifier* subclassExclude)
    {
        return Loader::open(file, subclassInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::open(const std::string& file, const Identifier* subclassInclude, std::vector<const Identifier*> subclassesExclude)
    {
        return Loader::open(file, std::vector<const Identifier*>(1, subclassInclude), subclassesExclude);
    }

    bool Loader::open(const std::string& file, const std::vector<const Identifier*>& subclassesInclude)
    {
        return Loader::open(file, subclassesInclude, 0);
    }

    bool Loader::open(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude)
    {
        return Loader::open(file, subclassesInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::open(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, std::vector<const Identifier*> subclassesExclude)
    {
        return Loader::open(std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >(1, std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(file, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(subclassesInclude, subclassesExclude))));
    }


    // Multiple files with common conditions
    bool Loader::open(const std::vector<std::string>& files)
    {
        return Loader::open(files, ClassIdentifier<BaseObject>::getIdentifier());
    }

    bool Loader::open(const std::vector<std::string>& files, const Identifier* subclassInclude)
    {
        return Loader::open(files, subclassInclude, 0);
    }

    bool Loader::open(const std::vector<std::string>& files, const Identifier* subclassInclude, const Identifier* subclassExclude)
    {
        return Loader::open(files, subclassInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::open(const std::vector<std::string>& files, const Identifier* subclassInclude, std::vector<const Identifier*> subclassesExclude)
    {
        return Loader::open(files, std::vector<const Identifier*>(1, subclassInclude), subclassesExclude);
    }

    bool Loader::open(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude)
    {
        return Loader::open(files, subclassesInclude, 0);
    }

    bool Loader::open(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude)
    {
        return Loader::open(files, subclassesInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::open(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude, std::vector<const Identifier*> subclassesExclude)
    {
        std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > temp;
        for (std::vector<std::string>::const_iterator it = files.begin(); it < files.end(); ++it)
            temp.insert(temp.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(*it, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(subclassesInclude, subclassesExclude)));

        return Loader::open(temp);
    }


    // Multiple files with individual conditions
    bool Loader::open(const std::vector<std::pair<std::string, const Identifier*> >& files)
    {
        std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > temp;
        for (std::vector<std::pair<std::string, const Identifier*> >::const_iterator it = files.begin(); it < files.end(); ++it)
            temp.insert(temp.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(it->first, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(std::vector<const Identifier*>(1, it->second), std::vector<const Identifier*>(1, (const Identifier*)0))));

        return Loader::open(temp);
    }

    bool Loader::open(const std::vector<std::pair<std::string, std::pair<const Identifier*, const Identifier*> > >& files)
    {
        std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > temp;
        for (std::vector<std::pair<std::string, std::pair<const Identifier*, const Identifier*> > >::const_iterator it = files.begin(); it < files.end(); ++it)
            temp.insert(temp.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(it->first, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(std::vector<const Identifier*>(1, it->second.first), std::vector<const Identifier*>(1, it->second.second))));

        return Loader::open(temp);
    }

    bool Loader::open(const std::vector<std::pair<std::string, std::pair<const Identifier*, std::vector<const Identifier*> > > >& files)
    {
        std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > temp;
        for (std::vector<std::pair<std::string, std::pair<const Identifier*, std::vector<const Identifier*> > > >::const_iterator it = files.begin(); it < files.end(); ++it)
            temp.insert(temp.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(it->first, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(std::vector<const Identifier*>(1, it->second.first), it->second.second)));

        return Loader::open(temp);
    }

    bool Loader::open(const std::vector<std::pair<std::string, std::vector<const Identifier*> > >& files)
    {
        std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > temp;
        for (std::vector<std::pair<std::string, std::vector<const Identifier*> > >::const_iterator it = files.begin(); it < files.end(); ++it)
            temp.insert(temp.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(it->first, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(it->second, std::vector<const Identifier*>(1, (const Identifier*)0))));

        return Loader::open(temp);
    }

    bool Loader::open(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, const Identifier*> > >& files)
    {
        std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > temp;
        for (std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, const Identifier*> > >::const_iterator it = files.begin(); it < files.end(); ++it)
            temp.insert(temp.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(it->first, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(it->second.first, std::vector<const Identifier*>(1, it->second.second))));

        return Loader::open(temp);
    }

    bool Loader::open(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >& files)
    {
        Loader::files_s = files;
        Loader::unload();
        return Loader::load(Loader::files_s);
    }

    // ###############################
    // ###           add           ###
    // ###############################
    bool Loader::add(const std::string& file)
    {
        return Loader::add(file, ClassIdentifier<BaseObject>::getIdentifier());
    }

    bool Loader::add(const std::string& file, const Identifier* subclassInclude)
    {
        return Loader::add(file, subclassInclude, 0);
    }

    bool Loader::add(const std::string& file, const Identifier* subclassInclude, const Identifier* subclassExclude)
    {
        return Loader::add(file, std::vector<const Identifier*>(1, subclassInclude), std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::add(const std::string& file, const Identifier* subclassInclude, std::vector<const Identifier*> subclassesExclude)
    {
        return Loader::add(file, std::vector<const Identifier*>(1, subclassInclude), subclassesExclude);
    }

    bool Loader::add(const std::string& file, const std::vector<const Identifier*>& subclassesInclude)
    {
        return Loader::add(file, subclassesInclude, 0);
    }

    bool Loader::add(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude)
    {
        return Loader::add(file, subclassesInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::add(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, std::vector<const Identifier*> subclassesExclude)
    {
        Loader::files_s.insert(Loader::files_s.end(), std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > >(file, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> >(subclassesInclude, subclassesExclude)));
        return Loader::load(file, subclassesInclude, subclassesExclude);
    }

    // ###############################
    // ###         remove          ###
    // ###############################
    void Loader::remove()
    {
        Loader::files_s.clear();
    }

    void Loader::remove(const std::vector<std::string>& files)
    {
        for (std::vector<std::string>::const_iterator it = files.begin(); it < files.end(); ++it)
            Loader::remove(*it);
    }

    void Loader::remove(const std::string& file)
    {
        for (std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >::iterator it = Loader::files_s.begin(); it < Loader::files_s.end(); ++it)
            if (it->first.compare(file) == 0)
                Loader::files_s.erase(it);
    }

    // ###############################
    // ###         reload          ###
    // ###############################
    bool Loader::reload()
    {
        return Loader::reload(ClassIdentifier<BaseObject>::getIdentifier());
    }

    bool Loader::reload(const Identifier* subclassInclude)
    {
        return Loader::reload(subclassInclude, 0);
    }

    bool Loader::reload(const Identifier* subclassInclude, const Identifier* subclassExclude)
    {
        return Loader::reload(std::vector<const Identifier*>(1, subclassInclude), std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::reload(const Identifier* subclassInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        return Loader::reload(std::vector<const Identifier*>(1, subclassInclude), subclassesExclude);
    }

    bool Loader::reload(const std::vector<const Identifier*>& subclassesInclude)
    {
        return Loader::reload(subclassesInclude, 0);
    }

    bool Loader::reload(const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude)
    {
        return Loader::reload(subclassesInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    bool Loader::reload(const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        Loader::unload(subclassesInclude, subclassesExclude);
        return Loader::load(Loader::files_s, subclassesInclude, subclassesExclude);
    }

    // ###############################
    // ###         unload          ###
    // ###############################
    void Loader::unload()
    {
        Loader::unload(ClassIdentifier<BaseObject>::getIdentifier());
    }

    void Loader::unload(const Identifier* subclassInclude)
    {
        Loader::unload(subclassInclude, 0);
    }

    void Loader::unload(const Identifier* subclassInclude, const Identifier* subclassExclude)
    {
        Loader::unload(std::vector<const Identifier*>(1, subclassInclude), std::vector<const Identifier*>(1, subclassExclude));
    }

    void Loader::unload(const Identifier* subclassInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        Loader::unload(std::vector<const Identifier*>(1, subclassInclude), subclassesExclude);
    }

    void Loader::unload(const std::vector<const Identifier*>& subclassesInclude)
    {
        Loader::unload(subclassesInclude, 0);
    }

    void Loader::unload(const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude)
    {
        Loader::unload(subclassesInclude, std::vector<const Identifier*>(1, subclassExclude));
    }

    void Loader::unload(const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        for (Iterator<BaseObject> it = ObjectList<BaseObject>::start(); it; )
        {
            bool isIncluded = false;
            for (std::vector<const Identifier*>::const_iterator it_in = subclassesInclude.begin(); it_in < subclassesInclude.end(); ++it_in)
            {
                if (it->isA(*it_in))
                {
                    isIncluded = true;
                    break;
                }
            }

            if (isIncluded)
            {
                bool isExcluded = false;
                for (std::vector<const Identifier*>::const_iterator it_ex = subclassesInclude.begin(); it_ex < subclassesInclude.end(); ++it_ex)
                {
                    if (it->isA(*it_ex))
                    {
                        isExcluded = true;
                        break;
                    }
                }

                if (!isExcluded)
                {
                    delete *(it++);
                    continue;
                }
            }

            ++it;
        }
    }

    // ###############################
    // ###          load           ###
    // ###############################
    bool Loader::load(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >& files)
    {
        bool success = true;
        for (std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >::const_iterator it = files.begin(); it < files.end(); ++it)
            if (!Loader::load(it->first, it->second.first, it->second.second))
                success = false;

        return success;
    }

    bool Loader::load(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        bool success = true;
        for (std::vector<std::string>::const_iterator it = files.begin(); it < files.end(); ++it)
            if (!Loader::load(*it, subclassesInclude, subclassesExclude))
                success = false;

        return success;
    }

    bool Loader::load(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >& files, const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        bool success = true;
        for (std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >::const_iterator it = files.begin(); it < files.end(); ++it)
            if (!Loader::load(it->first, subclassesInclude, subclassesExclude))
                success = false;

        return success;
    }

    bool Loader::load(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude)
    {
        // ...load... //
    }
}
