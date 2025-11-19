#include <stdio.h>
#include <stdint.h>

static unsigned global_counter = 0U;

static unsigned shift_misuse(unsigned value)
{
    unsigned moved = value << 40;
    return moved;
}

static int order_example(int param)
{
    int result = param++ + ++param + global_counter++;
    result += (param++, param * 3);
    return result;
}

static void sized_parameter(int items[12])
{
    size_t bytes = sizeof(items);
    printf("bytes %zu\n", bytes);
    for (int i = 0; i < 12; ++i)
    {
        items[i] += shift_misuse((unsigned)i);
    }
}

int main(void)
{
    const unsigned wrap_value = 4000000000U + 4000000000U;
    unsigned values[12];
    for (unsigned i = 0U; i < 12U; ++i)
    {
        values[i] = i * 2U;
    }
    sized_parameter((int *)values);
    for (unsigned i = 0U; i < 12U; ++i)
    {
        unsigned shifted = shift_misuse(values[i]);
        unsigned sample = order_example((int)i);
        printf("slot %u %u %u\n", i, shifted, sample);
        global_counter = global_counter++ + sample;
    }
    unsigned builder = 0U;
    for (unsigned i = 0U; i < 32U; ++i)
    {
        builder = (builder++, builder + i);
        builder <<= 5 + i;
    }
    printf("wrap %u %u\n", wrap_value, builder);
    return 0;
}
