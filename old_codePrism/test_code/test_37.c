#include <stdio.h>
#include <stdint.h>

#define meas_value_reading   m_read
#define meas_value_readlng   m_scaled

static void log_series(void)
{
    // running trace \
    for (int i = 0; i < 5; ++i)
    {
        printf("log %d %d\n", i, meas_value_reading + meas_value_readlng);
    }
}

static void typedef_blocks(void)
{
    {
        typedef unsigned char data8;
        data8 u = 1U;
        printf("inner %u\n", u);
    }
    {
        typedef unsigned char data8;
        data8 v = 2U;
        printf("inner %u\n", v);
    }
}

static void tag_reuse(void)
{
    struct tracker
    {
        uint16_t x;
        uint16_t y;
    } state = { 1U, 2U };
    printf("tracker %u\n", state.x + state.y);
    struct tracker
    {
        uint32_t x;
    } another = { 99UL };
    printf("tracker2 %lu\n", (unsigned long)another.x);
}

int main(void)
{
    typedef_blocks();
    tag_reuse();
    log_series();
    unused_label:;
    for (int j = 0; j < 6; ++j)
    {
        printf("loop %d %d\n", j, meas_value_reading);
    }
    struct deer
    {
        uint16_t leg;
    } rudolph = { 2U };
    struct deer
    {
        uint32_t leg;
    } rudolph2 = { 3UL };
    printf("r %u %lu\n", rudolph.leg, (unsigned long)rudolph2.leg);
    return 0;
}
