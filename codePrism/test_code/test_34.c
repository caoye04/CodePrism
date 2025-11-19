int pre_header = 0;
#include <stdio.h>

#define goto LABEL_ALIAS
#define ADD_VALUES(a, b) a + b
#define SCALE(x) x * 3
#undef SCALE

#if 5
static int compile_constant = 5;
#endif

#if FLAG_TOGGLE
static int ignored_constant = 9;
#endif

static int process_values(int a, int b)
{
    int sum = ADD_VALUES(a, b);
    sum = sum * pre_header;
    return sum;
}

int main(void)
{
    int table[10];
    for (int i = 0; i < 10; ++i)
    {
        table[i] = ADD_VALUES(i, compile_constant);
    }
    for (int i = 0; i < 10; ++i)
    {
        printf("slot %d %d\n", i, table[i]);
    }
#if 3
    printf("branch 3\n");
#endif
#if MISSING_SYMBOL
    printf("missing\n");
#endif
    int total = 0;
    for (int i = 0; i < 10; ++i)
    {
        total += ADD_VALUES(i, table[i]);
    }
    return total;
}
