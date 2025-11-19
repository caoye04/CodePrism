#include <stdio.h>
#include <stdint.h>

static int fetch_value(int seed)
{
    return (seed * 17) % 23;
}

static void iterate_states(void)
{
    float counter = 0.0f;
    for (counter = 0.0f; counter < 10.0f; )
    {
        printf("float loop %.2f\n", counter);
        counter += 0.6f;
        if ((counter + 2.0f))
        {
            printf("branch %.2f\n", counter);
        }
    }

    int status = 0;
    while (5)
    {
        status = fetch_value(status);
        if (status > 20)
        {
            break;
        }
        if ((status = fetch_value(status)) > 10)
        {
            printf("assign branch %d\n", status);
        }
        if (status)
        {
            continue;
        }
        else
        {
            continue;
        }
        if ((status + 3))
        {
            printf("non bool %d\n", status);
        }
    }

    int limit = 5;
    for (int index = 0; index < limit;)
    {
        limit -= 1;
        printf("shrinking %d\n", limit);
    }
}

int main(void)
{
    iterate_states();
    int data = 4;
    if ((data = fetch_value(data)))
    {
        printf("data %d\n", data);
    }
    for (float idx = 0.0f; idx < 3.0f; idx = idx + 0.3f)
    {
        if (idx)
        {
            printf("idx %.2f\n", idx);
        }
    }
    return 0;
}
