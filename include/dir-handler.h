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

#ifndef __DIR_HANDLER_H__
# define __DIR_HANDLER_H__

#include <stdint.h>

/* Get the maximum path size based of
 * different operating systems. */
#ifndef PATH_MAX
  #ifdef __linux__
    #include <linux/limits.h>
  #elif BSD
    #include <limits.h>
  #elif __APPLE__
    #include <limits.h>
  #elif _WIN32
    #include <windef.h>
      #define PATH_MAX MAX_PATH
  #endif // __linux__
#endif // PATH_MAX


/// Contains paths for all the images to render
typedef struct impaths {
  uint64_t image_count; // Number of images
  struct charlist {
    char im_path[PATH_MAX];
    struct charlist* next_p;
  } *list;
  struct charlist *index;
  struct charlist *end;
} impaths_t;

/**
 * @brief Check if the first character is a '~'.
 * If so, this function will replace the '~' with a propper
 * "/home/user/" path.
 *
 * @param path Input path.
 * @param formatted_path Output formatted path.
 *
 * @return 0 if everything is alright.
 *
 * @note Thanks to OpenAI's ChatGPT for all the help!
 */
uint8_t formatPath(const char *restrict path, char formatted_path[PATH_MAX]);

/**
 * @brief Check if the directory exists.  If not, create them.
 *
 * @param path Path to check.
 *
 * @note Thanks to OpenAI's ChatGPT for all the help!
 *
 * @return 0 if everything is alright, 1 if errors occured.
 */
uint8_t verifyDirPath(const char path[PATH_MAX]);

static int compareFun (const void *restrict p, const void *restrict q);

// DEPRECATED
//uint8_t getImgCount(const char str[PATH_MAX], uint64_t *restrict img_count);

uint8_t getImgPath(const char str[PATH_MAX], impaths_t *restrict im_paths);

uint8_t imPathsInit(impaths_t *restrict im_paths);
uint8_t imPathsIndexReset(impaths_t *restrict im_paths);
uint8_t imPathsPush(const char *restrict str, impaths_t *restrict im_paths);
uint8_t imPathsNext(impaths_t *restrict im_paths);
uint8_t ImPathsSort(impaths_t *restrict im_paths);
uint8_t imPathsFree(impaths_t *restrict im_paths);

#endif // __DIR_CHECKER_H__
