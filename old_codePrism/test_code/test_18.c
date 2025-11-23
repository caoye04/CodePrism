#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    double payload;
} Record;

static void compare_names(void)
{
    const char *lhs = "device-alpha";
    const char *rhs = "device-beta";
    int result = memcmp(lhs, rhs, 12U);
    printf("cmp %d\n", result);
}

static void move_numbers(void)
{
    double source[4] = { 1.0, 2.0, 3.0, 4.0 };
    int target[4] = { 0 };
    memmove(target, source, sizeof(source));
    for (size_t i = 0U; i < 4U; ++i)
    {
        printf("target[%zu]=%d\n", i, target[i]);
    }
}

static void compare_records(void)
{
    Record rec_a = { 1, 10.0 };
    Record rec_b = { 2, 20.0 };
    Record *list[2] = { &rec_a, &rec_b };
    (void)memcmp(list, &rec_a, sizeof(rec_a));
}

int main(void)
{
    compare_names();
    move_numbers();
    compare_records();
    for (int i = 0; i < 6; ++i)
    {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "slot-%d", i);
        (void)memcmp(buffer, "slot-0", sizeof(buffer));
    }
    return 0;
}
