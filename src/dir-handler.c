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

uint8_t formatPath(const char *restrict path, char formatted_path[PATH_MAX]) {
   
    /* PCRE pre-requisites */
    pcre *regex;
    const char *error;
    int erroffset;
    int ovector[2];
    int rc;

    strcpy(formatted_path, path);

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
                   path,
                   strlen(path),
                   0,
                   0,
                   ovector,
                   sizeof(ovector)/sizeof(int));
    if (rc >= 0) {
        /* If "file://" found at the beginning, remove it */
        memmove(formatted_path, path + ovector[1], strlen(path) - ovector[1] + 1);
    }
    else if (rc == PCRE_ERROR_NOMATCH) { }
    else {
        log_fatal("PCRE matching failed: %d", rc);
        exit(EXIT_FAILURE);
    }

    /* Deallocated PCRE regex */
    pcre_free(regex);

    #ifdef _WIN32

    TCHAR expandedPath[MAX_PATH];
    if (ExpandEnvironmentStrings(formatted_path, expandedPath, MAX_PATH) == 0) {
        log_error("Cannot expand path `%s'", path)
        exit(EXIT_FAILURE);
    }
    strcpy(formatted_path, expandedPath);

    #else

    if(path[0] == '~' && getenv("HOME") == NULL) {
        log_fatal("You are homeless. No, I'm serious! There is no HOME environment variable: %x", getenv("HOME")[0]);
        exit(EXIT_FAILURE);
    }

    wordexp_t p;
    int result = wordexp(formatted_path, &p, 0);

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
    strcpy(formatted_path, *p.we_wordv);

    wordfree(&p);

    #endif

    return 0;
}

uint8_t verifyDirPath(const char path[PATH_MAX]) {

    #ifdef _WIN32
    DWORD dwAttrib = GetFileAttributesA(path);

    if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        /* Directory config dir exists */
        return 0;
    }
    else {
        char tmpStr[PATH_MAX];
        char *ppath = NULL;
        size_t len;

        /* Copy path to tmpStr */
        snprintf(tmpStr, sizeof(tmpStr), "%s", path);
        len = strlen(tmpStr);

        /* If the path ends with '\', replace it with a NULL terminator */
        if(tmpStr[len - 1] == '\\')
            tmpStr[len - 1] = '\0';

        //TODO:FIXME: Figure out what functions to use to mkdir and see if a dir exists in MS Windows

        /* Iterate over all characters.
         *
         * If there is a '/', temporarily replace it with a NULL terminator,
         * create the directory and replace back the '/'.
         */
        for(ppath = tmpStr + 1; *ppath; ppath++) {
            if(*ppath == '\\') {
                *ppath = '\0';
                //TODO
                //if(mkdir(tmpStr, S_IRWXU) != 0) {
                //    if(errno != EEXIST) {
                //        log_error("Failed to create directory `%s': %s", tmpStr, strerror(errno));
                //        return 1;
                //    }
                //}
                *ppath = '\\';
            }
        }
        //TODO
        /* Finally, create the final target directory */
        //if(mkdir(tmpStr, S_IRWXU) != 0) {
        //    if(errno != EEXIST) {
        //        log_error("Failed to create directory `%s': %s\n", tmpStr, strerror(errno));
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
        char tmpStr[PATH_MAX];
        char *ppath = NULL;
        size_t len;

        /* Copy path to tmpStr */
        snprintf(tmpStr, sizeof(tmpStr), "%s", path);
        len = strlen(tmpStr);

        /* If the path ends with '/', replace it with a NULL terminator */
        if(tmpStr[len - 1] == '/')
            tmpStr[len - 1] = '\0';

        /* Iterate over all characters.
         *
         * If there is a '/', temporarily replace it with a NULL terminator,
         * create the directory and replace back the '/'.
         */
        for(ppath = tmpStr + 1; *ppath; ppath++) {
            if(*ppath == '/') {
               *ppath = '\0';
                if(mkdir(tmpStr, S_IRWXU) != 0) {
                    if(errno != EEXIST) {
                        log_error("Failed to create directory `%s': %s", tmpStr, strerror(errno));
                        return 1;
                    }
                }
                *ppath = '/';
            }
        }

        /* Finally, create the final target directory */
        if(mkdir(tmpStr, S_IRWXU) != 0) {
            if(errno != EEXIST) {
                log_error("Failed to create directory `%s': %s\n", tmpStr, strerror(errno));
                return 1;
            }
        }
    }

    #endif

    return 0;
}

static int compareFun (const void *restrict p, const void *restrict q) {
  /* compare_fun() and some code from getImgPath() from
   * https://www.linuxquestions.org/questions/programming-9/how-to-list-and-sort-files-in-some-directory-by-the-names-on-linux-win-in-c-4175555160/
   * by NevemTeve - Thank you NevemTeve
   * This function is mandatory for qsort to be able to know
   * what approach to use to sort the images              */

  const char *l = *(const char**)p;
  const char *r = *(const char**)q;
  int cmp;

  cmp = strcmp(l, r);
  return cmp;
}


uint8_t getImgPath(const char str[PATH_MAX], impaths_t *restrict im_paths) {
  /* This function serves for saving the images paths from a
   * choosen directory to a dynamically allocated array of
   * pointers, pointers pointing to the string of path
   * I choosed this way because a normal initialized array
   * would use much more memory, Imlib uses a lot of memory
   * anyway.
   *
   * Just like getImgCount(), getImgPath() gets choice
   * which is used to know where pImgCount and pImgPath
   * should point to: argument or configuration file
   *
   * readdir() mixes up the files order, so qsort is used
   *
   * Most partitions have their first files as . and ..,
   * current directory and previous directory respectively
   * which shouldn't be part of the image loading, so if
   * statements are used to know if the first 1-2 files
   * are or not . and ..                                  */

  //char** imgPath = (char**)malloc(img_count * sizeof(char*));

  char tmp_path[PATH_MAX];
  char tmp_path2[PATH_MAX];
  formatPath(str, tmp_path);

  DIR *d;
  struct dirent *dir;

  d = opendir(tmp_path);

  int temp = 0;

  if(d) {
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

            imPathsPush(tmp_path2, im_paths);
        } else {
            log_error("Animation path is too big and exceedes system's maximum path length: %d characters long", PATH_MAX);
            exit(EXIT_FAILURE);
        }
    }
    closedir(d);

    // readdir() dumps mixed files, so qsort will sort alphabetically */
    ImPathsSort(im_paths);
    //qsort(imgPath, img_count, sizeof((imgPath)[0]), compare_fun);

    /* Prints all the selected files */
    log_debug("Selected files:");
    imPathsIndexReset(im_paths); // Reset index

    for(uint64_t i = 1; i < im_paths->image_count; i++) {
        log_debug("  | File %d: %s", i, im_paths->index->im_path);
        if(! imPathsNext(im_paths)) { // Go to next image
            log_fatal("Reached end of list before image count");
            exit(EXIT_FAILURE);
        }
    }

    imPathsIndexReset(im_paths); // Reset index
    log_debug(  "  | ** End of files **\n");

  }

    return 0;
}

uint8_t imPathsInit(impaths_t *restrict im_paths) {
    im_paths->image_count = 0;
    im_paths->list = NULL;
    im_paths->index = NULL;
    im_paths->end = NULL;

    return 0;
}

uint8_t imPathsIndexReset(impaths_t *restrict im_paths) {
    im_paths->index = im_paths->list;

    return 0;
}

uint8_t imPathsPush(const char *restrict str, impaths_t *restrict im_paths) {
    struct charlist *tmp_char_list;
    struct charlist *tmp_char_list2;

    tmp_char_list = (struct charlist*)malloc(sizeof(struct charlist));
    if (tmp_char_list == NULL) {
        log_fatal("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }

    strncpy(tmp_char_list->im_path, str, PATH_MAX - 1);
    tmp_char_list->im_path[PATH_MAX - 1] = '\0';
    tmp_char_list->next_p = NULL;

    if (im_paths->list == NULL) {
        im_paths->list = im_paths->index = im_paths->end = tmp_char_list;
    } else {
        tmp_char_list2 = im_paths->end;
        im_paths->end = tmp_char_list;
        tmp_char_list2->next_p = im_paths->end;
    }

    im_paths->image_count++;

    return 0;
}

uint8_t imPathsNext(impaths_t *restrict im_paths) {
    if(im_paths->index->next_p != NULL) {
        im_paths->index = im_paths->index->next_p;
        return 1; // If didn't reach end
    }
    else
        return 0; // If reached end
}

uint8_t ImPathsSort(impaths_t *restrict im_paths) {
    //TODO

    return 0;
}

uint8_t imPathsFree(impaths_t *restrict im_paths) {
    im_paths->index = im_paths->list;

    while(im_paths->index != NULL) {
        im_paths->list = im_paths->list->next_p;
        free(im_paths->index);
        im_paths->index = im_paths->list;
    }

    imPathsInit(im_paths);

    return 0;
}
