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

#ifndef __ARG_H__
#define __ARG_H__

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct params {
    /*** VALUES ***/
    char im_dir_path[PATH_MAX];       /* Path to images directory */
    char conf_path[PATH_MAX];         /* Path to configuration file to use */
    double frame_time;                /* Time between frames */
    char static_wallpaper[PATH_MAX];  /* Path to static wallpaper */
    int fit_opt;                      /* The fit option */
    int debug;                        /* debug */

    /*** BOOLEANS of value existence ***/
    bool has_im_dir_path;             /* if true, this type has a directory value */
    bool has_conf_path;               /* if true, the configuration file from argument will be used */
    bool has_frame_time;              /* if true, this type has a time value */
    bool has_static_wallpaper;        /* if true, this type has a staticwallpaper value */
    bool has_fit_opt;                 /* if true, this type has a fit option value */
    bool has_debug;
} params_t;


/**
 * @brief Populate parameters based on argument options.
 *
 * @param argc argc from main()
 * @param argv argv from main()
 * @param parameters params_t to be populated
 *
 * @return 0 if everything is okay.
 */
uint8_t arg_get_opt(const int argc, const char **argv, params_t *restrict parameters);

#endif // __ARG_H__
