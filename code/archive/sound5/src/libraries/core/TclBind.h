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

#ifndef _TclBind_H__
#define _TclBind_H__

#include "CorePrereqs.h"

#include <cassert>
#include <string>
#include "util/Singleton.h"

namespace orxonox
{
    class _CoreExport TclBind : public Singleton<TclBind>
    {
        friend class Singleton<TclBind>;
        public:
            TclBind(const std::string& datapath);
            ~TclBind();

            static std::string tcl(const std::string& tclcode);
            static void bgerror(const std::string& error);

            void setDataPath(const std::string& datapath);
            const std::string& getTclDataPath() const { return this->tclDataPath_; }
            static std::string getTclLibraryPath();

            void initializeTclInterpreter();
            static Tcl::interpreter* createTclInterpreter();
            Tcl::interpreter* getTclInterpreter() const { return this->interpreter_; }

            static std::string tcl_query(Tcl::object const &args);
            static void tcl_execute(Tcl::object const &args);

            static bool eval(const std::string& tclcode);

        private:
            TclBind(const TclBind& other);

            Tcl::interpreter* interpreter_;
            std::string tclDataPath_;
            bool bSetTclDataPath_;

            static TclBind* singletonPtr_s;
    };
}

#endif /* _TclBind_H__ */
