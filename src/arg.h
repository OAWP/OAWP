/*
 * Copyright (C) 2023 TheRealOne78 <bajcsielias78@gmail.com>
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
    char imDirPath[PATH_MAX]; /* path to images directory */
    char confPath[PATH_MAX];  /* path to configuration file to use */
    double frameTime;         /* Time between frames */
    uint8_t fitOpt;           /* The final fit option */ //TODO: Create an enum for each fit option, instead of chars (oawp.h)

    bool hasConf;             /* If true, the configuration file from argument will be used */
    bool hasTime;             /* If true, this type has a time value */
    bool hasDir;              /* If true, this type has a directory value */
    bool hasStaticWallpaper;  /* If true, this type has a StaticWallpaper value */
    bool hasFit;              /* If true, this type has a Fit option value */
} params_t;

#endif // __ARG_H__
