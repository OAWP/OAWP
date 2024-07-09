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

# DEPRECATED

LDFLAGS = -lX11 -lImlib2 -lconfig -lm
CC = gcc
CFLAGS = -O2
SRC = ./src/*.c
BIN = oawp
BUILD_DIR = ./build/
CONF_DIR = ./.config/oawp/
INSTALL_DIR = /usr/bin/
CONF_FILE = oawp.conf

$(BIN):
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(BUILD_DIR)$(BIN)

install:
	install -t $(INSTALL_DIR) --owner=$(shell stat -c "%U" $(INSTALL_DIR)) --group=$(shell stat -c "%G" $(INSTALL_DIR)) -m 775 $(BUILD_DIR)$(BIN)

install-config:
	for f in /home/*/; do \
		install -d --owner=$$(stat -c "%U" $$f) --group=$$(stat -c "%G" $$f) -m 755 "$$f$(CONF_DIR).." ; \
		install -d --owner=$$(stat -c "%U" $$f) --group=$$(stat -c "%G" $$f) -m 755 "$$f$(CONF_DIR)" ; \
		install -t "$$f$(CONF_DIR)" --owner=$$(stat -c "%U" $$f) --group=$$(stat -c "%G" $$f) -m 755 "$(CONF_DIR)$(CONF_FILE)" ; \
	done

all: $(BIN) install install-config

uninstall:
	rm -f $(INSTALL_DIR)$(BIN)

clean:
	rm -rf $(BUILD_DIR)

TESTDIR = test/
test: $(BIN)
	$(BUILD_DIR)$(BIN)

.PHONY: all install uninstall clean
