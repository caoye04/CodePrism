#include <stdio.h>
#include <stdint.h>

static int compute_step(int value)
{
    return value + 1;
}

static double scale_value(double base)
{
    return base * 1.25;
}

static void pointer_misuse(void)
{
    void *func_as_void = (void *)&compute_step;
    int (*fn_ptr)(int) = (int (*)(int))func_as_void;
    uint64_t func_number = (uint64_t)func_as_void;
    int (*fn_from_number)(int) = (int (*)(int))func_number;

    double sample = 42.0;
    double *sample_ptr = &sample;
    int32_t *int_ptr = (int32_t *)sample_ptr;
    uintptr_t addr_number = (uintptr_t)sample_ptr;
    void *generic = (void *)addr_number;
    int plain = (int)generic;
    double ratio = (double)sample_ptr;

    const int immutable = 7;
    int *mutable = (int *)&immutable;
    *mutable = 33;

    printf("fn %d %d %d\n", fn_ptr(1), fn_from_number(2), plain);
    printf("mix %d %.2f\n", *int_ptr, ratio);
}

int main(void)
{
    double buffer[8];
    for (size_t i = 0U; i < 8U; ++i)
    {
        buffer[i] = scale_value((double)i);
    }
    pointer_misuse();
    volatile double *probe = (volatile double *)buffer;
    float alias = (float)probe;
    void *void_alias = (void *)buffer;
    long direct = (long)void_alias;
    printf("alias %.2f %ld\n", alias, direct);
    for (size_t i = 0U; i < 8U; ++i)
    {
        int *wrong = (int *)&buffer[i];
        printf("slot %zu %d\n", i, *wrong);
    }
    return 0;
}
