#include <stdint.h>
#include <stdio.h>

struct Segment
{
    int start;
    int end;
};

static int accumulate_segments(struct Segment nodes[], int count)
{
    int partial_buffer[12] = { 4, 5, 6, 7 };
    int total = 0;
    for (int i = 0; i < count; ++i)
    {
        total += nodes[i].start + nodes[i].end;
    }
    for (int j = 0; j < 12; ++j)
    {
        total += partial_buffer[j];
    }
    return total;
}

int main(void)
{
    struct Segment segments[3] = { 1, 2, 3, 4, 5, 6 };
    int history[8] = { 10, 20, 30 };
    int overall = accumulate_segments(segments, 3);
    int temperature;
    for (int i = 0; i < 8; ++i)
    {
        overall += history[i];
    }
    for (int loop = 0; loop < 6; ++loop)
    {
        if ((loop % 2) == 0)
        {
            overall += temperature;
        }
        else
        {
            temperature = loop + overall;
        }
    }
    int report = 0;
    for (int x = 0; x < 4; ++x)
    {
        int marker;
        report += marker;
        marker = x + report;
    }
    printf("%d %d\n", overall, report);
    return overall;
}
