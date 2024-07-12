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

#ifndef __FANCY_TEXT_H__
#define __FANCY_TEXT_H__

#include <stdint.h>

# ifndef _OAWP_ENABLE_COLORS_
  #define _OAWP_ENABLE_COLORS_ 1
  /*
   * --enable-colors
   *
   * When enabled, color sequences will be used
   */

  /* FOREGROUND */
  #define RST "\033[0m"

  /* Normal foreground colors */
  #define KBLK  "\033[30m" //black
  #define KRED  "\033[31m" //red
  #define KGRN  "\033[32m" //green
  #define KYEL  "\033[33m" //yellow
  #define KBLU  "\033[34m" //blue
  #define KMAG  "\033[35m" //magenta
  #define KCYN  "\033[36m" //cyan
  #define KWHT  "\033[37m" //white

  /* Bright foreground colors */
  #define KBBLK "\033[90m" //bright black(gray)
  #define KBRED "\033[91m" //bright red
  #define KBGRN "\033[92m" //bright green
  #define KBYEL "\033[93m" //bright yellow
  #define KBBLU "\033[94m" //bright blue
  #define KBMAG "\033[95m" //bright magenta
  #define KBCYN "\033[96m" //bright cyan
  #define KBWHT "\033[97m" //bright white

  /* Normal background colors */
  #define BBLK  "\033[40m" //black background
  #define BRED  "\033[41m" //red background
  #define BGRN  "\033[42m" //green background
  #define BYEL  "\033[43m" //yellow background
  #define BBLU  "\033[44m" //blue background
  #define BMAG  "\033[45m" //magenta background
  #define BCYN  "\033[46m" //cyan background
  #define BWHT  "\033[47m" //white background

  /* Bright background colors */
  #define BBBLK "\033[100m" //bright black (gray) background
  #define BBRED "\033[101m" //bright red background
  #define BBGRN "\033[102m" //bright green background
  #define BBYEL "\033[103m" //bright yellow background
  #define BBBLU "\033[104m" //bright blue background
  #define BBMAG "\033[105m" //bright magenta background
  #define BBCYN "\033[106m" //bright cyan background
  #define BBWHT "\033[107m" //bright white background

  /* misc */
  #define BOLD  "\033[1m"  //bold
  #define UNDL  "\033[4m"  //underline

# endif  /* _OAWP_ENABLE_COLORS_ */

/* colored status macros */
#define DEBUG_TEXT_PUTS  KBLU "DEBUG" RST
#define INFO_TEXT_PUTS   RST "[" KBLU "i" RST "] " RST // [i]
#define WARN_TEXT_PUTS   RST "[" KYEL "w" RST "] " RST // [w]
#define ERR_TEXT_PUTS    RST "[" KRED "e" RST "] " RST // [e]

#define INFO_TEXT_PUTS_CAST(text)  INFO_TEXT_PUTS text
#define WARN_TEXT_PUTS_CAST(text)  WARN_TEXT_PUTS text
#define ERR_TEXT_PUTS_CAST(text)   ERR_TEXT_PUTS text

/* Print the *AWP logo */
void puts_logo(uint8_t logo);

/* Print the help menu */
void help(void);

/* Print the version of this */
void version(void);

enum awp_logo {
logo_oawp,   /* Standard OAWP */
logo_xawp,   /* X11 */
logo_wawp,   /* Wayland */
logo_mawp,   /* Apple macOS */
logo_winawp  /* Microsoft Windows */
};

#endif
