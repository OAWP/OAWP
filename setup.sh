#!/usr/bin/env sh

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

# SOURCE
. ./sh/logging.sh
. ./sh/cmake.sh
. ./sh/chkroot.sh # Check for ROOT id

# Install Dependencies
sh ./sh/prepare.sh
EXIT_CODE=${?}
if [ ${EXIT_CODE} -ne 0 ]; then
    exit ${EXIT_CODE};
fi

# Configure a CMake project
cmake_init

# Build the project
cmake_build

# Install the project in the system
cmake_install

## Clean the project
#cmake_clean

log_info "Done. Have a nice day!"
