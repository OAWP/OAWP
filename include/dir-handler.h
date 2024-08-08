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
#define __DIR_HANDLER_H__

#include <stdint.h>

/// Get the maximum path size based of different operating systems
#ifndef PATH_MAX
#    ifdef _WIN32   // Windows
#        include <windef.h>
#        define PATH_MAX MAX_PATH
#    elif __linux__ // Linux
#        include <linux/limits.h>
#    else           // Any *nix
#        include <limits.h>
#    endif
#endif /* PATH_MAX */


/// Contains paths for all the images to render
typedef struct {
  uint64_t image_count; // Number of images
  struct CharList {
    char im_path[PATH_MAX];
    struct CharList* p_next;
  } *p_list;
  struct CharList *p_index;
  struct CharList *p_end;
} ImPaths;

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
uint8_t format_path(char dst[PATH_MAX], const char *restrict src);

/**
 * @brief Check if the directory exists.  If not, create them.
 *
 * @param path Path to check.
 *
 * @note Thanks to OpenAI's ChatGPT for all the help!
 *
 * @return 0 if everything is alright, 1 if errors occured.
 */
uint8_t verify_dir_path(const char path[PATH_MAX]);

uint8_t im_paths_get(const char str[PATH_MAX], ImPaths *restrict im_paths);

uint8_t im_paths_init(ImPaths *restrict im_paths);
uint8_t im_paths_index_reset(ImPaths *restrict im_paths);
uint8_t im_paths_push(const char *restrict str, ImPaths *restrict im_paths);
uint8_t im_paths_next(ImPaths *restrict im_paths);

static struct CharList* _im_paths_get_pos_ptr(struct CharList *restrict char_list, uint64_t pos, uint64_t elements);
static struct CharList* _im_paths_sorted_merge(struct CharList *a, struct CharList *b);
static struct CharList* _im_paths_merge_sort(struct CharList *restrict char_list, uint64_t elements);
uint8_t im_paths_sort(ImPaths *restrict im_paths);

uint8_t im_paths_free(ImPaths *restrict im_paths);

#endif // __DIR_CHECKER_H__
