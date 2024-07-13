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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef _WIN32
# include <wordexp.h>
#endif

#ifdef _WIN32
# include <Windows.h>
#endif

/* OAWP created header files */
#include "dir-checker.h"
#include "log.h"

uint8_t formatPath(char *path, char formatted_path[PATH_MAX]) {

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

    wordfree(&p);


    if (strlen(p.we_wordv[0]) >= PATH_MAX) {
        log_error("Path length exceeds the %d character limit.", PATH_MAX);
        exit(EXIT_FAILURE);
    }
    strcpy(formatted_path, *p.we_wordv);

    #endif

    return 0;
}

uint8_t verifyDirPath(char path[PATH_MAX]) {

    #ifdef _WIN32
    //TODO: Handle paths in MS Windows
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
