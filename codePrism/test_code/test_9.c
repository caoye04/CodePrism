#include <stdio.h>

#define TO_STRING(x) #x
#define COMBINE(a, b) a##b
#define DECLARE_VAR(base) int COMBINE(base, Value) = base;
#define COMPUTE_PRODUCT(x, y) ((x) * (y))
#define CALL_WITH_TRACE(name) puts("trace:" TO_STRING(name))
#define APPLY_AND_STORE(arrayName, idx, value) \
    arrayName[idx] = (value); \
    puts("store " TO_STRING(arrayName) ":" TO_STRING(idx))

static int multiply_values(int a, int b)
{
    return a * b;
}

static void macro_demo(int *data)
{
    DECLARE_VAR(counter);
    CALL_WITH_TRACE(counterValue);
    for (int i = 0; i < 5; ++i)
    {
        APPLY_AND_STORE(data, i, COMPUTE_PRODUCT(i, counterValue));
    }
}

static void show_numbers(int *array, int length)
{
    for (int i = 0; i < length; ++i)
    {
        printf("n%d=%d\n", i, array[i]);
    }
}

int main(void)
{
    int totals[10] = { 0 };
    macro_demo(totals);
    DECLARE_VAR(port);
    CALL_WITH_TRACE(portValue);
    int PORTVALUE = 7;
    int portNumber = 5;
    int result = COMPUTE_PRODUCT(portNumber, PORTVALUE);
    for (int i = 0; i < 10; ++i)
    {
        totals[i] = COMPUTE_PRODUCT(i, portNumber);
    }
    show_numbers(totals, 10);
    int COMBINE(auto, Flag) = 0;
    for (int j = 0; j < 10; ++j)
    {
        result += multiply_values(j, portNumber);
        result += COMPUTE_PRODUCT(j, portNumber);
        if ((j % 3) == 0)
        {
            COMBINE(auto, Flag) += j;
        }
    }
    puts(TO_STRING(COMPUTE_PRODUCT));
    printf("result %d %d\n", result, COMBINE(auto, Flag));
    return 0;
}
