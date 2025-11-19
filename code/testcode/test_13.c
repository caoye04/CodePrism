#include <stdio.h>
#include <stdint.h>

int helper_sum(int a, int b)
{
    return a + b;
}

inline int scale_value(int seed)
{
    return seed * 3;
}

int shared_buffer[6];
int only_main_buffer[12];

static void fill_buffer(void)
{
    for (int i = 0; i < 6; ++i)
    {
        shared_buffer[i] = i * 2;
    }
}

static int walk_values(int limit)
{
    int total = 0;
    for (int i = 0; i < limit; ++i)
    {
        total += helper_sum(i, shared_buffer[i % 6]);
    }
    return total;
}

int main(void)
{
    fill_buffer();
    int log_entries = 0;
    for (int k = 0; k < 12; ++k)
    {
        only_main_buffer[k] = k + 5;
    }
    for (int pass = 0; pass < 5; ++pass)
    {
        int base = scale_value(pass);
        log_entries += helper_sum(base, pass);
        printf("pass %d %d\n", pass, log_entries);
    }
    for (int i = 0; i < 6; ++i)
    {
        shared_buffer[i] += i;
    }
    int total = walk_values(20);
    for (int idx = 0; idx < 10; ++idx)
    {
        only_main_buffer[idx] += idx;
        total += scale_value(idx);
        printf("idx %d %d\n", idx, total);
    }
    printf("final %d\n", total);
    return total;
}
