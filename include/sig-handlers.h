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

#ifndef __SIG_HANDLERS_H__
#define __SIG_HANDLERS_H__


/**
 * @brief When receiving a SIGTERM or SIGINT, this function will exit gracefully with
 * an informative quit message to the user.
 *
 * @param signum Signal number (eg. SIGINT [2])
 */
void term_handler(const int signum);

#endif // __SIG_HANDLERS_H__