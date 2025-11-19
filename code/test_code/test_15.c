#include <stdio.h>
#include <stdint.h>

static void print_matrix(int mat[2][3])
{
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            printf("%d ", mat[row][col]);
        }
        printf("\n");
    }
}

static int sum_design(const int *table, size_t size)
{
    int total = 0;
    for (size_t i = 0; i < size; ++i)
    {
        total += table[i];
    }
    return total;
}

int main(void)
{
    int matrix[2][3] = { 1, 2, 3, 4, 5, 6 }; 
    int values[4] = { [0] = 10, [1] = 11, [0] = 12, [2] = 13 }; 
    int design[] = { [3] = 20, [1] = 15 }; 

    print_matrix(matrix);
    for (size_t i = 0; i < 4U; ++i)
    {
        printf("values %zu %d\n", i, values[i]);
    }
    int total = sum_design(design, sizeof(design) / sizeof(design[0]));
    for (size_t j = 0U; j < sizeof(design) / sizeof(design[0]); ++j)
    {
        total += design[j];
        printf("design %zu %d\n", j, total);
    }
    printf("final %d\n", total);
    return total;
}
