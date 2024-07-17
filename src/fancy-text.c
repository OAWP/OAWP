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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fancy-text.h"
#include "info.h"
#include "log.h"

static bool _is_executable(const char *restrict executable) {
    char command[1024];
    snprintf(command, sizeof(command), "which %s > /dev/null 2>&1", executable);
    return (system(command) == 0);
}

void _puts_logo(const AwpLogo logo) {
    
    /* File to pipe to the *AWP output */
    FILE *p_rainbow_pipe = stdout;
    /*
     * Outputs *AWP ascii logo with a rainbow text manipulator, a tool to color
     * text on terminal with gradient.
     *
     * _Not a hard dependency_
     */
    
#ifdef ENABLE_COLORS
    /// If true, rainbow pipe needs to be closed
    bool will_close_p_rainbow_pipe = true;
    
    
    if (_is_executable("lcat-rs")) {
        //if (access("lcat-rs", X_OK) == 0) {
        log_debug("`lcat-rs' found in the system, opting for it to print *AWP logo");
        
        p_rainbow_pipe = popen("lcat-rs", "w");
        /* lcat-rs by `davidkna' - <https://github.com/davidkna/lcat-rs> */
    }
    
    else if (_is_executable("roflcat")) {
        //else if (access("roflcat", X_OK) == 0) {
        log_debug("`roflcat' found in the system, opting for it to print *AWP logo");
        
        p_rainbow_pipe = popen("roflcat", "w");
        /* roflcat by `jameslzhu' - <https://github.com/jameslzhu/roflcat> */
    }
    
    else if (_is_executable("lolcat")) {
        //else if (access("lolcat", X_OK) == 0) {
        log_debug("`lolcat' found in the system, opting for it to print *AWP logo");
        
        p_rainbow_pipe = popen("lolcat", "w");
        /* lolcat by `busyloop' - <https://github.com/busyloop/lolcat> */
    }
    else {
        log_debug("No rainbow text manipulator found in the system, printing raw *AWP logo");
        p_rainbow_pipe = stdout;
        will_close_p_rainbow_pipe = false;
    }
    
#endif
    
    switch(logo) {
        
        case LOGO_OAWP:
            /* Outputs OAWP ascii logo without gradient */
            fprintf(p_rainbow_pipe, "\n" /* print logo */
                    KBCYN"  /$$$$$$ " KBLU"  /$$$$$$ " KMAG" /$$      /$$" KBGRN" /$$$$$$$" RST"\n"
                    KBCYN" /$$__  $$" KBLU" /$$__  $$" KMAG"| $$  /$ | $$" KBGRN"| $$__  $$"RST"\n"
                    KBCYN"| $$  \\ $$" KBLU"| $$  \\ $$" KMAG"| $$ /$$$| $$" KBGRN"| $$  \\ $$"RST"\n"
                    KBCYN"| $$  | $$" KBLU"| $$$$$$$$" KMAG"| $$/$$ $$ $$" KBGRN"| $$$$$$$/"RST"\n"
                    KBCYN"| $$  | $$" KBLU"| $$__  $$" KMAG"| $$$$_  $$$$" KBGRN"| $$____/"RST"\n"
                    KBCYN"| $$  | $$" KBLU"| $$  | $$" KMAG"| $$$/ \\  $$$" KBGRN"| $$"RST"\n"
                    KBCYN"|  $$$$$$/" KBLU"| $$  | $$" KMAG"| $$/   \\  $$" KBGRN"| $$"RST"\n"
                    KBCYN" \\______/ " KBLU"|__/  |__/" KMAG"|__/     \\__/" KBGRN"|__/"RST"\n"
      );
      break;

#ifdef __APPLE__
    case LOGO_MAWP:
      /* Outputs MAWP ascii logo without gradient */
      fprintf(p_rainbow_pipe, "\n" /* print logo */
              KWHT
              " /$$      /$$" "  /$$$$$$ " " /$$      /$$" " /$$$$$$$""\n"
              "| $$$    /$$$" " /$$__  $$" "| $$  /$ | $$" "| $$__  $$""\n"
              "| $$$$  /$$$$" "| $$  \\ $$" "| $$ /$$$| $$" "| $$  \\ $$""\n"
              "| $$ $$/$$ $$" "| $$$$$$$$" "| $$/$$ $$ $$" "| $$$$$$$/""\n"
              "| $$  $$$| $$" "| $$__  $$" "| $$$$_  $$$$" "| $$____/""\n"
              "| $$\\  $ | $$" "| $$  | $$" "| $$$/ \\  $$$" "| $$""\n"
              "| $$ \\/  | $$" "| $$  | $$" "| $$/   \\  $$" "| $$""\n"
              "|__/     |__/" "|__/  |__/" "|__/     \\__/" "|__/"RST"\n"
      );
      break;

#elif _WIN32
      
    case LOGO_WINAWP:
      /* Outputs WinAWP ascii logo without gradient */
      fprintf(p_rainbow_pipe, "\n" /* print logo */
              KBCYN" /$$      /$$" " /$$" "          " KBWHT"  /$$$$$$ " " /$$      /$$" " /$$$$$$$"RST"\n"
              KBCYN"| $$  /$ | $$" "|__/" "          " KBWHT" /$$__  $$" "| $$  /$ | $$" "| $$__  $$"RST"\n"
              KBCYN"| $$ /$$$| $$" " /$$" " /$$$$$$$ " KBWHT"| $$  \\ $$" "| $$ /$$$| $$" "| $$  \\ $$"RST"\n"
              KBCYN"| $$/$$ $$ $$" "| $$" "| $$__  $$" KBWHT"| $$$$$$$$" "| $$/$$ $$ $$" "| $$$$$$$/"RST"\n"
              KBCYN"| $$$$_  $$$$" "| $$" "| $$  \\ $$" KBWHT"| $$__  $$" "| $$$$_  $$$$" "| $$____/"RST"\n"
              KBCYN"| $$$/ \\  $$$" "| $$" "| $$  | $$" KBWHT"| $$  | $$" "| $$$/ \\  $$$" "| $$"RST"\n"
              KBCYN"| $$/   \\  $$" "| $$" "| $$  | $$" KBWHT"| $$  | $$" "| $$/   \\  $$" "| $$"RST"\n"
              KBCYN"|__/     \\__/" "|__/" "|__/  |__/" KBWHT"|__/  |__/" "|__/     \\__/" "|__/"RST"\n"
      );
      break;

#else
      
    case LOGO_XAWP:
      /* Outputs XAWP ascii logo without gradient */
      fprintf(p_rainbow_pipe, "\n" /* print logo */
              KYEL" /$$   /$$"  KRED"  /$$$$$$ "  KMAG" /$$      /$$"  KBCYN" /$$$$$$$ "  RST"\n"
              KYEL"| $$  / $$"  KRED" /$$__  $$"  KMAG"| $$  /$ | $$"  KBCYN"| $$__  $$"  RST"\n"
              KYEL"|  $$/ $$/"  KRED"| $$  \\ $$"  KMAG"| $$ /$$$| $$"  KBCYN"| $$  \\ $$"  RST"\n"
              KYEL" \\  $$$$/ "  KRED"| $$$$$$$$"  KMAG"| $$/$$ $$ $$"  KBCYN"| $$$$$$$/"  RST"\n"
              KYEL"  >$$  $$ "  KRED"| $$__  $$"  KMAG"| $$$$_  $$$$"  KBCYN"| $$____/ "  RST"\n"
              KYEL" /$$/\\  $$"  KRED"| $$  | $$"  KMAG"| $$$/ \\  $$$"  KBCYN"| $$      "  RST"\n"
              KYEL"| $$  \\ $$"  KRED"| $$  | $$"  KMAG"| $$/   \\  $$"  KBCYN"| $$      "  RST"\n"
              KYEL"|__/  |__/"  KRED"|__/  |__/"  KMAG"|__/     \\__/"  KBCYN"|__/      "  RST"\n"
      );
      break;
     
    case LOGO_WAWP:
      /* Outputs WAWP ascii logo without gradient */
      fprintf(p_rainbow_pipe, "\n" /* print logo */
              /* KBYEL "\033[103m" - Yellow background, white foreground */
              KBWHT BBYEL " /$$      /$$" "  /$$$$$$ " " /$$      /$$" " /$$$$$$$ "RST"\n"
              KBWHT BBYEL "| $$  /$ | $$" " /$$__  $$" "| $$  /$ | $$" "| $$__  $$"RST"\n"
              KBWHT BBYEL "| $$ /$$$| $$" "| $$  \\ $$" "| $$ /$$$| $$" "| $$  \\ $$"RST"\n"
              KBWHT BBYEL "| $$/$$ $$ $$" "| $$$$$$$$" "| $$/$$ $$ $$" "| $$$$$$$/"RST"\n"
              KBWHT BBYEL "| $$$$_  $$$$" "| $$__  $$" "| $$$$_  $$$$" "| $$____/ "RST"\n"
              KBWHT BBYEL "| $$$/ \\  $$$" "| $$  | $$" "| $$$/ \\  $$$" "| $$      "RST"\n"
              KBWHT BBYEL "| $$/   \\  $$" "| $$  | $$" "| $$/   \\  $$" "| $$      "RST"\n"
              KBWHT BBYEL "|__/     \\__/" "|__/  |__/" "|__/     \\__/" "|__/      "RST"\n"
      );
      break;

#endif
  }

  /* Fush the rainbow pipe file */
  fflush(p_rainbow_pipe);

#ifdef ENABLE_COLORS
  /// Close p_rainbow_pipe in case it was opened
  if(will_close_p_rainbow_pipe)
      pclose(p_rainbow_pipe);
#endif

  /* After printing the logo, print the title and version as well */
  fprintf(stdout, KBWHT"Open Animated Wallpaper Player v%s"RST"\n\n", __VERSION_STR);


  /* ASCII art:
   *
   * [OAWP]
   *   /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$
   *  /$$__  $$ /$$__  $$| $$  /$ | $$| $$__  $$
   * | $$  \ $$| $$  \ $$| $$ /$$$| $$| $$  \ $$
   * | $$  | $$| $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   * | $$  | $$| $$__  $$| $$$$_  $$$$| $$____/
   * | $$  | $$| $$  | $$| $$$/ \  $$$| $$
   * |  $$$$$$/| $$  | $$| $$/   \  $$| $$
   *  \______/ |__/  |__/|__/     \__/|__/
   *
   * [XAWP]
   *  /$$   /$$  /$$$$$$  /$$      /$$ /$$$$$$$
   * | $$  / $$ /$$__  $$| $$  /$ | $$| $$__  $$
   * |  $$/ $$/| $$  \ $$| $$ /$$$| $$| $$  \ $$
   *  \  $$$$/ | $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   *   >$$  $$ | $$__  $$| $$$$_  $$$$| $$____/
   *  /$$/\  $$| $$  | $$| $$$/ \  $$$| $$
   * | $$  \ $$| $$  | $$| $$/   \  $$| $$
   * |__/  |__/|__/  |__/|__/     \__/|__/
   *
   * [WAWP]
   *  /$$      /$$  /$$$$$$  /$$      /$$ /$$$$$$$
   * | $$  /$ | $$ /$$__  $$| $$  /$ | $$| $$__  $$
   * | $$ /$$$| $$| $$  \ $$| $$ /$$$| $$| $$  \ $$
   * | $$/$$ $$ $$| $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   * | $$$$_  $$$$| $$__  $$| $$$$_  $$$$| $$____/
   * | $$$/ \  $$$| $$  | $$| $$$/ \  $$$| $$
   * | $$/   \  $$| $$  | $$| $$/   \  $$| $$
   * |__/     \__/|__/  |__/|__/     \__/|__/
   *
   * [MAWP]
   *  /$$      /$$  /$$$$$$  /$$      /$$ /$$$$$$$
   * | $$$    /$$$ /$$__  $$| $$  /$ | $$| $$__  $$
   * | $$$$  /$$$$| $$  \ $$| $$ /$$$| $$| $$  \ $$
   * | $$ $$/$$ $$| $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   * | $$  $$$| $$| $$__  $$| $$$$_  $$$$| $$____/
   * | $$\  $ | $$| $$  | $$| $$$/ \  $$$| $$
   * | $$ \/  | $$| $$  | $$| $$/   \  $$| $$
   * |__/     |__/|__/  |__/|__/     \__/|__/
   *
   * [WinAWP]
   *  /$$      /$$ /$$            /$$$$$$  /$$      /$$ /$$$$$$$
   * | $$  /$ | $$|__/           /$$__  $$| $$  /$ | $$| $$__  $$
   * | $$ /$$$| $$ /$$ /$$$$$$$ | $$  \ $$| $$ /$$$| $$| $$  \ $$
   * | $$/$$ $$ $$| $$| $$__  $$| $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   * | $$$$_  $$$$| $$| $$  \ $$| $$__  $$| $$$$_  $$$$| $$____/
   * | $$$/ \  $$$| $$| $$  | $$| $$  | $$| $$$/ \  $$$| $$
   * | $$/   \  $$| $$| $$  | $$| $$  | $$| $$/   \  $$| $$
   * |__/     \__/|__/|__/  |__/|__/  |__/|__/     \__/|__/
   *
   * ASCII art generated from patorjk.com/software/taag
   *
   * #-> bigmoney-ne : by nathan bloomfield (xzovik@gmail.com)
   * #-> based on art from the legendary MAKEMONEYFAST chain letter
   * #->
   * #-> History:
   * #->   5-30-2007 : first version (required characters only)
   * #->
   * #-> (end comments)                                           */
}

void puts_logo_auto(void) {
#ifdef __APPLE__
  _puts_logo(LOGO_MAWP);

#elif _WIN32
  _puts_logo(LOGO_MAWP);

#else
  if (getenv("DISPLAY") == NULL) {
    log_info("Couldn't find any active display.");
    _puts_logo(LOGO_OAWP);
    return;
  } else if (getenv("DISPLAY_WAYLAND") != NULL) {
    _puts_logo(LOGO_WAWP);
    return;
  }
  _puts_logo(LOGO_XAWP);

#endif
}

void help(void) {
  fprintf(stdout,
                                                                                          "\n"
         "OAWP - Open Animated Wallpaper Player"                                          "\n"
         "Play animated wallpapers"                                                       "\n"
                                                                                          "\n"
         "Usage: oawp [options]"                                                          "\n"
                                                                                          "\n"
         "Options:"                                                                       "\n"
         "-h, --help\t\t\t"              "Output this help list and exit"                 "\n"
         "-t, --time\t\t\t"              "Set the time OAWP needs to wait between the"    "\n"
         "\t\t\t\t"                      "change of images: --time seconds.milliseconds"  "\n"
                                                                                          "\n"
         "-v, --version\t\t\t"           "Output version information and license and exit""\n"
         "-D, --debug\t\t\t"             "Output the debug log"                           "\n"
         "-d, --directory\t\t\t"         "Set directory containing animation frames:"     "\n"

         #ifdef _WIN32
                                         "--directory "KBWHT"C:\\Users\\Foo\\OAWP\\wallgif\\"RST"\n"
         #else
         "\t\t\t\t"                      "--directory "KBWHT"/home/foo/wallgif/"RST       "\n"
         #endif

                                                                                          "\n"
         "-c, --config\t\t\t"            "Set another configuration file than the default""\n"

         #ifdef _WIN32
                                    KBWHT"%%APPDATA%%\\oawp\\oawp.conf"RST" configuration file\n"
         #else
         "\t\t\t\t"                 KBWHT"%s/.config/oawp/oawp.conf"RST" configuration file\n"
         #endif

                                                                                          "\n"
         "-S, --set-static-wallpaper\t"  "Set a static wallpaper and exit"                "\n"
                                                                                          "\n"
         "Note that OAWP uses a lot of system resources like RAM and CPU!"                "\n"
                                                                                          "\n"
        , getenv("HOME"));
}

void version(void) {
  fprintf(stdout,
         "OAWP version %s | git %s" /* version number */                                  "\n"
         "Source URI: <%s>"                                                               "\n"
                                                                                          "\n"
         "Copyright (C) 2023-2024 TheRealOne78"                                           "\n"
         "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.""\n"
         "This is free software: you are free to change and redistribute it."             "\n"
         "There is NO WARRANTY, to the extent permitted by law."                          "\n"
                                                                                          "\n"
        , __VERSION_STR, GIT_HASH_SHORT, SRC_URI);
}
