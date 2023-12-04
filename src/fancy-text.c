/*
 * Copyright (C) 2023 TheRealOne78 <bajcsielias78@gmail.com>
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
#include "oawp.h"

void puts_logo(void) {

  FILE *rainbowPipe;
  bool isToCloseFile = true;

  #ifndef _WIN32

  /*
   * Outputs *AWP ascii logo with a rainbow text manipulator, a tool to color
   * text on terminal with gradient.
   *
   * This can be run only on *nix operating systems, thus the ifndef _WIN32
   * macro.
   *
   * _Not a hard dependency_
   */

  /* File to pipe to the *AWP output */

  if (access("lcat-rs", X_OK) == 0) {
    if(_DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": `lcat-rs' found in the system, opting for it to print *AWP logo");

    rainbowPipe = popen("lcat-rs", "w");
    /* lcat-rs by `davidkna' - <https://github.com/davidkna/lcat-rs> */
  }
  else if (access("roflcat", X_OK) == 0) {
    if(_DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": `roflcat' found in the system, opting for it to print *AWP logo");

    rainbowPipe = popen("roflcat", "w");
    /* roflcat by `jameslzhu' - <https://github.com/jameslzhu/roflcat> */
  }
  else if (access("lolcat", X_OK) == 0) {
    if(_DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": `lolcat' found in the system, opting for it to print *AWP logo");

    rainbowPipe = popen("lolcat", "w");
    /* lolcat by `busyloop' - <https://github.com/busyloop/lolcat> */
  }
  else {
    if(_DEBUG)
      fprintf(stdout, DEBUG_TEXT_PUTS": No rainbow text manipulator found in the system, printing raw *AWP logo");
    rainbowPipe = stdout;
    isToCloseFile = false;
  }
  #endif /* _WIN32 */

  /* Outputs XAWP ascii logo without gradient */
  fprintf(rainbowPipe, "\n" /* print logo */
    KYEL" /$$   /$$"  KRED"  /$$$$$$ "  KMAG" /$$      /$$"  KBCYN" /$$$$$$$ "  RST"\n"
    KYEL"| $$  / $$"  KRED" /$$__  $$"  KMAG"| $$  /$ | $$"  KBCYN"| $$__  $$"  RST"\n"
    KYEL"|  $$/ $$/"  KRED"| $$  \\ $$"  KMAG"| $$ /$$$| $$"  KBCYN"| $$  \\ $$"  RST"\n"
    KYEL" \\  $$$$/ "  KRED"| $$$$$$$$"  KMAG"| $$/$$ $$ $$"  KBCYN"| $$$$$$$/"  RST"\n"
    KYEL"  >$$  $$ "  KRED"| $$__  $$"  KMAG"| $$$$_  $$$$"  KBCYN"| $$____/ "  RST"\n"
    KYEL" /$$/\\  $$"  KRED"| $$  | $$"  KMAG"| $$$/ \\  $$$"  KBCYN"| $$      "  RST"\n"
    KYEL"| $$  \\ $$"  KRED"| $$  | $$"  KMAG"| $$/   \\  $$"  KBCYN"| $$      "  RST"\n"
    KYEL"|__/  |__/"  KRED"|__/  |__/"  KMAG"|__/     \\__/"  KBCYN"|__/      "  RST"\n"
  );

  /* After printing the logo, print the title and version as well */
  printf(KBWHT "Open Animated Wallpaper Player v%s\n\n" RST, VERSION);

  /* Fush the rainbow pipe file and close it unless it's stdout */
  fflush(rainbowPipe);
  if(isToCloseFile)
    fclose(rainbowPipe);


  /* ASCII art:
   *
   *  /$$   /$$  /$$$$$$  /$$      /$$ /$$$$$$$
   * | $$  / $$ /$$__  $$| $$  /$ | $$| $$__  $$
   * |  $$/ $$/| $$  \ $$| $$ /$$$| $$| $$  \ $$
   *  \  $$$$/ | $$$$$$$$| $$/$$ $$ $$| $$$$$$$/
   *   >$$  $$ | $$__  $$| $$$$_  $$$$| $$____/
   *  /$$/\  $$| $$  | $$| $$$/ \  $$$| $$
   * | $$  \ $$| $$  | $$| $$/   \  $$| $$
   * |__/  |__/|__/  |__/|__/     \__/|__/
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

void help(void) {
  printf(                                                                                 "\n"
         "OAWP - Open Animated Wallpaper Player"                                          "\n"
         "Play animated wallpapers in X11 by passing OAWP a directory containing the"     "\n"
         "pictures frames wanted to be displayed."                                        "\n"
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
         "\t\t\t\t"                      "--directory "KBWHT"/home/foo/wallgif/"RST       "\n"
                                                                                          "\n"
         "-c, --config\t\t\t"            "Set another configuration file than the default""\n"
         "\t\t\t\t"                 KBWHT"%s/.config/oawp/oawp.conf"RST" configuration file\n"
                                                                                          "\n"
         "-S, --set-static-wallpaper\t"  "Set a static wallpaper and exit"                "\n"
                                                                                          "\n"
         "Note that OAWP uses a lot of system resources like RAM and CPU!"                "\n"
                                                                                          "\n"
        , getenv("HOME"));
}

void version(void) {
  printf("OAWP version %s" /* version number */                                           "\n"
                                                                                          "\n"
         "Copyright (C) 2023 TheRealOne78"                                                "\n"
         "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.""\n"
         "This is free software: you are free to change and redistribute it."             "\n"
         "There is NO WARRANTY, to the extent permitted by law."                          "\n"
                                                                                          "\n"
        , VERSION);
}
