/*
 * Copyright (C) 2023-2024 TheRealOne78 <bajcsielias78@gmail.com>
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fancy-text.h"
#include "arg.h"
#include "oawp.h"
#include "pixmanip.h"
#include "log.h"


uint8_t argGetOpt(int argc, char **argv, params_t *parameters) {

    /* Struct argument options */
    static struct option long_options [] = {
        { "help"                , no_argument      , NULL , 'h' },
        { "time"                , required_argument, NULL , 't' },
        { "version"             , no_argument      , NULL , 'v' },
        { "debug"               , no_argument      , NULL , 'D' },
        { "fit"                 , required_argument, NULL , 'f' }, // Not implemented yet - This is a feature that OAWP will fit the photo based on user's requirements
        { "directory"           , required_argument, NULL , 'd' },
        { "config"              , required_argument, NULL , 'c' },
        { "set-static-wallpaper", required_argument, NULL , 'S' },
        { NULL                  , 0                , NULL , 0   }
    };
    /* And check for arguments */
    while(1) {
        int c = getopt_long(argc, argv, "ht:vDf:d:c:S:", long_options, NULL);
        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch(c) {
            /* help */
            case 'h':
                help();
                exit(0);
                break;

                /* time */
            case 't':
                //snprintf(configTime, sizeof(configTime), "%s", optarg); TODO: remove this?
                parameters->frameTime = atof(optarg);
                if(parameters->frameTime < MIN_FRAME_TIME) {
                    log_error("Time cannot be less than %lf.", MIN_FRAME_TIME);
                    exit(1);
                }
                //TODO
                parameters->hasFrameTime = true;
                break;

                /* version */
            case 'v':
                version();
                exit(0);
                break;

                /* debug */
            case 'D':
                _DEBUG = true;
                log_debug("Enabled debug");
                break;

                /* fit */
            case 'f':
                printf("Fit is not implemented yet, skipping...");
                /* TODO: implement this fit and remove this break */
                break;
                /* part of Fit */
                //TODO make a function to transform optarg to enums
                parameters->fitOpt = fit_atoe(optarg);
                parameters->hasFitOpt = true;
                break;

                /* directory */
            case 'd':
                strcpy(parameters->imDirPath, optarg);
                getImgCount(parameters->imDirPath);
                getImgPath(parameters->imDirPath, 1);
                parameters->hasImDirPath = true;
                break;

                /* config */
            case 'c':
                if(access(optarg, F_OK) != 0) {
                    log_error("%s configuration file does not exist.", optarg);
                    exit(1);
                }
                if(access(optarg, R_OK) != 0) {
                    log_error("%s configuration file cannot be read. Please check the file permissions.", optarg);
                    exit(1);
                }
                strcpy(parameters->confPath, optarg);
                parameters->hasConfPath = true;
                break;

                /* set-static-wallpaper */
            case 'S':
                if(access(optarg, F_OK) != 0) {
                    log_error("%s from 'static-wallpaper' does not exists.", optarg);
                    exit(1);
                }
                if(access(optarg, R_OK) != 0) {
                    log_error("%s from 'static-wallpaper' cannot be read. Please check the file permissions.", optarg);
                    exit(1);
                }
                /*
                 * TODO implement this in main function instead
                 * imgPath = (char**)malloc(1 * sizeof(char*));
                 * imgPath[0] = (char*)malloc(PATH_MAX * sizeof(char));
                 * strcpy(imgPath[0], optarg);
                 * imgCount++;
                 * usingStaticWallpaper = true;
                 */
                parameters->hasStaticWallpaper = true;
                break;

            case '?':
                /* No need to print and error message because
                   getopt_long did that already. */
                exit(EXIT_FAILURE);
                break;

            default:
                abort();
        }
    }

    return 0; /* EXIT-VALUE: Everything OK */
}

static int compare_fun (const void *p, const void *q) {
    /* compare_fun() and some code from getImgPath() from
     * https://www.linuxquestions.org/questions/programming-9/how-to-list-and-sort-files-in-some-directory-by-the-names-on-linux-win-in-c-4175555160/
     * by NevemTeve - Thank you NevemTeve
     * This function is mandatory for qsort to be able to know
     * what approach to use to sort the images              */

    const char *l = *(const char**)p;
    const char *r = *(const char**)q;
    int cmp;

    cmp = strcmp(l, r);
    return cmp;
}
