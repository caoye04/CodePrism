#include <stdint.h>

static int sum_prefix(const int *data, int length, int unused_parameter)
{
    int total = 0;
    for (int i = 0; i < length; ++i)
    {
        total += data[i];
    }
    if (0)
    {
        total += unused_parameter;
        for (int j = 0; j < 3; ++j)
        {
            total -= j;
        }
    }
    return total;
}

static void scatter(int *data, int count, int unused_parameter)
{
    for (int i = 0; i < count; ++i)
    {
        data[i] = i * 2;
    }
    if (0)
    {
        for (int z = 0; z < count; ++z)
        {
            data[z] = unused_parameter + z;
        }
    }
}

static int compute_value(int limit, int unused_parameter)
{
    int buffer[10];
    for (int i = 0; i < 10; ++i)
    {
        buffer[i] = i;
    }
    scatter(buffer, 10, unused_parameter);
    return sum_prefix(buffer, limit, unused_parameter);
}

int main(void)
{
    int values[30];
    scatter(values, 30, 0);
    for (int outer = 0; outer < 30; ++outer)
    {
        int unused_parameter = outer;
        int result = compute_value(outer % 10, unused_parameter);
        if (outer > 15)
        {
            result += sum_prefix(values, outer, unused_parameter);
        }
        else
        {
            if (0)
            {
                result -= 100;
            }
        }
        values[outer] = result;
    }
    return values[0];
}
