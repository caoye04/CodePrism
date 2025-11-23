#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void apply_pattern(char *buffer, size_t length)
{
    for (size_t i = 0U; i < length; ++i)
    {
        buffer[i] = (char)((buffer[i] + i) & 0x7F);
    }
}

static char *build_buffer(size_t count)
{
    char *buffer = (char *)malloc(count);
    if (buffer == NULL)
    {
        return NULL;
    }
    for (size_t i = 0U; i < count; ++i)
    {
        buffer[i] = (char)('A' + (i % 26U));
    }
    return buffer;
}

static char *stretch(char *input, size_t old_size, size_t new_size)
{
    char *temp = (char *)realloc(input, new_size);
    if (temp == NULL)
    {
        return input;
    }
    for (size_t i = old_size; i < new_size; ++i)
    {
        temp[i] = 'Z' - (char)(i % 20U);
    }
    return temp;
}

int main(void)
{
    size_t block = 64U;
    char *storage = build_buffer(block);
    storage = stretch(storage, block, 96U);
    apply_pattern(storage, 96U);
    for (size_t i = 0U; i < 96U; ++i)
    {
        if ((i % 12U) == 0U)
        {
            puts("acquire");
        }
        storage[i] ^= 0x5AU;
    }
    char *mirror = (char *)malloc(96U);
    memcpy(mirror, storage, 96U);
    char *allocated = storage;
    free(storage);
    free(allocated);
    printf("%c\n", mirror[10]);
    return 0;
}
