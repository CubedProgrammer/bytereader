// This file is part of /CubedProgrammer/bytereader.
// Copyright (C) 2022, github.com/CubedProgrammer, owner of said account.

// /CubedProgrammer/bytereader is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

// /CubedProgrammer/bytereader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along with /CubedProgrammer/bytereader. If not, see <https://www.gnu.org/licenses/>.

#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#ifdef BYTEREADER_SEARCH_REPLACE
#include"bytefind.h"
#endif
#define ALTERNATE_COLOURS 01
#define OFFSET 02
#define SPACE_BYTES 04
#define MASK_NUM(n, m)((n) & (m))
#define ISINTERACTIVE isatty(STDOUT_FILENO)

unsigned readbytes(const char *fname, int bpl, short mode, long unsigned off, long unsigned len)
{
    // up to 256 bytes per line
    char unsigned cbuf[256];
    unsigned rd = 0, tmp = 0;
    char y = 0;
    FILE *fh = fopen(fname, "rb");
    if(fh != NULL)
    {
        fseek(fh, off, SEEK_SET);
        tmp = fread(cbuf, 1, bpl, fh);
    }
    while(tmp && rd < len)
    {
        if(ISINTERACTIVE && MASK_NUM(mode, OFFSET))
            printf("%08x: ", rd);
        for(int j = 0; rd + j < len && j < tmp; j++)
        {
            if(ISINTERACTIVE && MASK_NUM(mode, ALTERNATE_COLOURS))
            {
                y = !y;
                if(y)
                    fputs("\033\13333m", stdout);
                else
                    fputs("\033\133m", stdout);
            }
            if(cbuf[j] < 16)
                printf("0%x", cbuf[j]);
            else
                printf("%x", cbuf[j]);
            if(!(rd + j + 1 == len || j + 1 == tmp) && MASK_NUM(mode, SPACE_BYTES))
                putchar(' ');
        }
        if(ISINTERACTIVE)
            puts("\033\133m");
        rd += tmp;
        tmp = fread(cbuf, 1, bpl, fh);
    }
    if(fh)
        fclose(fh);
    return rd;
}
int chrnum(char c)
{
    c = tolower(c);
    int d = 0;
    if(c >= '0' && c <= '9')
        d = c - '0';
    else if(c >= 'a' && c <= 'f')
        d = c - 'a' + 10;
    return d;
}
int main(int argl, char *argv[])
{
    char const *arg;
    char searchbuf[90], replbuf[90];
    size_t searchlen, repllen;
    char argtype = 0, search = 0;
    short mode = 0;
    long unsigned off = 0, len = 0xffffffff;
    unsigned cols = 64;
    int succ = 0;
    if(argl == 1)
    {
        printf("%s version 1.6\n", *argv);
        puts("Specify files to be read, bytes will be printed in hexadecimal.\n\nCommand line options...");
        puts("-a to alternate colours for each byte, easier to read.");
        puts("-b to set the offset, let n be the next argument, the first n bytes will be skipped.");
        puts("-c to set the number of bytes per row.");
        puts("-d to put a space between each byte.");
        puts("-f to search for a sequence of bytes, bytes must be given in hexadecimal.");
        puts("-l to set the maximum number of bytes read to the next argument.");
        puts("-n to display byte offset of each row.");
        puts("-r to do a search and replace, next two arguments will be the bytes to search for, and to replace.");
        puts("Make the replace argument + to erase all occurrences of the sequence of bytes with -r.");
        puts("Output will differ based on if stdout is attached to a real terminal.");
    }
    for(int i = 1; i < argl; ++i)
    {
        arg = argv[i];
        if(*arg == '-')
        {
            for(const char *it = arg + 1; *it != '\0'; ++it)
            {
                switch(*it)
                {
#ifdef BYTEREADER_SEARCH_REPLACE
                    case'r':
                        search = 3;
                        break;
#endif
                    case'n':
                        mode |= OFFSET;
                        break;
                    case'l':
                        argtype = 3;
                        break;
#ifdef BYTEREADER_SEARCH_REPLACE
                    case'f':
                        search = 1;
                        break;
#endif
                    case'd':
                        mode |= SPACE_BYTES;
                        break;
                    case'c':
                        argtype = 1;
                        break;
                    case'b':
                        argtype = 2;
                        break;
                    case'a':
                        mode |= ALTERNATE_COLOURS;
                        break;
                    default:
                        succ = 1;
                        fprintf(stderr, "\033\13331mUnrecognized option: -%c\033\133m\n", *it);
                }
            }
        }
        else 
#ifdef BYTEREADER_SEARCH_REPLACE
        if(search & 1)
        {
            searchlen = strlen(arg);
            for(size_t i = 0; i < searchlen; i += 2)
                searchbuf[i >> 1] = chrnum(arg[i]) * 16 + chrnum(arg[i + 1]);
            ++searchlen;
            searchlen >>= 1;
            ++search;
        }
        else if(search == 4)
        {
            repllen = strlen(arg);
            if((repllen & 1) == 1)
                repllen = 0;
            else
            {
                for(size_t i = 0; i < repllen; i += 2)
                    replbuf[i >> 1] = chrnum(arg[i]) * 16 + chrnum(arg[i + 1]);
                ++repllen;
                repllen >>= 1;
            }
            search += 2;
        }
        else
#endif
        {
            switch(argtype)
            {
                case 3:
                    len = strtoul(arg, NULL, 10);
                    break;
                case 2:
                    off = strtoul(arg, NULL, 10);
                    break;
                case 1:
                    cols = strtoul(arg, NULL, 10);
                    break;
                default:
#ifdef BYTEREADER_SEARCH_REPLACE
                    if(search)
                    {
                        if(search == 6)
                            succ = bytereplace(arg, searchbuf, replbuf, searchlen, repllen);
                        else
                            succ = bytefind(arg, cols, searchbuf, searchlen);
                    }
                    else
#endif
                        if(ISINTERACTIVE)
                            printf("%s has %lu bytes.\n", arg, off + readbytes(arg, cols, mode, off, len));
                        else
                            readbytes(arg, cols, mode, off, len);
            }
            argtype = 0;
        }
    }
    return 0;
}
