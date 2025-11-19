#include <stdio.h>

static int evaluate(int mode)
{
    if (mode < 0)
        return -1;
    if (mode == 0)
        return 0;
    if (mode > 10)
    {
        return 10;
    }
    if (mode == 5)
        printf("midpoint\n"); 
    else if (mode == 6)
        printf("after midpoint\n");
    else if (mode == 7)
        printf("seven\n");
    return mode;
}

static void drive_cases(void)
{
    for (int i = -2; i < 8; ++i)
    {
        if (i % 2 == 0)
            printf("even %d\n", i);
        else
            printf("odd %d\n", i);
        if (i == 3)
            printf("special\n");
    }
}

int main(void)
{
    for (int i = -2; i < 12; ++i)
    {
        int value = evaluate(i);
        printf("i=%d -> %d\n", i, value);
    }
    drive_cases();
    if (evaluate(5) > 0)
        printf("positive\n");
    else if (evaluate(-1) < 0)
        printf("negative\n");
    return 0;
}
