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

# Colors
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

# Formatting
BOLD="\033[1m"
UNDERLINE="\033[4m"

# Misc
ENDCOLOR="\033[0m" # Reset color and formatting

# Sequences [colored]
INFO="[${BLUE}I${ENDCOLOR}]"
WARN="[${YELLOW}W${ENDCOLOR}]"
ERR="[${RED}E${ENDCOLOR}]"

: "${LOG_FILE:="./logfile.log"}"

uncolor_sequence() {
    sed -E 's/\\\033\[[0-9;]*m//g'
}

log_info() {
    message="$1"
    timestamp="$(date +'%Y-%m-%d %H:%M:%S')"
    printf "%s %s: %s\n" "${INFO}" "${timestamp}" "${message}"
    printf "%s %s: %s\n" "${INFO}" "${timestamp}" "${message}" | uncolor_sequence >> "${LOG_FILE}"
}

log_warn() {
    message="$1"
    timestamp="$(date +'%Y-%m-%d %H:%M:%S')"
    printf "%s %s: %s\n" "${WARN}" "${timestamp}" "${message}"
    printf "%s %s: %s\n" "${WARN}" "${timestamp}" "${message}" | uncolor_sequence >> "${LOG_FILE}"
}

log_err() {
    message="$1"
    timestamp="$(date +'%Y-%m-%d %H:%M:%S')"
    printf "%s %s: %s\n" "${ERR}" "${timestamp}" "${message}" >&2
    printf "%s %s: %s\n" "${ERR}" "${timestamp}" "${message}" | uncolor_sequence >> "${LOG_FILE}"
}
