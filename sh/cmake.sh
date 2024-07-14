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

: "${CMAKE_BINARY_DIR:="./build/"}"
: "${CMAKE_SOURCE_DIR:="./src/"}"
: "${CMAKE_BUILD_TYPE:="Release"}"

cmake_init() {
    # Configure the project
    mkdir -p "${CMAKE_BINARY_DIR}"

    cmake -S . -B "${CMAKE_BINARY_DIR}"
    EXIT_CODE=${?}   

    if [ ${EXIT_CODE} -ne 0 ]; then
        log_err "Couldn't configure the project! Please check the source files."
        exit ${EXIT_CODE}
    fi
}

cmake_build() {
    # Run cmake to build the project
    log_info "Compiling with $(nproc) threads ..."
    cmake --build "${CMAKE_BINARY_DIR}" -j="$(nproc)"
    EXIT_CODE=${?}

    if [ ${EXIT_CODE} -ne 0 ]; then
        log_err "Couldn't build the project! Please check the build logs."
        exit ${EXIT_CODE}
    fi

    log_info "Done compiling!"
}

cmake_test() {
    # Run ctest to run unit tests
    ctest --test-dir "${CMAKE_BINARY_DIR}"
    EXIT_CODE=${?}

    if [ ${EXIT_CODE} -ne 0 ]; then
        log_err "Tests failed! Please check the test logs."
        exit ${EXIT_CODE}
    fi
}

cmake_install() {
    # Run cmake to install the project
    cmake --build "${CMAKE_BINARY_DIR}" --target install
    EXIT_CODE=${?}

    if [ ${EXIT_CODE} -ne 0 ]; then
        log_err "Couldn't install! Please check if OAWP compiled successfully."
        exit ${EXIT_CODE}


    fi
}

cmake_uninstall() {
    # Run cmake to uninstall the project
    cmake --build "${CMAKE_BINARY_DIR}" --target uninstall
    EXIT_CODE=${?}

    if [ ${EXIT_CODE} -ne 0 ]; then
        log_err "Couldn't uninstall!"
        exit ${EXIT_CODE}
    fi
}

cmake_clean() {
    # Run cmake to clean up
    cmake --build "${CMAKE_BINARY_DIR}" --target clean
    EXIT_CODE=${?}

    if [ ${EXIT_CODE} -ne 0 ]; then
        log_err "Couldn't clean up!"
        exit ${EXIT_CODE}
    fi
}
