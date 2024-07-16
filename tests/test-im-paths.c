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

#include "log.h"
#include "dir-handler.h"
#include <stdlib.h>

impaths_t* tmp;

int main(){

  tmp = (impaths_t*)malloc(sizeof(impaths_t));
    log_debug("Init");
    imPathsInit(tmp);
    log_debug("Debug 1");

    imPathsPush("Hello 1", tmp);
    log_debug("Debug 2");
    imPathsPush("Hello 2", tmp);
    log_debug("Debug 3");
    imPathsPush("Hello 3", tmp);
    log_debug("Debug 4");
    imPathsPush("Hello 4", tmp);
    log_debug("Debug 5");

    imPathsFree(tmp);
}
