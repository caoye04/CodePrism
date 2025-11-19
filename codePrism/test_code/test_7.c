#include <stdint.h>
#include <stdio.h>

static long accumulate_patterns(void)
{
    long base = 60l;
    for (int i = 0; i < 6; ++i)
    {
        base += 070;
    }
    return base;
}

static int mix_constants(int base)
{
    int mask = 0123;
    int flag = 0777;
    long total = 100l;
    long other = 250l;
    for (int i = 0; i < base; ++i)
    {
        total += mask;
        other += flag;
    }
    return (int)(total + other);
}

static void write_values(int *buffer, int length)
{
    int start = 010;
    for (int i = 0; i < length; ++i)
    {
        buffer[i] = start + i * 010;
    }
}

int main(void)
{
    int records[20];
    write_values(records, 20);
    for (int i = 0; i < 20; ++i)
    {
        records[i] += mix_constants(i);
    }
    long result = accumulate_patterns();
    for (int j = 0; j < 20; ++j)
    {
        result += records[j];
        printf("row %d %ld\n", j, result);
    }
    long suffix = 900l;
    suffix += result;
    printf("end %ld\n", suffix);
    return (int)suffix;
}
