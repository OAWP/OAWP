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
. ./sh/chkroot.sh
. ./sh/logging.sh

# CONSTANTS
OAWP_CONFIG_DIR="./assets/.config/oawp/"

# Function to create .config directory for a user
create_config_dir() {
  username=${1}
  home_dir=$(eval echo "~${username}")
  config_dir="${home_dir}/.config/"

  if [ -d "${home_dir}" ] && [ ! -d "${config_dir}" ]; then
    mkdir -p "${config_dir}"
    chown "${username}:${username}" "${config_dir}"
    log_info "Created \`${config_dir}' for user \`${username}'."
  fi

  cp -r "${OAWP_CONFIG_DIR}" "${config_dir}"
  chown -R "${username}:${username}" "${config_dir}$(basename ${OAWP_CONFIG_DIR})"
  log_info "Copied OAWP config directory for user \`${username}'."
}

# Iterate over each user in /etc/passwd
while IFS=: read -r username _ uid _ home_dir _; do

  if [ "$(uname)" = "Darwin" ]; then
      # macOS normal users start from 501
      if [ "${uid}" -ge 501 ]; then
          create_config_dir "${username}"
      fi
  else
      if [ "${uid}" -ge 1000 ]; then
          create_config_dir "${username}"
      fi
  fi

done < /etc/passwd
