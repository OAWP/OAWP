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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "pixmanip.h"
#include "log.h"

void setFitOpts(int *restrict currentOptions, const int newOption) {

    // Check position flag conflicts
    if (newOption & POSITION_MASK) {
        log_error("Cannot use multiple positions at the same time!\n"
                  "Please make sure fit is configured correctly!");
        exit(EXIT_FAILURE);
    }
    // Check modifier flag conflicts
    if (newOption & FIT_MASK) {
        log_error("Cannot use multiple modifiers at the same time!\n"
                  "Please make sure fit is configured correctly!");
        exit(EXIT_FAILURE);
    }

    // Set the new option
    *currentOptions |= newOption;
}

int fit_atoe(const char *restrict fitOptStr) {

    int fitOpts = 0;
    char *token;
    char strCopy[256];

    /* Create a copy of the input string since strtok modifies the string */
    strncpy(strCopy, fitOptStr, sizeof(strCopy));
    strCopy[sizeof(strCopy) - 1] = '\0';

    /* Uppercase every char of fitOpt */
    for (uint32_t temp = 0; temp < strlen(strCopy); temp++)
        strCopy[temp] = toupper(strCopy[temp]);

    /* Tokenize the input string using space and comma as delimiters */
    token = strtok(strCopy, " ,");
    while (token != NULL) {
        if (strcmp(token, "FULLSCREEN") == 0) {
            setFitOpts(&fitOpts, _FIT_FULLSCREEN);
        } else if (strcmp(token, "CENTERED") == 0) {
            setFitOpts(&fitOpts, _FIT_CENTERED);
        } else if (strcmp(token, "TOP-LEFT") == 0) {
            setFitOpts(&fitOpts, _FIT_TOP_LEFT);
        } else if (strcmp(token, "BOTTOM-LEFT") == 0) {
            setFitOpts(&fitOpts, _FIT_BOTTOM_LEFT);
        } else if (strcmp(token, "BOTTOM-RIGHT") == 0) {
            setFitOpts(&fitOpts, _FIT_BOTTOM_RIGHT);
        } else if (strcmp(token, "TOP-RIGHT") == 0) {
            setFitOpts(&fitOpts, _FIT_TOP_RIGHT);
        } else if (strcmp(token, "CROPPED") == 0) {
            setFitOpts(&fitOpts, _FIT_CROPPED);
        } else if (strcmp(token, "STRETCHED") == 0) {
            setFitOpts(&fitOpts, _FIT_STRETCHED);
        } else {
            log_fatal("`%s' is not a valid fit option!\n"
                      "Please make sure fit is configured correctly!",
                      fitOptStr);
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, " ,");
    }

    return fitOpts;
}

void ImFit_fitOpts(Imlib_Image *image[], const int fitOpts) {
    /* This function is responsible for fitting the image when rendering depending
     * in user's arguments.
     *
     * This function gets passed a pointer to the Imlib images and uses a global
     * char* variable "fitOpt", which has the fit option.
     *
     * Right now, the available fit options are the following:
     *   * FULLSCREEN,
     *   * CENTERED,
     *   * TOP-LEFT,
     *   * BOTTOM-LEFT,
     *   * BOTTOM-RIGHT,
     *   * TOP-RIGHT,
     *   * CROPPED,
     *   * STRETCHED.
     * Despite these being written with uppercase letters, the fitOpt is not case
     * sensitive.
     *
     * ImFit_fitOpts() will also modify the loaded Imlib images based on fitOpt.
     */

    /* This function is still a TODO and this is it's pseudo-code: */
    // int imWidth   = Imlib_get_image_width(*sampleImg);
    // int imHeight  = Imlib_get_image_height(*sampleImg);

    /* Position */
    if ((fitOpts & _FIT_FULLSCREEN) != 0) {
        // Scale image based on width and height;
        // return position;
    }

    else if ((fitOpts & _FIT_CENTERED) != 0) {
        // Determine the center of the image and XScreen's width and height and then
        // determine where the position should start from with that info;
    }

    else if ((fitOpts & _FIT_TOP_LEFT) != 0) {
        // Return the position as 0,0 since no modifications are needed;
    }

    else if ((fitOpts & _FIT_BOTTOM_LEFT) != 0) {
        // Determine XScreen's height;
        // Return position as (Xscreen height - img height),0;
    }

    else if ((fitOpts & _FIT_BOTTOM_RIGHT) != 0) {
        // Determine XScreen's width and height
        // Return position as (XScreen height - img height),(XScreen width - img
        // width);
    }

    else if ((fitOpts & _FIT_TOP_RIGHT) != 0) {
        // Determine XScreen's width and height;
        // Return position as 0,(XScreen width - img width);
    }

    else {
        /* At least _FIT_CENTERED should be selected by default */
        log_fatal("Unknown fatal error!");
        exit(EXIT_FAILURE);
    }

    /* Modifiers */
    if ((fitOpts & _FIT_CROPPED) != 0) {

    }

    else if ((fitOpts & _FIT_STRETCHED) != 0) {
    }
}
