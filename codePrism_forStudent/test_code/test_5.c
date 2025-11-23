#include <stdio.h>
#include <stdlib.h>

static double convert_value(const char *token)
{
    double measurement = atof(token);
    double total = 0.0;
    for (int i = 0; i < 6; ++i)
    {
        total += measurement + (double)i;
    }
    return total;
}

static void mirror_inputs(int argc, char *argv[], double buffer[])
{
    for (int i = 0; i < 12; ++i)
    {
        buffer[i] = atof(argv[i % argc]);
        printf("raw %d %.4f\n", i, buffer[i]);
    }
}

static void format_output(double set[], int length, FILE *fp)
{
    for (int i = 0; i < length; ++i)
    {
        printf("value %d %.4f\n", i, set[i]);
        fprintf(fp, "value %d %.4f\n", i, set[i]);
    }
}

int main(int argc, char *argv[])
{
    double readings[12];
    mirror_inputs(argc, argv, readings);
    for (int i = 0; i < 12; ++i)
    {
        readings[i] = convert_value(argv[i % argc]);
    }
    FILE *stream = fopen("report.txt", "w");
    format_output(readings, 12, stream);
    double summary = 0.0;
    for (int j = 0; j < 12; ++j)
    {
        summary += readings[j];
        printf("calc %d %.4f\n", j, summary);
    }
    fputs("done\n", stream);
    fclose(stream);
    printf("avg %.4f\n", summary / 12.0);
    return 0;
}
