 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #

SET(CPPTCL_FILES
  cpptcl.h
  cpptcl.cc

  details/callbacks.h
  details/callbacks_v.h
  details/constructors.h
  details/conversions.h
  details/dispatchers.h
  details/metahelpers.h
  details/methods.h
  details/methods_v.h
)

ORXONOX_ADD_LIBRARY(cpptcl_orxonox
  ORXONOX_EXTERNAL
  NO_DLL_INTERFACE
  VERSION
    1.1.3
  LINK_LIBRARIES
    ${TCL_LIBRARY}
  SOURCE_FILES
    ${CPPTCL_FILES}
)
