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

#ifndef __DIR_CHECKER_H__
# define __DIR_CHECKER_H__

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
    #endif // MAX_PATH (windows)
  #endif // _WIN32
#endif // PATH_MAX


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
uint8_t formatPath(char *path, char formatted_path[PATH_MAX]);

/**
 * @brief Check if the directory exists.  If not, create them.
 *
 * @param path Path to check.
 *
 * @note Thanks to OpenAI's ChatGPT for all the help!
 *
 * @return 0 if everything is alright, 1 if errors occured.
 */
uint8_t verifyDirPath(char path[PATH_MAX]);
