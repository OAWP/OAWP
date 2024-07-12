/*
 * Copyright (C) 2023-2024 TheRealOne78 <bajcsielias78@gmail.com>
 *
 * This file is part of the OAWP project
 *
 * OAWP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OAWP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OAWP. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "fit.h"
#include "fancy-text.h"

int8_t fit_atoe(char *fitOpt) {

  for(uint32_t temp = 0; temp < strlen(fitOpt); temp++) /* Uppercase every char of fitOpt */
    fitOpt[temp] = toupper(fitOpt[temp]);

  if(strcmp(fitOpt, "FULLSCREEN") == 0) {
      return _FIT_FULLSCREEN;
  }

  else if(strcmp(fitOpt, "FULLSCREEN CROPPED") == 0) {
      return _FIT_FULLSCREEN_CROPPED;
  }

  else if(strcmp(fitOpt, "CENTERED") == 0) {
      return _FIT_CENTERED;
  }

  else if(strcmp(fitOpt, "TOP-LEFT") == 0) {
      return _FIT_TOP_LEFT;
  }

  else if(strcmp(fitOpt, "BOTTOM-LEFT") == 0) {
      return _FIT_BOTTOM_LEFT;
  }

  else if(strcmp(fitOpt, "BOTTOM-RIGHT") == 0) {
      return _FIT_BOTTOM_RIGHT;
  }

  else if(strcmp(fitOpt, "TOP-RIGHT") == 0) {
      return _FIT_TOP_RIGHT;
  }

  else {
    fprintf(stderr, ERR_TEXT_PUTS"Fatal error! %s is not a valid fit option!\n"
                    "Please make sure fit is configured correctly\n",
            fitOpt);
    exit(EXIT_FAILURE);
  }

}
