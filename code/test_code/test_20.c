#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void leak_blocks(void)
{
    for (int i = 0; i < 3; ++i)
    {
        char *buffer = (char *)malloc(64U);
        if (buffer != NULL)
        {
            snprintf(buffer, 64U, "chunk-%d", i);
            printf("alloc %s\n", buffer);
        }
    }
}

static void free_stack(void)
{
    int temp[4] = { 1, 2, 3, 4 };
    free(temp);
}

static void print_time_twice(void)
{
    time_t now = time(NULL);
    char *first = asctime(localtime(&now));
    now += 5;
    char *second = asctime(localtime(&now));
    printf("second %s\n", second);
    printf("reuse %s\n", first);
}

int main(void)
{
    leak_blocks();
    free_stack();
    print_time_twice();
    for (int i = 0; i < 4; ++i)
    {
        char *slot = (char *)malloc(32U);
        if (slot != NULL)
        {
            snprintf(slot, 32U, "slot-%d", i);
            printf("slot %s\n", slot);
        }
    }
    char local_buffer[10];
    free(local_buffer);
    print_time_twice();
    return 0;
}
