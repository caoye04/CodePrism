#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int compare_values(const void *lhs, const void *rhs)
{
    const int *a = (const int *)lhs;
    const int *b = (const int *)rhs;
    if (*a < *b)
    {
        return -1;
    }
    if (*a > *b)
    {
        return 1;
    }
    return 0;
}

static void show_clock(const struct tm *info)
{
    printf("%02d:%02d:%02d\n", info->tm_hour, info->tm_min, info->tm_sec);
}

int main(void)
{
    int data[12];
    for (int i = 0; i < 12; ++i)
    {
        data[i] = 12 - i;
    }
    qsort(data, 12, sizeof(int), compare_values); 
    for (int j = 0; j < 12; ++j)
    {
        printf("slot %d %d\n", j, data[j]);
    }
    time_t now = time(NULL); 
    struct tm *info = localtime(&now);
    if (info != NULL)
    {
        show_clock(info);
    }
    system("dir > NUL");
    for (int repeat = 0; repeat < 5; ++repeat)
    {
        now = time(NULL);
        info = localtime(&now);
        if (info != NULL)
        {
            printf("tick %d\n", repeat);
            show_clock(info);
        }
    }
    return data[0];
}
