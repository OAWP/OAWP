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
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dir-handler.h"
#include "fancy-text.h"
#include "arg.h"
#include "oawp.h"
#include "pixmanip.h"
#include "log.h"


uint8_t arg_get_opt(const int argc, const char **argv, params_t *restrict parameters) {

    char tmp_path[PATH_MAX];

    /* Struct argument options */
    static struct option long_options [] = {
        { "help"                , no_argument      , NULL , 'h' },
        { "time"                , required_argument, NULL , 't' },
        { "version"             , no_argument      , NULL , 'v' },
        { "debug"               , no_argument      , NULL , 'D' },
        { "fit"                 , required_argument, NULL , 'f' }, // Not implemented yet - This is a feature that OAWP will fit the image based on user's requirements
        { "directory"           , required_argument, NULL , 'd' },
        { "config"              , required_argument, NULL , 'c' },
        { "set-static-wallpaper", required_argument, NULL , 'S' },
        { NULL                  , 0                , NULL , 0   }
    };
    /* And check for arguments */
    while(1) {
        int c = getopt_long(argc, (char**)argv, "ht:vDf:d:c:S:", long_options, NULL);
        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch(c) {
            /* help */
            case 'h':
                help();
                exit(EXIT_SUCCESS);
                break;

                /* time */
            case 't':
                parameters->frame_time = atof(optarg);

                if(parameters->frame_time < MIN_FRAME_TIME) {
                    log_error("Time cannot be less than %lf.", MIN_FRAME_TIME);
                    exit(EXIT_FAILURE);
                }

                parameters->has_frame_time = true;
                break;

                /* version */
            case 'v':
                version();
                exit(EXIT_SUCCESS);
                break;

                /* debug */
            case 'D':
                parameters->debug = true;
                parameters->has_debug = true;
                break;

                /* fit */
            case 'f':
                parameters->fit_opt = fit_atoe(optarg);
                parameters->has_fit_opt = true;

                // TODO
                log_info("Fit is not implemented yet, skipping ...");
                break;

                /* directory */
            case 'd':
                strcpy(parameters->im_dir_path, optarg);

                // TODO: Remove this?
                //ImPaths a;
                //im_paths_init(&a);
                //im_paths_get(parameters->im_dir_path, &a);
                parameters->has_im_dir_path = true;
                break;

                /* config */
            case 'c':
                strcpy(parameters->conf_path, optarg);
                parameters->has_conf_path = true;
                break;

                /* set-static-wallpaper */
            case 'S':
                if(strlen(optarg) > PATH_MAX - 1) {
                    log_error("Static wallpaper path too long!");
                    exit(EXIT_FAILURE);
                }

                format_path(tmp_path, optarg);

                if(strlen(tmp_path) > PATH_MAX - 1) {
                    log_error("Static wallpaper path too long!");
                    exit(EXIT_FAILURE);
                }

                strncpy(parameters->static_wallpaper, tmp_path, PATH_MAX - 1);
                parameters->has_static_wallpaper = true;
                break;

            case '?':
                /*
                 * No need to print and error message because getopt_long did
                 * that already
                 */
                exit(EXIT_FAILURE);
                break;

            default:
                abort();
        }
    }

    return 0; /* EXIT-VALUE: Everything OK */
}
