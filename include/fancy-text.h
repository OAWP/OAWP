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
#include <stdbool.h>

/**
 * @brief Enable or disable color sequences in logging.
 *
 * Options: --enable-colors / --disable-colors
 *
 * @default --enable-colors
 *
 * When enabled, color sequences will be used. Otherwise, no color will be
 * displayed on the terminal.
 */
#ifndef DISABLE_COLORS
#    define ENABLE_COLORS
#endif

#ifdef ENABLE_COLORS
#    define LOG_USE_COLOR /** LOG_USER_COLOR **/
// > Normal foreground colors <
#    define KBLK  "\033[30m"  // black
#    define KRED  "\033[31m"  // red
#    define KGRN  "\033[32m"  // green
#    define KYEL  "\033[33m"  // yellow
#    define KBLU  "\033[34m"  // blue
#    define KMAG  "\033[35m"  // magenta
#    define KCYN  "\033[36m"  // cyan
#    define KWHT  "\033[37m"  // white
// > Bright foreground colors <
#    define KBBLK "\033[90m"  // bright black(gray)
#    define KBRED "\033[91m"  // bright red
#    define KBGRN "\033[92m"  // bright green
#    define KBYEL "\033[93m"  // bright yellow
#    define KBBLU "\033[94m"  // bright blue
#    define KBMAG "\033[95m"  // bright magenta
#    define KBCYN "\033[96m"  // bright cyan
#    define KBWHT "\033[97m"  // bright white
// > Normal background colors <
#    define BBLK  "\033[40m"  // black background
#    define BRED  "\033[41m"  // red background
#    define BGRN  "\033[42m"  // green background
#    define BYEL  "\033[43m"  // yellow background
#    define BBLU  "\033[44m"  // blue background
#    define BMAG  "\033[45m"  // magenta background
#    define BCYN  "\033[46m"  // cyan background
#    define BWHT  "\033[47m"  // white background
// > Bright background colors <
#    define BBBLK "\033[100m" // bright black (gray) background
#    define BBRED "\033[101m" // bright red background
#    define BBGRN "\033[102m" // bright green background
#    define BBYEL "\033[103m" // bright yellow background
#    define BBBLU "\033[104m" // bright blue background
#    define BBMAG "\033[105m" // bright magenta background
#    define BBCYN "\033[106m" // bright cyan background
#    define BBWHT "\033[107m" // bright white background
// > Miscellaneous <
#    define BOLD  "\033[1m"   // bold
#    define UNDL  "\033[4m"   // underline
#    define RST   "\033[0m"   // reset

#else /** LOG_USER_COLOR **/
// > Normal foreground colors <
#    define KBLK  "" // black
#    define KRED  "" // red
#    define KGRN  "" // green
#    define KYEL  "" // yellow
#    define KBLU  "" // blue
#    define KMAG  "" // magenta
#    define KCYN  "" // cyan
#    define KWHT  "" // white
// > Bright foreground colors <
#    define KBBLK "" // bright black(gray)
#    define KBRED "" // bright red
#    define KBGRN "" // bright green
#    define KBYEL "" // bright yellow
#    define KBBLU "" // bright blue
#    define KBMAG "" // bright magenta
#    define KBCYN "" // bright cyan
#    define KBWHT "" // bright white
// > Normal background colors <
#    define BBLK  "" // black background
#    define BRED  "" // red background
#    define BGRN  "" // green background
#    define BYEL  "" // yellow background
#    define BBLU  "" // blue background
#    define BMAG  "" // magenta background
#    define BCYN  "" // cyan background
#    define BWHT  "" // white background
// > Bright background colors <
#    define BBBLK "" // bright black (gray) background
#    define BBRED "" // bright red background
#    define BBGRN "" // bright green background
#    define BBYEL "" // bright yellow background
#    define BBBLU "" // bright blue background
#    define BBMAG "" // bright magenta background
#    define BBCYN "" // bright cyan background
#    define BBWHT "" // bright white background
// > Miscellaneous <
#    define BOLD  "" // bold
#    define UNDL  "" // underline
#    define RST   "" // reset
#endif  /* ENABLE_COLORS */

typedef enum {
  LOGO_OAWP,  /* Standard OAWP */
#ifdef __APPLE__
  LOGO_MAWP,  /* Apple macOS */
#elif _WIN32
  LOGO_WINAWP, /* Microsoft Windows */
#else
  LOGO_XAWP,  /* X11 */
  LOGO_WAWP,  /* Wayland */
#endif
} AwpLogo;

static bool _is_executable(const char *restrict executable);

/** Print the *AWP logo */
void _puts_logo(const AwpLogo logo);

/** Print the *AWP logo */
void puts_logo_auto(void);

/* Print the help menu */
void help(void);

/* Print the version of this */
void version(void);

#endif
