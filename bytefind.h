// This file is part of /CubedProgrammer/bytereader.
// Copyright (C) 2022, github.com/CubedProgrammer, owner of said account.

// /CubedProgrammer/bytereader is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

// /CubedProgrammer/bytereader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along with /CubedProgrammer/bytereader. If not, see <https://www.gnu.org/licenses/>. 

#ifndef Included_bytefind_h
#define Included_bytefind_h
#include<stddef.h>
int bytefind(short mode, const char *fname, size_t cols, const char *bytes, size_t size, long unsigned off, long unsigned len);
int bytereplace(short mode, const char *fname, const char *search, const char *replace, size_t ssize, size_t rsize, long unsigned off, long unsigned len);
#endif
