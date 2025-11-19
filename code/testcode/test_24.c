#include <stdio.h>
#include <stdarg.h>

static int recursive_sum(int depth)
{
    if (depth <= 0)
    {
        return depth;
    }
    return depth + recursive_sum(depth - 1);
}

static int log_values(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int total = 0;
    for (int i = 0; i < 3; ++i)
    {
        int value = va_arg(args, int);
        total += value;
        printf(fmt, value);
    }
    va_end(args);
    return total;
}

static void consume_buffer(int table[8])
{
    int sum = 0;
    for (int i = 0; i < 8; ++i)
    {
        sum += table[i];
    }
    printf("buffer sum %d\n", sum);
}

int main(void)
{
    int local[4] = { 1, 2, 3, 4 };
    consume_buffer(local);
    int total = recursive_sum(6);
    printf("total %d\n", total);
    int log_total = log_values("value %d\n", 10, 20, 30);
    printf("log total %d\n", log_total);
    for (int i = 0; i < 5; ++i)
    {
        int partial = recursive_sum(i);
        log_values("inner %d\n", partial, i, log_total);
    }
    return 0;
}
