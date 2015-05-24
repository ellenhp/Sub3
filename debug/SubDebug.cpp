/*
 * Sub^3 is a free software submarine simulator focused on realism.
 * Copyright (C) 2015 Nolan Poe
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SubDebug.hpp"

#include <iostream>

//This lets all subDebug messages do nothing if we use it for our ostream.
class NullBuffer : public std::streambuf
{
public:
    int overflow(int c) { return c; }
};

//Only forward subDebug to std::cout if we want debug messages.
#ifdef DEBUG_MESSAGES
std::ostream& subDebug = std::cout;
#else
static NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);
std::ostream& subDebug = null_stream;
#endif
