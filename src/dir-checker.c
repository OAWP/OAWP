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
#include <errno.h>
#include <pcre.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "dir-checker.h"
#include "log.h"

uint8_t formatPath(const char *restrict path, char formatted_path[PATH_MAX]) {
   
    /* PCRE pre-requisites */
    pcre *regex;
    const char *error;
    int erroffset;
    int ovector[2];
    int rc;

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
    if (ExpandEnvironmentStrings(path, expandedPath, MAX_PATH) == 0) {
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
    int result = wordexp(path, &p, 0);

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

int compare_fun (const void *restrict p, const void *restrict q) {
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
