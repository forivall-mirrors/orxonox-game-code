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
 #
 #  Author:
 #    Reto Grieder
 #  Description:
 #    Configures the number of build units per library
 #

SET(BUILD_UNITS_CONFIG_1_THREADS
  util              1
  core              1
  network           1
  tools             1
  orxonox           2 # Avoids running out of symbol adresses (fix with /bigobj)
  designtools       1
  notifications     1
  objects           1
  overlays          1
  pickup            1
  pong              1
  questsystem       1
  weapons           1
)

SET(BUILD_UNITS_CONFIG_2_THREADS
  util              1
  core              2
  network           1
  tools             1
  orxonox           2
  designtools       1
  notifications     1
  objects           2
  overlays          1
  pickup            1
  pong              1
  questsystem       1
  weapons           1
)

SET(BUILD_UNITS_CONFIG_4_THREADS
  util              2
  core              4
  network           2
  tools             2
  orxonox           4
  designtools       1
  notifications     1
  objects           2
  overlays          2
  pickup            1
  pong              1
  questsystem       2
  weapons           2
)

SET(BUILD_UNITS_CONFIG_8_THREADS
  util              2
  core              8
  network           5
  tools             3
  orxonox           8
  designtools       1
  notifications     1
  objects           3
  overlays          3
  pickup            2
  pong              1
  questsystem       3
  weapons           3
)
