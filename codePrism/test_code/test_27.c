#include <stdio.h>

static void handle_status(int status)
{
    switch (status)
    {
        case 0:
            printf("idle\n");
        case 1:
            printf("start\n");
            break;
        case 2:
            printf("running\n");
        case 3:
            printf("finishing\n");
            break;
    }
}

int main(void)
{
    for (int i = 0; i < 5; ++i)
    {
        handle_status(i);
    }
    int state = 0;
    switch (state)
    {
        case 5:
            printf("case 5\n");
            break;
        case 6:
            printf("case 6\n");
        case 7:
            printf("case 7\n");
            break;
    }
    return 0;
}
