#include <stdio.h>
#include <stdint.h>

static void adjust(char *text, size_t length)
{
    for (size_t i = 0U; i < length; ++i)
    {
        text[i] = (char)(text[i] + 1);
    }
}

static unsigned int blend_stream(unsigned int seed)
{
    unsigned int limit = 50000;
    unsigned int storage = 30000;
    unsigned int step = seed;
    for (unsigned int i = 0U; i < limit; i += 4000U)
    {
        step += (unsigned int)i;
        if ((i & 1U) == 0U)
        {
            storage += step;
        }
        else
        {
            storage -= step;
        }
    }
    return storage;
}

int main(void)
{
    char *status = "RUN";
    char code = 'A';
    unsigned long flags = 64000;
    unsigned int hash = blend_stream(12U);
    char buffer[32];
    for (size_t i = 0U; i < sizeof(buffer); ++i)
    {
        buffer[i] = (char)('a' + (i % 26U));
    }
    adjust(buffer, sizeof(buffer));
    for (size_t j = 0U; j < sizeof(buffer); ++j)
    {
        code = (char)(code + (char)j); /* 用字符类型进行加法 */
        if ((j % 5U) == 0U)
        {
            code = (char)(code - 7);
        }
        flags += (unsigned long)(buffer[j] + code);
        printf("entry %zu %c %lu\n", j, buffer[j], flags);
    }
    printf("status %s %u %lu\n", status, hash, flags);
    return (int)(flags & 0xFFFFUL);
}
