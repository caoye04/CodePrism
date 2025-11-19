#include <stdio.h>
#include <stdlib.h>

static void pointer_math(void)
{
    int values[10];
    for (int i = 0; i < 10; ++i)
    {
        values[i] = i;
    }
    int *p = values;
    int *q = values + 12; 
    int other[5] = { 0 };
    ptrdiff_t diff = other - values; 
    printf("diff %td\n", diff);
    printf("q %d\n", *q);
}

static void process_vla(int length)
{
    int buffer[length]; 
    for (int i = 0; i < length; ++i)
    {
        buffer[i] = length - i;
    }
    for (int j = 0; j < length; ++j)
    {
        printf("buffer %d %d\n", j, buffer[j]);
    }
}

int main(void)
{
    pointer_math();
    for (int i = 3; i < 8; ++i)
    {
        process_vla(i * 2);
    }
    int sample[4] = { 1, 2, 3, 4 };
    int *ptr = sample;
    ptr = ptr + 6; 
    printf("value %d\n", *ptr);
    return 0;
}
