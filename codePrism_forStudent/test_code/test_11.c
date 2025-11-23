#include <stdio.h>
#include <stdint.h>

typedef struct OtherType
{
    int32_t id;
    int32_t padding;
} OtherType; 

struct PhantomBlock
{
    int32_t shadow;
};

#define FLAG(value) ((value) ^ 0x55)

static int32_t collect_value(const int32_t *table, size_t length)
{
    int32_t result = 0;
    for (size_t i = 0U; i < length; ++i)
    {
        result += table[i];
        if ((i % 3U) == 0U)
        {
            result -= (int32_t)i;
        }
    }
    return result;
}

static void dump_sequence(void)
{
    int32_t values[10];
    for (size_t i = 0U; i < 10U; ++i)
    {
        values[i] = (int32_t)(i * 4U);
    }
    int32_t total = collect_value(values, 10U);
    for (size_t j = 0U; j < 10U; ++j)
    {
        total += (int32_t)j;
        printf("log %zu %d\n", j, total);
    }
    if (total > 50)
    {
        printf("threshold %d\n", total);
    }
}

int main(void)
{
    int32_t data[12];
    for (size_t i = 0U; i < 12U; ++i)
    {
        data[i] = (int32_t)(i + 3U);
    }
    int32_t result = collect_value(data, 12U);
    dump_sequence();
    for (size_t repeat = 0U; repeat < 6U; ++repeat)
    {
        result += (int32_t)(repeat * 2U);
        printf("repeat %zu %d\n", repeat, result);
    }
    return (int)result;
}
