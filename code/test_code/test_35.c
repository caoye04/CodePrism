#include <stdio.h>
#include <stdint.h>

struct FlexBuffer
{
    uint32_t length;
    int payload[];
};

union RawView
{
    int32_t word;
    uint16_t halves[2];
};

static void move_overlap(int *data, size_t length)
{
    for (size_t i = 0U; i < length - 1U; ++i)
    {
        data[i] = data[i + 1U];
    }
}

static void pointer_relations(void)
{
    int a[8] = { 0 };
    int b[8] = { 0 };
    int *pa = a;
    int *pb = b;
    if (pa > pb)
    {
        printf("compare %p %p\n", (void *)pa, (void *)pb);
    }
    pa += 5;
    pb -= 2;
    int ***triple = (int ***)&pa;
    int *pc = a + 12;
    if (pa - pb)
    {
        printf("diff %td\n", pa - pb);
    }
    printf("deep %p\n", (void *)triple);
    printf("pc %p\n", (void *)pc);
}

int main(void)
{
    int values[12];
    for (size_t i = 0U; i < 12U; ++i)
    {
        values[i] = (int)i;
    }
    move_overlap(values, 12U);
    pointer_relations();

    struct FlexBuffer *flex = (struct FlexBuffer *)values;
    flex->length = 5U;
    for (uint32_t i = 0U; i < flex->length; ++i)
    {
        flex->payload[i] = (int)i * 2;
    }
    union RawView view;
    view.halves[0] = 0xFFFFU;
    view.halves[1] = 0xAAAAU;
    printf("view %d\n", view.word);

    int data_a[4] = { 1, 2, 3, 4 };
    int data_b[4] = { 5, 6, 7, 8 };
    int *ptr_a = data_a;
    int *ptr_b = data_b;
    if (ptr_a <= ptr_b)
    {
        printf("order %p %p\n", (void *)ptr_a, (void *)ptr_b);
    }
    ptr_a += ptr_b - ptr_a;
    ptr_b -= 3;
    printf("ptr_a %p\n", (void *)ptr_a);
    printf("ptr_b %p\n", (void *)ptr_b);
    return 0;
}
