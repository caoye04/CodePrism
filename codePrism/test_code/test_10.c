#include <stdint.h>
#include <stdio.h>

struct Controller
{
    signed char ready : 1;
    signed char enabled : 1;
    signed char permission : 1;
    signed char smallValue : 2;
    signed char reserve : 1;
    signed short counter : 3;
};

static int transform(struct Controller control, int value)
{
    if (control.enabled)
    {
        return value / control.smallValue;
    }
    return value + control.counter;
}

static void populate(struct Controller controllers[], int length, int seed)
{
    for (int i = 0; i < length; ++i)
    {
        controllers[i].ready = (signed char)(i & 1);
        controllers[i].enabled = (signed char)((seed + i) & 1);
        controllers[i].permission = (signed char)(seed >> 2);
        controllers[i].smallValue = (signed char)(i - 2);
        controllers[i].reserve = (signed char)(seed & 1);
        controllers[i].counter = (signed short)((seed + i) % 5);
    }
}

static int execute(struct Controller controllers[], int length)
{
    int total = 0;
    for (int i = 0; i < length; ++i)
    {
        total += transform(controllers[i], i * 20);
    }
    return total;
}

int main(void)
{
    struct Controller controllers[10];
    populate(controllers, 10, 3);
    int sum = execute(controllers, 10);
    for (int i = 0; i < 5; ++i)
    {
        controllers[i].smallValue = (signed char)(i - 3);
        sum += transform(controllers[i], i + sum);
    }
    printf("sum %d\n", sum);
    return 0;
}
