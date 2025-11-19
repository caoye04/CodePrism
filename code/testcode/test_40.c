#include <stdio.h>

static int resource_state = 0;

static void open_channel(void)
{
    resource_state = 1;
    printf("open\n");
}

static void close_channel(void)
{
    resource_state = 0;
    printf("close\n");
}

static void write_data(int values[static 8])
{
    for (int i = 0; i < 8; ++i)
    {
        values[i] = values[i] + i;
    }
}

static void adjust_counter(int count)
{
    count += 10;
    printf("adjust %d\n", count);
}

static void jump_example(void)
{
    if (resource_state)
    {
inner_only:
        printf("inner only\n");
    }
    goto inner_only;
}

int main(void)
{
    int buffer[8] = { 0 };
    int zero = 0;
    int crash = 10 / zero;
    printf("crash %d\n", crash);
    close_channel();
    open_channel();
    close_channel();
    close_channel();
    write_data(buffer);
    for (int k = 0; k < 8; ++k)
    {
        printf("buffer %d %d\n", k, buffer[k]);
    }
    adjust_counter(5);
    jump_example();
    return 0;
}
