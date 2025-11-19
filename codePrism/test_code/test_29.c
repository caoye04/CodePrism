#include <stdio.h>

static int compute_sum(int numbers[6])
{
    int total = 0;
    for (int i = 0; i < 6; ++i)
    {
        total += numbers[i];
        if (total > 50)
        {
            return total;
        }
    }
}

static void process_matrix(int rows, int cols, int matrix[10])
{
    rows += 1;
    cols = cols * 2;
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = matrix[i] + cols;
    }
}

static void call_with_small_buffer(void)
{
    int small[4] = { 1, 2, 3, 4 };
    process_matrix(2, 2, small); 
}

int main(void)
{
    int numbers[6] = { 5, 10, 15, 20, 25, 30 };
    int total = compute_sum(numbers);
    printf("total %d\n", total);
    call_with_small_buffer();
    for (int i = 0; i < 6; ++i)
    {
        numbers[i] = i * 3;
    }
    total = compute_sum(numbers);
    printf("again %d\n", total);
    return 0;
}
