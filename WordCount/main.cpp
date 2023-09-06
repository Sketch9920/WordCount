#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSIZE 8192
char buf[BUFSIZE];

/* ..set default options.. */
bool bBytes = false, bWords = false, bLines = false, bFiles = false;

long tkc, tkl, tkw;

void wc(FILE* fd, const char* f);

/***************************
paraneters:
WordCount.exe -[options] Test.DOC
or
WordCount.exe Test.DOC

****************************/
int main(int argc, char*  argv[])
{

    char* s;
     /* ..process switches.. */
    while (--argc > 0 && (**++argv == '-' || **argv == '/'))
    {
        for (s = *argv + 1; *s; ++s)
            switch (tolower(*s))
            {
            case 'l': bLines = !bLines; break;
            case 'w': bWords = !bWords; break;
            case 'c': bBytes = !bBytes; break;
            case 'f': bFiles = !bFiles;  break;
            default:
                fprintf(stderr, "---> ERROR: bad option: -%c\n", *s);
                exit(1);
            }
    }

    /* ..set all three options by default.. */
    if (!(bBytes || bWords || bLines))
        bBytes = bWords = bLines = true;

    /* ..initialize totals.. */
    tkc = tkl = tkw = 0;

    /* ..process files.. */
    if (argc == 0)
        wc(0, "");
    else
    {
        /* ..check file switch.. */
        if (argc > 1)
            bFiles = !bFiles;

        FILE *fd;

        for (; *argv; ++argv)
            //if ((fd = fopen(*argv, 0)) >= 0)
            if( fopen_s(&fd, *argv, "rb") == 0)
            {
                wc(fd, *argv);
                fclose(fd);
            }
            else
                fprintf(stderr, "---> ERROR: can't open %s\n", *argv);
    }

    /* ..print totals.. */
    if (bFiles)
    {
        if (bBytes) printf("-------- ");
        if (bWords) printf("------- ");
        if (bLines) printf("------ ");
        putchar('\n');
        if (bBytes) printf("%8d ", tkc);
        if (bWords) printf("%7d ", tkw);
        if (bLines) printf("%6d ", tkl);
        putchar('\n');
    }
}

void wc(FILE *fd, const char* f)
{
    long kc, kl, kw;
    bool inword;
    int count;

    int i;

    kc = kw = kl = 0L;
    inword = false;

    while ((count = (int)fread(buf, sizeof(char), BUFSIZE, fd)) > 0)
    {
        kc += count;
        inword = false;
        for (i = 0; i < count; ++i)
            if (buf[i] == '\n')
                ++kl;
            else if (isspace(buf[i]))
                inword = false;
            else if (!inword)
            {
                inword = true;
                kw++;
            }
    }
    if (bBytes) printf("%8d ", kc);
    if (bWords) printf("%7d ", kw);
    if (bLines) printf("%6d ", kl);
    if (bFiles)
        puts(f);
    else
        putchar('\n');

    /* ..accumulate totals.. */
    tkc += kc;
    tkw += kw;
    tkl += kl;
}

