#include<stdio.h>
void bytefind(const char *fname, const char *bytes, size_t size)
{
    FILE *fh = fopen(fname, "r");
    if(fh != NULL)
    {
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
                    i += size - 1, printf("bytes found at position 0x%08lx.\n", i + before);
            }
            before += bc;
        }
        fclose(fh);
    }
    else
        printf("File %s could not be read.\n", fname);
}
