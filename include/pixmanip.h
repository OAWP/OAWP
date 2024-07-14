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

#ifndef __PIXMANIP_H__
#define __PIXMANIP_H__

#include <Imlib2.h>

enum fitOpt {
  /* Position flags (mutually exclusive) */
  _FIT_FULLSCREEN   = 1 << 0,
  _FIT_CENTERED     = 1 << 1,
  _FIT_TOP_LEFT     = 1 << 2,
  _FIT_BOTTOM_LEFT  = 1 << 3,
  _FIT_BOTTOM_RIGHT = 1 << 4,
  _FIT_TOP_RIGHT    = 1 << 5,

  /* Fitting flags (mutually exclusive) */
  _FIT_CROPPED   = 1 << 6,
  _FIT_STRETCHED = 1 << 7
};

/* Mutual exclusion masks */
const static int POSITION_MASK = _FIT_FULLSCREEN | _FIT_CENTERED | _FIT_TOP_LEFT |
                                 _FIT_BOTTOM_LEFT | _FIT_BOTTOM_RIGHT | _FIT_TOP_RIGHT;
const static int FIT_MASK = _FIT_CROPPED | _FIT_STRETCHED;

/** Sets fit options while checkig for conflicts */
void setFitOpts(int *restrict currentOptions, const int newOption);

/** Transforms strings like "BOTTOM-RIGHT STRETCHED" to an integer */
int fit_atoe(const char *restrict fitOpt);

/** Manipulates frames with Imlib2 */
void ImFit_fitOpts(Imlib_Image *image[], const int fitOpts);

#endif // __PIXMANIP_H__
