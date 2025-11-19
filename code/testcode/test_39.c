#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static void type_mix(void)
{
    uint16_t u16a = 100U;
    uint16_t u16b = 300U;
    uint32_t u32a = 0U;
    uint32_t u32b = 5U;
    int32_t s32a = -20;
    int32_t s32b = 40;
    uint8_t counter = 4U;

    bool flag = (bool)(u16a + u32b);   
    printf("flag %d\n", flag);

    uint64_t wide = u16a + u16b;   
    printf("wide %llu\n", (unsigned long long)wide);

    u32a = u32b * (u16a + u16b); 
    printf("mix %u\n", u32a);

    uint16_t narrow = (uint16_t)(s32a + s32b); 
    printf("narrow %u\n", narrow);

    size_t size = sizeof(counter++);  
    printf("size %zu %u\n", size, counter);

    for (int i = 0; i < 3; ++i)
    {
        flag = (bool)(counter + i);
        wide = u16a + u16b;
        u32a += u16a + u16b;
        narrow = (uint16_t)(s32a + s32b);
        size = sizeof(u16a++);
        printf("loop %d %d %llu %u %zu\n", i, flag, (unsigned long long)wide, narrow, size);
    }
}

int main(void)
{
    type_mix();
    return 0;
}
