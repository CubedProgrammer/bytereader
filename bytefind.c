// This file is part of /CubedProgrammer/bytereader.
// Copyright (C) 2022, github.com/CubedProgrammer, owner of said account.

// /CubedProgrammer/bytereader is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

// /CubedProgrammer/bytereader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along with /CubedProgrammer/bytereader. If not, see <https://www.gnu.org/licenses/>. 

#include<stdio.h>
#include<sys/stat.h>
#include<unistd.h>
#ifndef FINDBUFSZ
#define FINDBUFSZ 16384
#endif
#define ISINTERACTIVE isatty(STDOUT_FILENO)
int bytefind(const char *fname, size_t cols, const char *bytes, size_t size, long unsigned off, long unsigned len)
{
    FILE *fh = fopen(fname, "rb");
    int succ = 0;
    char realdev = ISINTERACTIVE;
    if(fh != NULL)
    {
        char cbuf[FINDBUFSZ];
        size_t ind = 0, bc, bneed;
        size_t before = off, start = 0, currcol = 0;
        char eq;
        fseek(fh, off, SEEK_CUR);
        if(realdev)
            printf("Searching %s...\n", fname);
        while(!feof(fh) && before < off + len)
        {
            bneed = sizeof cbuf;
            if(before + bneed > off + len)
                bneed = off + len - before;
            bc = fread(cbuf, 1, bneed, fh);
            start = 0;
            if(ind)
            {
                eq = 1;
                for(size_t j = ind; eq && j < size; ++j)
                    eq = bytes[j] == cbuf[j - ind];
                if(eq)
                {
                    if(realdev)
                    {
                        fputs("\033\13332m", stdout);
                        for(size_t j = 0; j < ind; ++j)
                        {
                            printf("%02x", bytes[j] & 0xff);
                            ++currcol;
                            if(currcol == cols)
                            {
                                putchar('\n');
                                currcol = 0;
                            }
                        }
                        for(size_t j = ind; j < size; ++j)
                        {
                            printf("%02x", cbuf[j - ind] & 0xff);
                            ++currcol;
                            if(currcol == cols)
                            {
                                putchar('\n');
                                currcol = 0;
                            }
                        }
                        fputs("\033\133m", stdout);
                    }
                    else
                        printf("%08lx\n", before - ind);
                    start = size - ind;
                }
                else if(realdev)
                {
                    for(size_t j = 0; j < ind; ++j)
                    {
                        printf("%02x", bytes[j] & 0xff);
                        ++currcol;
                        if(currcol == cols)
                        {
                            putchar('\n');
                            currcol = 0;
                        }
                    }
                }
                ind = 0;
            }
            for(size_t i = start; i < bc; ++i)
            {
                eq = 1;
                for(size_t j = 0; eq && j < size; ++j)
                {
                    eq = bytes[j] == cbuf[i + j];
                    if(eq && i + j == bc - 1 && j + 1 < size)
                        ind = j + 1, eq = 0;
                }
                if(eq)
                {
                    if(realdev)
                    {
                        fputs("\033\13332m", stdout);
                        for(size_t j = 0; eq && j < size; ++j)
                        {
                            printf("%02x", cbuf[i + j] & 0xff);
                            ++currcol;
                            if(currcol == cols)
                            {
                                putchar('\n');
                                currcol = 0;
                            }
                        }
                        fputs("\033\133m", stdout);
                    }
                    else
                        printf("%08lx\n", i + before);
                    i += size - 1;
                }
                else if(ind == 0 && realdev)
                {
                    printf("%02x", cbuf[i] & 0xff);
                    ++currcol;
                    if(currcol == cols)
                    {
                        putchar('\n');
                        currcol = 0;
                    }
                }
            }
            start = 0;
            before += bc;
        }
        fclose(fh);
        if(currcol != cols)
            putchar('\n');
    }
    else
        printf("File %s could not be read.\n", fname), succ = -1;
    return succ;
}

int bytereplace(const char *fname, const char *search, const char *replace, size_t ssize, size_t rsize, long unsigned off, long unsigned len)
{
    int succ = 0;
    FILE *fh = fopen(fname, "rb+"), *ofh;
    char tmpname[60];
    char realdev = ISINTERACTIVE;
    struct stat fdat;
    if(ssize == rsize)
        ofh = fh;
    else
    {
        sprintf(tmpname, "bytereader-tmp-%d", getpid());
        ofh = fopen(tmpname, "wb");
    }
    if(fh != NULL)
    {
        char cbuf[FINDBUFSZ];
        size_t ind = 0, bc, bneed;
        size_t before = off, start = 0;
        long pos, oldpos;
        char eq;
        if(ssize != rsize)
        {
            before = 0;
            for(; before < off; before += bc)
            {
                bneed = sizeof cbuf;
                if(bneed + before > off)
                    bneed = off - before;
                bc = fread(cbuf, 1, bneed, fh);
                fwrite(cbuf, 1, bc, ofh);
            }
            before = off;
        }
        else
            fseek(fh, off, SEEK_CUR);
        while(!feof(fh) && before < off + len)
        {
            bneed = sizeof cbuf;
            if(before + bneed > off + len)
                bneed = off + len - before;
            bc = fread(cbuf, 1, bneed, fh);
            start = 0;
            if(ind)
            {
                eq = 1;
                for(size_t j = ind; eq && j < ssize; ++j)
                    eq = search[j] == cbuf[j - ind];
                if(eq)
                {
                    pos = before - ind;
                    if(ofh == fh)
                    {
                        oldpos = ftell(fh);
                        fseek(fh, pos, SEEK_SET);
                    }
                    fwrite(replace, 1, rsize, ofh);
                    if(ofh == fh)
                        fseek(fh, oldpos, SEEK_SET);
                    start = ssize - ind;
                }
                else if(ofh != fh)
                    fwrite(search, 1, ind, ofh);
                ind = 0;
            }
            for(size_t i = start; i < bc; ++i)
            {
                eq = 1;
                for(size_t j = 0; eq && j < ssize; ++j)
                {
                    eq = search[j] == cbuf[i + j];
                    if(eq && i + j == bc - 1 && j + 1 < ssize)
                        ind = j + 1, eq = 0;
                }
                if(eq)
                {
                    pos = i + before;
                    if(ofh == fh)
                    {
                        oldpos = ftell(fh);
                        fseek(fh, pos, SEEK_SET);
                    }
                    fwrite(replace, 1, rsize, ofh);
                    if(ofh == fh)
                        fseek(fh, oldpos, SEEK_SET);
                    i += ssize - 1;
                }
                else if(ofh != fh && ind == 0)
                    fputc(cbuf[i], ofh);
            }
            before += bc;
        }
        if(ssize != rsize)
        {
            while(!feof(fh))
            {
                bneed = sizeof cbuf;
                bc = fread(cbuf, 1, bneed, fh);
                fwrite(cbuf, 1, bc, ofh);
            }
        }
        fclose(fh);
        if(ssize != rsize)
        {
            fclose(ofh);
            stat(fname, &fdat);
            if(rename(tmpname, fname) != 0)
                perror("Moving temporary file failed");
            else if(chmod(fname, fdat.st_mode) != 0)
                perror("chmod failed");
        }
    }
    else
        succ = -1, printf("\033\13331mFile %s could not be opened.\033\133m\n", fname);
    return succ;
}
