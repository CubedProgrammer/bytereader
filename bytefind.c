// This file is part of /CubedProgrammer/bytereader.
// Copyright (C) 2022, github.com/CubedProgrammer, owner of said account.

// /CubedProgrammer/bytereader is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

// /CubedProgrammer/bytereader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along with /CubedProgrammer/bytereader. If not, see <https://www.gnu.org/licenses/>. 

#include<stdio.h>
void bytefind(const char *fname, const char *bytes, size_t size)
{
    FILE *fh = fopen(fname, "r");
    if(fh != NULL)
    {
        printf("Searching %s...\n", fname);
        char cbuf[16384];
        size_t ind = 0, bc, before = 0;
        char eq;
        while(!feof(fh))
        {
            bc = fread(cbuf, 1, sizeof(cbuf), fh);
            if(ind)
            {
                eq = 1;
                for(size_t j = ind; eq && j < size; ++j)
                    eq = bytes[j] == cbuf[j - ind];
                if(eq)
                    printf("bytes found at position 0x%08lx.\n", before - ind);
                ind = 0;
            }
            for(size_t i = 0; i < bc; ++i)
            {
                eq = 1;
                for(size_t j = 0; eq && j < size; ++j)
                {
                    eq = bytes[j] == cbuf[i + j];
                    if(eq && i + j == bc - 1)
                        ind = j + 1, eq = 0;
                }
                if(eq)
                    printf("bytes found at position 0x%08lx.\n", i + before), i += size - 1;
            }
            before += bc;
        }
        fclose(fh);
    }
    else
        printf("File %s could not be read.\n", fname);
}
