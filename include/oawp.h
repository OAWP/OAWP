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
#ifndef __OAWP_H__
# define __OAWP_H__

#include <stdbool.h>
#include <Imlib2.h>
#include "dir-handler.h"

#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 199309L
#endif

#ifdef DEBUG
/* If DEBUG was defined somewhere else as false, redefine here */
# if DEBUG == false
/* If you want DEBUG as true, define it only here in A and B, unless you know
 * what you are doing. The values between A and B must be the same! */
#  define DEBUG false  /* A - MUST be the same as B! */
# endif
#else
# define DEBUG false  /* B - MUST be the same as A! */
#endif

/* The default wait time between frame changes */
#ifndef DEFAULT_FRAME_TIME
# define DEFAULT_FRAME_TIME 0.07
#endif

/* The minimum wait time between frame changes OAWP can accept */
#ifndef MIN_FRAME_TIME
# define MIN_FRAME_TIME 0.001
#endif

/* The fit option OAWP should use by default */
#ifndef DEFAULT_FIT_OPTION
# define DEFAULT_FIT_OPTION "fit"
#endif

/* The default OAWP config file */
#ifndef DEFAULT_CONFIG_FILE_PATH
# define DEFAULT_CONFIG_FILE_PATH "~/.config/oawp/oawp.conf"
#endif

/* If _DEBUG is true, print debug info.
 * Note that _DEBUG is a variable that may be changed in runtime and DEBUG is a
 * defined macro */
extern bool _DEBUG;

typedef struct {
  Window root;
  Pixmap pixmap;
  Imlib_Context *render_context;
  int width, height;
} Monitor;

void help(void);
void version(void);

void freeUsingPath(void);
void setRootAtoms(Display *restrict display, Monitor *restrict monitor);
void ImFit(Imlib_Image *image[]);

#endif
