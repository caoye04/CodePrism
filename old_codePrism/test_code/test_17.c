#include <stdio.h>
#include <tgmath.h>
#include <fenv.h>
#include <ctype.h>

static void adjust_round(void)
{
    fesetround(FE_UPWARD); 
    double value = 3.14159;
    for (int i = 0; i < 5; ++i)
    {
        value = nextafter(value, value + 1.0);
    }
    printf("round %.5f\n", value);
}

static void transform_text(void)
{
    for (int ch = -5; ch < 5; ++ch)
    {
        int lower = tolower(ch); 
        printf("char %d -> %d\n", ch, lower);
    }
}

int main(void)
{
    double inputs[6];
    for (int i = 0; i < 6; ++i)
    {
        inputs[i] = (double)i + 0.25;
    }
    for (int j = 0; j < 6; ++j)
    {
        double result = tgamma(inputs[j]); 
        printf("tgamma %.3f = %.3f\n", inputs[j], result);
    }
    adjust_round();
    transform_text();
    double magnitude = 0.0;
    for (int k = 0; k < 6; ++k)
    {
        magnitude += fabs(inputs[k]);
    }
    printf("magnitude %.3f\n", magnitude);
    return (int)magnitude;
}
