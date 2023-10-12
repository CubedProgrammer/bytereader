// This file is part of /CubedProgrammer/bytereader.
// Copyright (C) 2022, github.com/CubedProgrammer, owner of said account.

// /CubedProgrammer/bytereader is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

// /CubedProgrammer/bytereader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along with /CubedProgrammer/bytereader. If not, see <https://www.gnu.org/licenses/>. 

#ifndef Included_bytereader_h
#define Included_bytereader_h
#include<stdio.h>
#define ALTERNATE_COLOURS 01
#define OFFSET 02
#define SPACE_BYTES 04
#define CAPITAL_LETTERS 010
#define MASK_NUM(n, m)((n) & (m))
#define ISINTERACTIVE isatty(STDOUT_FILENO)
void readskip(FILE *f, long unsigned cnt);
FILE *openfile(const char *name, const char *mode);
#endif
