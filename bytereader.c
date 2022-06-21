#include<stdio.h>
#include<stdlib.h>
#define ALTERNATE_COLOURS 01
#define OFFSET 02
#define MASK_NUM(n, m)((n) & (m))
unsigned readbytes(const char *fname, int bpl, short mode, long unsigned off, long unsigned len)
{
    // up to 256 bytes per line
    char unsigned cbuf[256];
    unsigned rd = 0, tmp;
    char y = 0;
    FILE *fh = fopen(fname, "rb");
    fseek(fh, off, SEEK_SET);
    tmp = fread(cbuf, 1, bpl, fh);
    while(tmp)
    {
        if(MASK_NUM(mode, OFFSET))
            printf("%08x: ", rd);
        for(int j = 0; j < tmp; j++)
        {
            if(MASK_NUM(mode, ALTERNATE_COLOURS))
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
        }
        puts("\033\133m");
        rd += tmp;
        tmp = fread(cbuf, 1, bpl, fh);
    }
    fclose(fh);
    return rd;
}
int main(int argl, char *argv[])
{
    char const *arg;
    char argtype = 0;
    short mode = 0;
    long unsigned off = 0, len = 0xffffffff;
    unsigned cols = 64;
    for(int i = 1; i < argl; ++i)
    {
        arg = argv[i];
        if(*arg == '-')
        {
            for(const char *it = arg + 1; *it != '\0'; ++it)
            {
                switch(*it)
                {
                    case'n':
                        mode |= OFFSET;
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
                        fprintf(stderr, "\033\13331mUnrecognized option: -%c\033\133m\n", *it);
                }
            }
        }
        else
        {
            switch(argtype)
            {
                case 2:
                    off = strtoul(arg, NULL, 10);
                    break;
                case 1:
                    cols = strtoul(arg, NULL, 10);
                    break;
                default:
                    printf("%s has %u bytes.\n", arg, readbytes(arg, cols, mode, off, len));
            }
            argtype = 0;
        }
    }
    return 0;
}
