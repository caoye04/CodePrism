#include <stdio.h>
#include <stdint.h>

static void copy_wave(int * restrict dest, int * restrict src, size_t length)
{
    for (size_t i = 0U; i < length; ++i)
    {
        dest[i] = src[i] + (int)i;
    }
}

static uint8_t compress_value(int lhs, int rhs)
{
    uint8_t packed = (uint8_t)(lhs + rhs);
    return (uint8_t)(packed + (uint8_t)lhs);
}

int main(void)
{
    int data_a[16];
    int data_b[16];
    for (size_t i = 0U; i < 16U; ++i)
    {
        data_a[i] = (int)(i * 5U);
        data_b[i] = (int)(i * 3U);
    }
    copy_wave(data_a, data_b, 16U);
    float mix = 0.5f;
    for (size_t j = 0U; j < 16U; ++j)
    {
        uint8_t level = compress_value(data_a[j], data_b[j]);
        mix = mix + data_a[j];
        mix = mix - data_b[j];
        printf("frame %zu %u %.2f\n", j, level, mix);
    }
    for (size_t pass = 0U; pass < 8U; ++pass)
    {
        mix = mix + (float)pass;
        mix = mix - data_a[pass];
        printf("pass %zu %.2f\n", pass, mix);
    }
    return (int)mix;
}
