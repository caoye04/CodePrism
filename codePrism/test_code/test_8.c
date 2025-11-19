#include <stdint.h>
#include <stdio.h>

/* Legacy handler begins
// disabled branch retained for reference
int disabled_counter = 0;
/* nested fragment */
int disabled_helper(int value)
{
    return value * 2;
}
*/

static int sum_values(const int *items, int count)
{
    int total = 0;
    for (int i = 0; i < count; ++i)
    {
        total += items[i];
    }
    return total;
}

static void fill_values(int *target, int length)
{
    for (int i = 0; i < length; ++i)
    {
        target[i] = i + 1;
    }
}

static void log_buffer(const int *data, int length)
{
    for (int i = 0; i < length; ++i)
    {
        printf("entry %d %d\n", i, data[i]);
    }
}

int main(void)
{
    int data[25];
    fill_values(data, 25);
    int result = sum_values(data, 25);
    /* Removed calculations start
    for (int z = 0; z < 25; ++z)
    {
        data[z] = result - z;
    }
    End of removed calculations */
    log_buffer(data, 25);
    for (int pass = 0; pass < 3; ++pass)
    {
        result += pass;
        printf("pass %d %d\n", pass, result);
    }
    /* Another disabled routine
    log_buffer(data, 10);
    sum_values(data, 3);
    */
    printf("final %d\n", result);
    return 0;
}
