#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static int compute_factor(int limit)
{
    int value = 1;
    for (int i = 1; i <= limit; ++i)
    {
        value *= i;
    }
    return value;
}

static void convert_text(const char *text)
{
    errno = 0;
    (void)strtol(text, NULL, 10);
}

static void random_log(void)
{
    puts("log");
    if (errno != 0)
    {
        printf("unexpected errno %d\n", errno);
    }
}

int main(void)
{
    convert_text("100000000000000000000");
    random_log();
    convert_text("12");
    int result = compute_factor(8);
    compute_factor(5);
    printf("result %d\n", result);
    convert_text("3000000000000000000000");
    puts("done");
    if (errno != 0)
    {
        printf("last errno %d\n", errno);
    }
    return 0;
}
