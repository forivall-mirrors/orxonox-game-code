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
 #    Check package version info
 #    MAJOR: Breaking change
 #    MINOR: No breaking changes by the dependency package
 #           For example any code running on 3.0 should still run on 3.1
 #           But you can specify that the code only runs on 3.1 and higher
 #           or 4.0 and higher (so both 3.1 and 4.0 will work).

MACRO(CHECK_PACKAGE_VERSION)
  SET(_allowed_minimum_versions ${ARGN})

  IF(NOT EXISTS ${DEPENDENCY_PACKAGE_DIR}/version.txt)
    SET(DEPENDENCY_VERSION 1.0)
  ELSE()
    # Get version from file
    FILE(READ ${DEPENDENCY_PACKAGE_DIR}/version.txt _file_content)
    SET(_match)
    STRING(REGEX MATCH "([0-9]+.[0-9]+)" _match ${_file_content})
    IF(_match)
      SET(DEPENDENCY_VERSION ${_match})
    ELSE()
      MESSAGE(FATAL_ERROR "The version.txt file in the dependency file has corrupt version information.")
    ENDIF()
  ENDIF()
  
  INCLUDE(CompareVersionStrings)
  SET(_version_match FALSE)
  FOREACH(_version ${_allowed_minimum_versions})
    # Get major version
    STRING(REGEX REPLACE "^([0-9]+)\\..*$" "\\1" _major_version "${_version}")
    COMPARE_VERSION_STRINGS(${DEPENDENCY_VERSION} ${_major_version} _result TRUE)
    IF(_result EQUAL 0)
      COMPARE_VERSION_STRINGS(${DEPENDENCY_VERSION} ${_version} _result FALSE)
      IF(NOT _result LESS 0)
        SET(_version_match TRUE)
      ENDIF()
    ENDIF()
  ENDFOREACH(_version)
  IF(NOT _version_match)
    MESSAGE(FATAL_ERROR "Your dependency package version is ${DEPENDENCY_VERSION}\n"
            "Possible required versions: ${_allowed_minimum_versions}\n"
            "You can get a new version from www.orxonox.net")
  ENDIF()
  
ENDMACRO(CHECK_PACKAGE_VERSION)
