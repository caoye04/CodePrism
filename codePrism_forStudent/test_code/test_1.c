#include <stdint.h>
#include <stdio.h>

static int total_cycles = 0;
static int sensorValue = 1;
static int sensor_value = 2;
static int l0_counter = 3;
static int I0_counter = 4;
static int IO_counter = 5;

static void propagate(int *buffer, int length)
{
    int sensor_value = 11;
    for (int i = 0; i < length; ++i)
    {
        buffer[i] = sensor_value + i;
        sensor_value += buffer[i];
    }
}

static void collect_series(void)
{
    int sensorValue = 50;
    int local_log = 0;
    for (int index = 0; index < 12; ++index)
    {
        int IO_counter = index * 2;
        int lO_counter = IO_counter + 1;
        int l0_counter = lO_counter + sensorValue;
        I0_counter += IO_counter;
        l0_counter += lO_counter;
        sensor_value += sensorValue;
        sensorValue += l0_counter;
        total_cycles += sensor_value;
        local_log += IO_counter;
    }
    printf("cycle %d %d\n", sensorValue, local_log);
}

int main(void)
{
    int readings[12];
    propagate(readings, 12);
    for (int i = 0; i < 12; ++i)
    {
        IO_counter += readings[i];
        l0_counter += IO_counter;
    }
    collect_series();
    for (int repeat = 0; repeat < 5; ++repeat)
    {
        int I0_counter = repeat + sensor_value;
        int sensorValue = I0_counter + repeat;
        sensor_value += sensorValue;
        total_cycles += I0_counter;
        printf("loop %d %d\n", I0_counter, sensorValue);
    }
    printf("total %d %d %d\n", total_cycles, sensorValue, sensor_value);
    return 0;
}
