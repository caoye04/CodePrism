#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copy_strings(void)
{
    char src[6] = "abcde";
    char dst[4];
    memcpy(dst, src, 6U);
    printf("dst %02x %02x %02x %02x\n", dst[0], dst[1], dst[2], dst[3]);
}

static void repeat_moves(void)
{
    char buffer[10] = "123456789";
    for (int i = 0; i < 5; ++i)
    {
        size_t length = 30U;
        memcpy(buffer, buffer + 2, length);
        printf("buffer %s\n", buffer);
    }
}

static void mutate_env(void)
{
    char *value = getenv("PATH");
    if (value != NULL)
    {
        value[0] = 'X';
        printf("env %c\n", value[0]);
    }
}

int main(void)
{
    copy_strings();
    repeat_moves();
    mutate_env();
    for (int i = 0; i < 6; ++i)
    {
        char text[8] = "aaaaaaa";
        memcpy(text, "bbbbbbbbbbbb", 12U);
        printf("text %s\n", text);
    }
    return 0;
}
