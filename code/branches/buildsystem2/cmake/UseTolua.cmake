#    UseTolua.cmake - CMake Module to generate LUA Bindings with tolua
#    Copyright (C) 2008  Adrian Friedli
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

MACRO(TOLUA _tolua_package _tolua_srcfiles_varname)
  # TODO: cleaner arguments handling
  SET(_tolua_inputfiles ${ARGN})
  LIST(REMOVE_ITEM _tolua_inputfiles "INPUTFILES")

  SET(_tolua_pkgfile "${CMAKE_CURRENT_BINARY_DIR}/tolua.pkg")
  SET(_tolua_cxxfile "${CMAKE_CURRENT_BINARY_DIR}/ToluaBind${_tolua_package}.cc")
  SET(_tolua_hfile   "${CMAKE_CURRENT_BINARY_DIR}/ToluaBind${_tolua_package}.h")
  SET(${_tolua_srcfiles_varname} ${${_tolua_srcfiles_varname}} ${_tolua_cxxfile})

  # TODO: check secureness of this temporary file
  FILE(REMOVE ${_tolua_pkgfile})
  FOREACH(_tolua_inputfile ${_tolua_inputfiles})
    FILE(APPEND ${_tolua_pkgfile} "\$cfile \"${_tolua_inputfile}\"\n")
  ENDFOREACH(_tolua_inputfile)

  # Note: Some of the variables are already defined in src/tolua/CMakeLists.txt
  #       or in the platform config
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_tolua_cxxfile} ${_tolua_hfile}
    COMMAND toluaexe_orxonox -n ${_tolua_package}
                             -w ${CMAKE_CURRENT_SOURCE_DIR}
                             -o ${_tolua_cxxfile}
                             -H ${_tolua_hfile}
                             -s ${TOLUA_PARSER_SOURCE}
                                ${_tolua_pkgfile}
    DEPENDS              ${TOLUA_PARSER_DEPENDENCIES}
    IMPLICIT_DEPENDS CXX ${_tolua_inputfiles}
    WORKING_DIRECTORY    ${TOLUA_PARSER_WORKING_DIRECTORY}
    COMMENT "Generating tolua bind files for package ${_tolua_package}"
  )
ENDMACRO(TOLUA)
