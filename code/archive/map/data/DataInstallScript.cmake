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
 #    Joins both resources.oxr files together
 #  Caution:
 #    Variables are declared with @...@ because they don't exist anymore
 #    during the installation. But we still require variables (_external_file)
 #    so we can only replace @...@ but not ${...}
 #

# Write some comment
FILE(APPEND @DATA_INSTALL_DIRECTORY@/DefaultResources.oxr "\n\n\n <!-- ---------------------------------------- -->")
FILE(APPEND @DATA_INSTALL_DIRECTORY@/DefaultResources.oxr     "\n <!-- Content from the external data directory -->")
FILE(APPEND @DATA_INSTALL_DIRECTORY@/DefaultResources.oxr     "\n <!-- ---------------------------------------- -->\n\n")

# Append the external file
FILE(READ   @EXTERNAL_DATA_DIRECTORY@/resources.oxr _external_file)
FILE(APPEND @DATA_INSTALL_DIRECTORY@/DefaultResources.oxr ${_external_file})
