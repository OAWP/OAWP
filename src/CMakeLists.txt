## Copyright_notice ####################################################
#                                                                      #
# SPDX-License-Identifier: GPL-3.0-or-later                            #
#                                                                      #
# Copyright (C) 2023-2024 TheRealOne78 <bajcsielias78@gmail.com>       #
#                                                                      #
# This file is part of the OAWP project                                #
#                                                                      #
# OAWP is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# OAWP is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with OAWP.  If not, see <http://www.gnu.org/licenses/>.        #
#                                                                      #
########################################################################

# Set the output directory for libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/libs")

# Set main executable source files
set("${PROJECT_NAME}_SRC"
  arg.c
  dir-handler.c
  fancy-text.c
  log.c
  oawp.c
  pixmanip.c
  sig-handlers.c
)

add_executable("${PROJECT_NAME}"
  "${${PROJECT_NAME}_SRC}")

set(include_dirs
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_BINARY_DIR}/include"
)

foreach(target
  "${PROJECT_NAME}"
)
  target_include_directories(${target} PRIVATE ${include_dirs})
  target_include_directories(${target} PRIVATE IMLIB2_INCLUDE_DIR)
  target_include_directories(${target} PRIVATE LIBPCRE_INCLUDE_DIR)
  target_include_directories(${target} PRIVATE X11_INCLUDE_DIR)
endforeach()

target_link_libraries("${PROJECT_NAME}"
  "${IMLIB2_LINK_LIBRARIES}"
  "${LIBPCRE_LINK_LIBRARIES}"
  "${LIBCONFIG_LINK_LIBRARIES}"
  "${X11_LIBRARIES}"
  "${MATH_LIBRARY}"
)

# Set the output directory for executables
set(EXECUTABLE_OUTPUT_PATH
  "${CMAKE_BINARY_DIR}/build")
