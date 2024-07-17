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

/* Basic programming */
#include <dirent.h>
#include <errno.h>
#include <pcre.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#ifndef _WIN32
# include <sys/stat.h>
#endif

#ifndef _WIN32
# include <unistd.h>
#endif

#ifdef _WIN32
# include <Windows.h>
#else
# include <wordexp.h>
#endif

/* OAWP created header files */
#include "dir-handler.h"
#include "log.h"

uint8_t format_path(char dst[PATH_MAX], const char *restrict src) {

    /* PCRE pre-requisites */
    pcre *regex;
    const char *error;
    int erroffset;
    int ovector[2];
    int rc;

    strcpy(dst, src);

    /* Compile the regex pattern */
    regex = pcre_compile("^file://", 0, &error, &erroffset, NULL);
    if (!regex) {
        log_fatal("PCRE compilation failed at offset %d: %s",
                  erroffset, error);
        exit(EXIT_FAILURE);
    }

    /* Execute the regex */
    rc = pcre_exec(regex,
                   NULL,
                   src,
                   strlen(src),
                   0,
                   0,
                   ovector,
                   sizeof(ovector)/sizeof(int));
    if (rc >= 0) {
        /* If "file://" found at the beginning, remove it */
        memmove(dst, src + ovector[1], strlen(src) - ovector[1] + 1);
    }
    else if (rc == PCRE_ERROR_NOMATCH) { }
    else {
        log_fatal("PCRE matching failed: %d", rc);
        exit(EXIT_FAILURE);
    }

    /* Deallocated PCRE regex */
    pcre_free(regex);

    #ifdef _WIN32

    TCHAR expanded_src[MAX_PATH];
    if (ExpandEnvironmentStrings(dst, expanded_src, MAX_PATH) == 0) {
        log_error("Cannot expand src `%s'", src)
        exit(EXIT_FAILURE);
    }
    strcpy(dst, expanded_src);

    #else

    if(src[0] == '~' && getenv("HOME") == NULL) {
        log_fatal("You are homeless. No, I'm serious! There is no HOME environment variable: %x", getenv("HOME")[0]);
        exit(EXIT_FAILURE);
    }

    wordexp_t p;
    int result = wordexp(dst, &p, 0);

    if (result != 0) {
        log_fatal("wordexp failed");
        exit(EXIT_FAILURE);
    }

    if (p.we_wordc <= 0) {
        log_fatal("wordexp produced no output");
        exit(EXIT_FAILURE);
    }

    if (strlen(p.we_wordv[0]) >= PATH_MAX) {
        log_error("Path length exceeds the %d character limit.", PATH_MAX);
        exit(EXIT_FAILURE);
    }
    strcpy(dst, *p.we_wordv);

    wordfree(&p);

    #endif

    return 0;
}

uint8_t verify_dir_path(const char path[PATH_MAX]) {

    #ifdef _WIN32
    DWORD dw_attrib = GetFileAttributesA(path);

    if (dw_attrib != INVALID_FILE_ATTRIBUTES && (dw_attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        /* Directory config dir exists */
        return 0;
    }
    else {
        char tmp_str[PATH_MAX];
        char *ppath = NULL;
        size_t len;

        /* Copy path to tmp_str */
        snprintf(tmp_str, sizeof(tmp_str), "%s", path);
        len = strlen(tmp_str);

        /* If the path ends with '\', replace it with a NULL terminator */
        if(tmp_str[len - 1] == '\\')
            tmp_str[len - 1] = '\0';

        //TODO:FIXME: Figure out what functions to use to mkdir and see if a dir exists in MS Windows

        /* Iterate over all characters.
         *
         * If there is a '/', temporarily replace it with a NULL terminator,
         * create the directory and replace back the '/'.
         */
        for(ppath = tmp_str + 1; *ppath; ppath++) {
            if(*ppath == '\\') {
                *ppath = '\0';
                //TODO
                //if(mkdir(tmp_str, S_IRWXU) != 0) {
                //    if(errno != EEXIST) {
                //        log_error("Failed to create directory `%s': %s", tmp_str, strerror(errno));
                //        return 1;
                //    }
                //}
                *ppath = '\\';
            }
        }
        //TODO
        /* Finally, create the final target directory */
        //if(mkdir(tmp_str, S_IRWXU) != 0) {
        //    if(errno != EEXIST) {
        //        log_error("Failed to create directory `%s': %s\n", tmp_str, strerror(errno));
        //        return 1;
        //    }
        //}
    }

    #else

    if (access(path, F_OK) == 0) {
        /* Default config dir exists */
        return 0;
    }
    else {
        char tmp_str[PATH_MAX];
        char *ppath = NULL;
        size_t len;

        /* Copy path to tmp_str */
        snprintf(tmp_str, sizeof(tmp_str), "%s", path);
        len = strlen(tmp_str);

        /* If the path ends with '/', replace it with a NULL terminator */
        if(tmp_str[len - 1] == '/')
            tmp_str[len - 1] = '\0';

        /* Iterate over all characters.
         *
         * If there is a '/', temporarily replace it with a NULL terminator,
         * create the directory and replace back the '/'.
         */
        for(ppath = tmp_str + 1; *ppath; ppath++) {
            if(*ppath == '/') {
               *ppath = '\0';
                if(mkdir(tmp_str, S_IRWXU) != 0) {
                    if(errno != EEXIST) {
                        log_error("Failed to create directory `%s': %s", tmp_str, strerror(errno));
                        return 1;
                    }
                }
                *ppath = '/';
            }
        }

        /* Finally, create the final target directory */
        if(mkdir(tmp_str, S_IRWXU) != 0) {
            if(errno != EEXIST) {
                log_error("Failed to create directory `%s': %s\n", tmp_str, strerror(errno));
                return 1;
            }
        }
    }

    #endif

    return 0;
}

static int compare_fun (const void *restrict p, const void *restrict q) {
  /* compare_fun() and some code from im_paths_get() from
   * https://www.linuxquestions.org/questions/programming-9/how-to-list-and-sort-files-in-some-directory-by-the-names-on-linux-win-in-c-4175555160/
   * by NevemTeve - Thank you NevemTeve
   */

  const char *l = (const char*)p;
  const char *r = (const char*)q;
  int cmp;

  cmp = strcmp(l, r);
  return cmp;
}


uint8_t im_paths_get(const char str[PATH_MAX], ImPaths *restrict im_paths) {
  /* This function serves for saving the images paths from a
   * choosen directory to a dynamically allocated array of
   * pointers, pointers pointing to the string of path
   * I choosed this way because a normal initialized array
   * would use much more memory, Imlib uses a lot of memory
   * anyway.
   *
   * Most file systems have their first files as . and ..,
   * current directory and previous directory respectively
   * which shouldn't be part of the image loading, so these won't be included
   */

  char tmp_path[PATH_MAX];
  char tmp_path2[PATH_MAX];
  format_path(tmp_path, str);

  DIR *d;
  struct dirent *dir;

  d = opendir(tmp_path);

  int temp = 0;
 
  if(! d) {
      log_fatal("Something went wrong when reading directory %s", tmp_path);
      exit(EXIT_FAILURE);
  }

  /* Check if this str path ends with '/' */
  size_t str_len = strlen(tmp_path);
  if(tmp_path[str_len - 1 ] != '/') {
      if(str_len < PATH_MAX)
          tmp_path[str_len] = '/';
      else {
          log_error("Animation path is too big and exceedes system's maximum path length: %d characters long", PATH_MAX);
          exit(EXIT_FAILURE);
      }
  }

  while((dir = readdir(d)) != NULL) {
      /* Now check if there are any "." and ".." files in path
                                * in order to know where the actual images start     */
      if(strcmp(dir->d_name, ".") == 0) {
          log_debug("\"%s\" has current directory file, skipping it.", tmp_path);
          continue;
      }
      else if(strcmp(dir->d_name, "..") == 0) {
          log_debug("\"%s\" has parent directory file, skipping it.", tmp_path);
          continue;
      }

      size_t tmp_path_len = strlen(tmp_path);
      size_t dir_name_len = strlen(dir->d_name);

      if (tmp_path_len + dir_name_len + 1 <= PATH_MAX) { // Check total length
          strncpy(tmp_path2, tmp_path, PATH_MAX - 1); // Copy tmp_path to tmp_path2
          tmp_path2[PATH_MAX - 1] = '\0'; // Ensure null-terminated

          strncat(tmp_path2, dir->d_name, PATH_MAX - tmp_path_len - 1); // Append dir->d_name to tmp_path2
          tmp_path2[PATH_MAX - 1] = '\0'; // Ensure null-terminated

          im_paths_push(tmp_path2, im_paths);
      } else {
          log_error("Animation path is too big and exceedes system's maximum path length: %d characters long", PATH_MAX);
          exit(EXIT_FAILURE);
      }
  }
  closedir(d);

  // readdir() dumps mixed files, so qsort will sort alphabetically */
  im_paths_sort(im_paths);

  /* Prints all the selected files */
  log_debug("Selected files:");
  im_paths_index_reset(im_paths); // Reset index

  for(uint64_t i = 1; i < im_paths->image_count; i++) {
      log_debug("  | File %d: %s", i, im_paths->p_index->im_path);
      if(! im_paths_next(im_paths)) { // Go to next image
          log_fatal("Reached end of list before image count");
          exit(EXIT_FAILURE);
      }
  }

  im_paths_index_reset(im_paths); // Reset index
  log_debug("  | ** End of files **\n");

  return 0;
}

uint8_t im_paths_init(ImPaths *restrict im_paths) {
    im_paths->image_count = 0;
    im_paths->p_list = NULL;
    im_paths->p_index = NULL;
    im_paths->p_end = NULL;

    return 0;
}

uint8_t im_paths_index_reset(ImPaths *restrict im_paths) {
    im_paths->p_index = im_paths->p_list;

    return 0;
}

uint8_t im_paths_push(const char *restrict str, ImPaths *restrict im_paths) {
    struct CharList *tmp_char_list;
    struct CharList *tmp_char_list2;

    tmp_char_list = (struct CharList*)malloc(sizeof(struct CharList));
    if (tmp_char_list == NULL) {
        log_fatal("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }

    strncpy(tmp_char_list->im_path, str, PATH_MAX - 1);
    tmp_char_list->im_path[PATH_MAX - 1] = '\0';
    tmp_char_list->p_next = NULL;

    if (im_paths->p_list == NULL) {
        im_paths->p_list = im_paths->p_index = im_paths->p_end = tmp_char_list;
    } else {
        tmp_char_list2 = im_paths->p_end;
        im_paths->p_end = tmp_char_list;
        tmp_char_list2->p_next = im_paths->p_end;
    }

    im_paths->image_count++;

    return 0;
}

uint8_t im_paths_next(ImPaths *restrict im_paths) {
    if(im_paths->p_index->p_next != NULL) {
        im_paths->p_index = im_paths->p_index->p_next;
        return 1; // If didn't reach end
    }
    else
        return 0; // If reached end
}

uint8_t im_paths_sort(ImPaths *restrict im_paths) {
    //TODO

    return 0;
}

uint8_t im_paths_free(ImPaths *restrict im_paths) {
    im_paths->p_index = im_paths->p_list;

    while(im_paths->p_index != NULL) {
        im_paths->p_list = im_paths->p_list->p_next;
        free(im_paths->p_index);
        im_paths->p_index = im_paths->p_list;
    }

    im_paths_init(im_paths);

    return 0;
}
