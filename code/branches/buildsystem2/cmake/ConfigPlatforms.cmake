 #
 #   ORXONOX - the hottest 3D action shooter ever to exist
 #                    > www.orxonox.net <
 #
 #
 #   License notice:
 #
 #   This program is free software; you can redistribute it and/or
 #   modify it under the terms of the GNU General Public License
 #   as published by the Free Software Foundation; either version 2
 #   of the License, or (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program; if not, write to the Free Software
 #   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #   Author:
 #      Reto Grieder
 #   Co-authors:
 #      ...
 #
 
############## Platform config ################
# Do some platform specific fiddling.
###############################################

# Set some variables to default values that may be configured below
SET(ENET_ADDITIONAL_LIBRARIES "")
SET(ORXONOX_FPIC_FLAG "-fPIC")
SET(LIBRARY_DEBUG_POSTFIX "_d")
MARK_AS_ADVANCED(
  ENET_ADDITIONAL_LIBRARIES
  ORXONOX_FPIC_FLAG
  LIBRARY_DEBUG_POSTFIX
)

# Set the platform specific options and paths
INCLUDE(ConfigTardis)
INCLUDE(ConfigMSVC)
INCLUDE(ConfigMinGW)
