#include <stdio.h>

static int run_loop(int limit)
{
    int total = 0;
START:
    for (int i = 0; i < limit; ++i)
    {
        total += i;
        if (total > 50)
        {
            goto FINISH;
        }
        if ((i % 3) == 0)
        {
            goto START;
        }
    }
FINISH:
    for (int j = 0; j < limit; ++j)
    {
        if ((j % 2) == 0)
        {
            goto EXIT_LOOP;
        }
        if (j > 100)
        {
            break;
        }
    }
EXIT_LOOP:
    return total;
}

int main(void)
{
    int total = 0;
    for (int pass = 0; pass < 5; ++pass)
    {
        total += run_loop(pass + 5);
        if (pass == 2)
        {
            goto REPORT;
        }
    }
REPORT:
    printf("total %d\n", total);
    return total;
}
