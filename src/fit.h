/*
 * Copyright (C) 2023 TheRealOne78 <bajcsielias78@gmail.com>
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

#ifndef __FIT_H__
#define __FIT_H__

#include <Imlib2.h>

enum fitEnum {
_FIT_FULLSCREEN,
_FIT_FULLSCREEN_CROPPED,
_FIT_CENTERED,
_FIT_TOP_LEFT,
_FIT_BOTTOM_LEFT,
_FIT_BOTTOM_RIGHT,
_FIT_TOP_RIGHT
};

void ImFit(Imlib_Image *image[]);

int8_t fit_atoe(char *fitOpt);

#endif // __FIT_H__
