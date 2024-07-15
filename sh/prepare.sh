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

# Basic global variables
PKG_MGR=""          # The package manager that will install the dependencies

check_deps() {
  # List of dependencies
  # Common Linux: gcc make  imlib2        libx11       libconfig
  # Debian:       gcc make  libimlib2-dev libx11-dev   libconfig-dev
  # RHEL:         gcc make  imlib2-devel  libX11-devel libconfig-devel
  # BSD:          gcc gmake imlib2        libx11       libconfig"

  # Checking existing dependencies
  if [ ! -x "$(command -v gcc)" ]; then
    log_info "${GREEN}gcc${ENDCOLOR} not detected, adding it in the dependencies install queue"
    DEPENDENCIES="$DEPENDENCIES gcc"
    DEB_DEPENDENCIES="$DEB_DEPENDENCIES gcc"
    RPM_DEPENDENCIES="$RPM_DEPENDENCIES gcc"
    BSD_DEPENDENCIES="$BSD_DEPENDENCIES gcc"
  fi

  if [ ! -x "$(command -v make)" ]; then
    log_info "${GREEN}make${ENDCOLOR} not detected, adding it in the dependencies install queue"
    DEPENDENCIES="$DEPENDENCIES make"
    DEB_DEPENDENCIES="$DEB_DEPENDENCIES make"
    RPM_DEPENDENCIES="$RPM_DEPENDENCIES make"
    BSD_DEPENDENCIES="$BSD_DEPENDENCIES gmake"
  fi

  if [ -x "$(pkg-config imlib2)" ] || [ ! -f "/usr/include/Imlib2.h" ]; then
    log_info "${GREEN}imlib2${ENDCOLOR} not detected, adding it in the dependencies install queue"
    DEPENDENCIES="$DEPENDENCIES imlib2"
    DEB_DEPENDENCIES="$DEB_DEPENDENCIES libimlib2-dev"
    RPM_DEPENDENCIES="$RPM_DEPENDENCIES imlib2-devel"
    BSD_DEPENDENCIES="$BSD_DEPENDENCIES imlib2"
  fi

  if [ -x "$(pkg-config x11)" ] || [ ! -d "/usr/include/X11" ]; then
    log_info "${GREEN}libx11${ENDCOLOR} not detected, adding it in the dependencies install queue"
    DEPENDENCIES="$DEPENDENCIES libx11"
    DEB_DEPENDENCIES="$DEB_DEPENDENCIES libx11-dev"
    RPM_DEPENDENCIES="$RPM_DEPENDENCIES libx11-devel"
    BSD_DEPENDENCIES="$BSD_DEPENDENCIES libx11"
  fi
 
  if [ -x "$(pkg-config libconfig)" ] || [ ! -f "/usr/include/libconfig.h" ]; then
  log_info "${GREEN}libconfig${ENDCOLOR} not detected, adding it in the dependencies install queue"
  DEPENDENCIES="$DEPENDENCIES libconfig"
  DEB_DEPENDENCIES="$DEB_DEPENDENCIES libconfig-dev"
  RPM_DEPENDENCIES="$RPM_DEPENDENCIES libconfig-devel"
  BSD_DEPENDENCIES="$BSD_DEPENDENCIES libconfig"
  fi

  if [ -z "${DEPENDENCIES}" ]; then # No dependencies to install
    return 0
  fi

  DEB_DEPENDENCIES=$(echo "${DEB_DEPENDENCIES}" | sed 's/^\ //')
  DEPENDENCIES=$(echo "${DEPENDENCIES}" | sed 's/^\ //')
  RPM_DEPENDENCIES=$(echo "${RPM_DEPENDENCIES}" | sed 's/^\ //')
  BSD_DEPENDENCIES=$(echo "${BSD_DEPENDENCIES}" | sed 's/^\ //')

  return 1 # There are dependencies to install
}

pkgmgr_finder() {
  # Choosing the right package manager
  ## Linux distributions
  ### Debian
  if [ -x "$(command -v apt-get)" ]; then
    log_info "${BOLD}apt-get${ENDCOLOR} package manager detected."
    PKG_MGR="apt-get"

  ### Arch
  elif [ -x "$(command -v pacman)" ]; then
    log_info "${BOLD}pacman${ENDCOLOR} package manager detected."
    PKG_MGR="pacman"

  ### Fedora
  elif [ -x "$(command -v dnf)" ]; then
    log_info "${BOLD}dnf${ENDCOLOR} package manager detected."
    PKG_MGR="dnf"

  ### CentOS - imlib2 lib/devel does not exist
  #elif [ -x "$(command -v yum)" ]; then
  #  log_info "{BOLD}yum${ENDCOLOR} package manager detected."
  #  PKG_MGR="yum"

  ### Gentoo
  elif [ -x "$(command -v emerge)" ]; then
    log_err "Portage detected!"
    log_err "Automatic package instalation with portage may lead to package conflicts."
    log_err "Please install ${GREEN}${DEPENDENCIES}${ENDCOLOR} manually and run this file again to compile and install OAWP in your system!"
    log_err "To install these dependencies, try \"sudo emerge --ask ${DEPENDENCIES}\"."
    log_err "For more info, see <https://wiki.gentoo.org/wiki/Emerge> and <https://wiki.gentoo.org/wiki/Portage>."
    exit 1

  ## BSD Family
  ### FreeBSD
  elif [ -x "$(command -v pkg)" ]; then
    log_info "${BOLD}pkg${ENDCOLOR} package manager detected."
    PKG_MGR="pkg"

  ### OpenBSD
  elif [ -x "$(command -v pkg_add)" ]; then
    log_info "${BOLD}pkg_add${ENDCOLOR} detected."
    log_warn "If you are using OpenBSD, you might not have a good desktop experience because of OpenBSD's lack of graphics drivers support for nVidia GPUs."
    # TODO:FIXME: Is that still the case with open-kernel or GSP_firmware+KVM?
    PKG_MGR="pkg_add"

  ### NetBSD
  elif [ -x "$(command -v pkgin)" ]; then
    log_info "${BOLD}pkgin${ENDCOLOR} package manager detected."
    PKG_MGR="pkgin"

  ### No package manager
  else
    log_err "A valid package manager was not found!"
    log_err "Please install ${DEPENDENCIES} manually and run this file again to compile and install XAWP in your system!"
    exit 1
  fi

  log_info "${GREEN}${PKG_MGR}${ENDCOLOR} will install the required dependencies"
}

pkgmgr_install() {
  # Install dependencies
  if [ "${PKG_MGR}" = "apt-get" ]; then
    "${PKG_MGR}" install -y ${DEB_DEPENDENCIES}
  ## Arch
  elif [ "${PKG_MGR}" = "pacman" ]; then
    "${PKG_MGR}" -Sy --noconfirm ${DEPENDENCIES}
  ## RedHat
  elif [ "${PKG_MGR}" = "dnf" ]; then
    "${PKG_MGR}" install -y ${RPM_DEPENDENCIES}
  ## FreeBSD
  elif [ "${PKG_MGR}" = "pkg" ]; then
    "${PKG_MGR}" install -y ${BSD_DEPENDENCIES}
  ## OpenBSD
  elif [ "${PKG_MGR}" = "pkg_add" ]; then
    "${PKG_MGR}" ${BSD_DEPENDENCIES}
  ## NetBSD
  elif [ "${PKG_MGR}" = "pkgin" ]; then
    "${PKG_MGR}" -y install ${BSD_DEPENDENCIES}
  fi
  EXIT_CODE=${?}

  if [ "${EXIT_CODE}" -ne 0 ]; then
    log_err "${PKG_MGR} returned with exit code ${EXIT_CODE}, aborting!"
    exit ${EXIT_CODE}
  fi

  log_info "Packages installed successfully!"
}

# Check the dependencies
check_deps

# Check if skip install
EXIT_CODE=${?}
if [ ${EXIT_CODE} -eq 0 ]; then
  log_info "No dependencies to install, skipping prepare.sh ..."
  return ${EXIT_CODE}

elif [ ${EXIT_CODE} -ne 1 ]; then
  log_err "Unknown fatal error: check_deps() returned ${EXIT_CODE}!"
  exit ${EXIT_CODE}
fi

# Check package managers
pkgmgr_finder

# Install missing dependencies
pkgmgr_install
