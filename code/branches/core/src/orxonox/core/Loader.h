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

#ifndef _Loader_H__
#define _Loader_H__

#include <string>
#include <vector>

#include "CorePrereqs.h"

namespace orxonox
{
    class Loader
    {
        public:
            static bool open(const std::string& file);
            static bool open(const std::string& file, const Identifier* subclassInclude);
            static bool open(const std::string& file, const Identifier* subclassInclude, const Identifier* subclassExclude);
            static bool open(const std::string& file, const Identifier* subclassInclude, std::vector<const Identifier*> subclassesExclude);
            static bool open(const std::string& file, const std::vector<const Identifier*>& subclassesInclude);
            static bool open(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude);
            static bool open(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, std::vector<const Identifier*> subclassesExclude);

            static bool open(const std::vector<std::string>& files);
            static bool open(const std::vector<std::string>& files, const Identifier* subclassInclude);
            static bool open(const std::vector<std::string>& files, const Identifier* subclassInclude, const Identifier* subclassExclude);
            static bool open(const std::vector<std::string>& files, const Identifier* subclassInclude, std::vector<const Identifier*> subclassesExclude);
            static bool open(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude);
            static bool open(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude);
            static bool open(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude, std::vector<const Identifier*> subclassesExclude);

            static bool open(const std::vector<std::pair<std::string, const Identifier*> >& files);
            static bool open(const std::vector<std::pair<std::string, std::pair<const Identifier*, const Identifier*> > >& files);
            static bool open(const std::vector<std::pair<std::string, std::pair<const Identifier*, std::vector<const Identifier*> > > >& files);
            static bool open(const std::vector<std::pair<std::string, std::vector<const Identifier*> > >& files);
            static bool open(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, const Identifier*> > >& files);
            static bool open(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >& files);


            static bool add(const std::string& file);
            static bool add(const std::string& file, const Identifier* subclassInclude);
            static bool add(const std::string& file, const Identifier* subclassInclude, const Identifier* subclassExclude);
            static bool add(const std::string& file, const Identifier* subclassInclude, std::vector<const Identifier*> subclassesExclude);
            static bool add(const std::string& file, const std::vector<const Identifier*>& subclassesInclude);
            static bool add(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude);
            static bool add(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, std::vector<const Identifier*> subclassesExclude);


            static void remove();
            static void remove(const std::vector<std::string>& files);
            static void remove(const std::string& file);


            static bool reload();
            static bool reload(const Identifier* subclassInclude);
            static bool reload(const Identifier* subclassInclude, const Identifier* subclassExclude);
            static bool reload(const Identifier* subclassInclude, const std::vector<const Identifier*>& subclassesExclude);
            static bool reload(const std::vector<const Identifier*>& subclassesInclude);
            static bool reload(const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude);
            static bool reload(const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude);


            static void unload();
            static void unload(const Identifier* subclassInclude);
            static void unload(const Identifier* subclassInclude, const Identifier* subclassExclude);
            static void unload(const Identifier* subclassInclude, const std::vector<const Identifier*>& subclassesExclude);
            static void unload(const std::vector<const Identifier*>& subclassesInclude);
            static void unload(const std::vector<const Identifier*>& subclassesInclude, const Identifier* subclassExclude);
            static void unload(const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude);

        private:
            static bool load(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >& files);
            static bool load(const std::vector<std::string>& files, const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude);
            static bool load(const std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > >& files, const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude);
            static bool load(const std::string& file, const std::vector<const Identifier*>& subclassesInclude, const std::vector<const Identifier*>& subclassesExclude);


            static std::vector<std::pair<std::string, std::pair<std::vector<const Identifier*>, std::vector<const Identifier*> > > > files_s;
    };
}

#endif /* _Loader_H__ */
