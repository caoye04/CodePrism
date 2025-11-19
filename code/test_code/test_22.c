#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static void reuse_stream(void)
{
    FILE *fp = fopen("data.bin", "rb");
    if (fp == NULL)
    {
        return;
    }
    int c = fgetc(fp);
    fclose(fp);
    if (c != EOF)
    {
        fputc(c, fp); 
    }
}

static void check_eof_wrong(void)
{
    FILE *fp = fopen("info.txt", "r");
    if (fp == NULL)
    {
        return;
    }
    int c = fgetc(fp);
    c += 1;
    if (c == EOF)
    {
        puts("end");
    }
    fclose(fp);
}

static void parse_number(const char *text)
{
    long value = strtol(text, NULL, 10);
    if (errno != 0)
    {
        printf("error %d\n", errno);
    }
    else
    {
        printf("value %ld\n", value);
    }
}

int main(void)
{
    reuse_stream();
    check_eof_wrong();
    parse_number("100000000000000000000");
    parse_number("42");
    errno = 5;
    parse_number("84");
    reuse_stream();
    return 0;
}
