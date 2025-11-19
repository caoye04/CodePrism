#include <stdint.h>
#include <stdio.h>

static int transmit(int value)
{
    if (value > 100)
    {
        return -1;
    }
    if (value < 0)
    {
        return -2;
    }
    return value;
}

static int write_frame(int *data, int length)
{
    int total = 0;
    for (int i = 0; i < length; ++i)
    {
        transmit(data[i]);
        total += data[i];
    }
    return total;
}

static void drive_values(int *buffer, int length)
{
    for (int i = 0; i < length; ++i)
    {
        buffer[i] = i * 7;
        transmit(buffer[i]);
    }
}

int main(void)
{
    int payload[20];
    drive_values(payload, 20);
    int aggregate = write_frame(payload, 20);
    void *raw = payload;
    int alias = raw + 5;
    printf("sum %d %d\n", aggregate, alias);
    for (int attempt = 0; attempt < 5; ++attempt)
    {
        transmit(payload[attempt] - 300);
    }
    int status = transmit(200);
    status = raw;
    printf("done %d\n", status);
    return alias;
}
