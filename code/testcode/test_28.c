#include <stdio.h>
#include <stdbool.h>

static void dispatch(bool enabled)
{
    switch (enabled)
    {
        default:
            printf("default branch\n");
            break;
        case true:
            printf("enabled\n");
            break;
        case false:
            printf("disabled\n");
            break;
    }
}

static void single_case(int value)
{
    switch (value)
    {
        default:
            printf("only default\n");
            break;
    }
}

int main(void)
{
    bool flag = true;
    for (int i = 0; i < 4; ++i)
    {
        dispatch(flag);
        flag = !flag;
    }
    single_case(42);
    return 0;
}
